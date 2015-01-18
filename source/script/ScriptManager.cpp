#include "script/ScriptManager.h"
#include "utilities/Utilities.h"
#include "resource/ResourceManager.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	ScriptManager::ScriptManager() : scriptsFolderPath("/scripts/"),
									 stdErrListener("error"), 
									 stdOutListener(""),
									 engineCoreScript(ScriptEngineCore::Create()),
									 gilLockCount(0),
									 currentChangedQueue(0),
									 activeResourceId(-1)
	{
	}

	ScriptManager::~ScriptManager()
	{
	}

	Script::Ptr ScriptManager::CreateScript()
	{
		Script::Ptr newScript = Script::Create();
		ReloadScript(newScript);
		scriptList.push_back(newScript);
		return newScript;
	}

	Script::Ptr ScriptManager::CreateScript(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;
		Script::Ptr newScript = Script::CreateFromFile(filename);
		RegisterResource(newScript);
		ReloadScript(newScript);
		scriptList.push_back(newScript);
		return newScript;
	}

	ScriptBehaviour::Ptr ScriptManager::CreateBehaviour(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;

		ScriptBehaviour::Ptr newBehaviour = ScriptBehaviour::CreateFromFile(filename);
		RegisterResource(newBehaviour);
		ReloadScript(newBehaviour);
		behaviourList.push_back(newBehaviour);
		startingBehaviours.push_back(newBehaviour);
		return newBehaviour;
	}

	void ScriptManager::AddBehaviour(ScriptBehaviour::Ptr newBehaviour)
	{
		RegisterResource(newBehaviour);
		ReloadScript(newBehaviour);
		behaviourList.push_back(newBehaviour);
		startingBehaviours.push_back(newBehaviour);
	}

	void ScriptManager::Setup()
	{
		try
		{
			// Add Epsilon Module to the Python initalisation startup
			PyImport_AppendInittab( "epsilon", &initepsilon);
		
			// Set the Program Name to be epsilon
			Py_SetProgramName("epsilon");

			// Start Python
			Py_Initialize();

			// Init GIL/Thread handling
			PyEval_InitThreads();
			gilLockCount = 1;

			// Insert the resources/scripts path into the Python sys.path
			std::string resourcePath = ResourceManager::GetInstance().GetBasePath() + scriptsFolderPath;
            import("sys").attr("path").attr("insert")(0, python::str(resourcePath.c_str()));
            
            // Also change the python cwd to the resources folder
            import("os").attr("chdir")(python::str(ResourceManager::GetInstance().GetBasePath()));
            
			// Get Module Global namespace
			pythonGlobalModule = import("__main__");
			pythonGlobalNamespace = extract<dict>(pythonGlobalModule.attr("__dict__"));

			// Import and insert the Epsilon Module into the Global namespace
			epsilonModule = import("epsilon");
			pythonGlobalNamespace["epsilon"] = epsilonModule;

			// Immediately override std err & out to Log
			import("sys").attr("stderr") = stdErrListener;
			import("sys").attr("stdout") = stdOutListener;

			// Release the GIL and allow threads to do their thing
			ReleaseGIL();
		}
		catch (const error_already_set&)
		{
			if (PyErr_Occurred()) 
			{
				PrintPythonError();
			}
		}
	}

	bool ScriptManager::StartEngineCore()
	{
		bool success = false;
		LockGIL();

		// Register the Script Engine Core with the ResourceManager for hotloading
		RegisterResource(engineCoreScript);

		activeResourceId = engineCoreScript->GetResourceId();

		// Init the Python Script Engine Core
		if (engineCoreScript->InitScript() && !engineCoreScript->InError() )
		{
			// Start it immediately
			success = engineCoreScript->OnStart();
		}

		activeResourceId = -1;

		ReleaseGIL();
		return success;
	}

	void ScriptManager::BehaviourOnDrawGizmos()
	{
		LockGIL();
		
		// Run OnDrawGizmos for each behaviour
		for (ScriptBehaviour::Ptr behaviour : behaviourList)
		{
			try
			{
				if (!behaviour->InError())
				{
					behaviour->OnDrawGizmos();
				}
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred())
				{
					PrintPythonError();
				}
			}
		}

		ReleaseGIL();
	}

	void ScriptManager::BehaviourOnGUI()
	{
		LockGIL();

		// Run OnGUI for each behaviour
		for (ScriptBehaviour::Ptr behaviour : behaviourList)
		{
			try
			{
				if (!behaviour->InError())
				{
					behaviour->OnGUI();
				}
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred())
				{
					PrintPythonError();
				}
			}
		}

		ReleaseGIL();
	}

	void ScriptManager::ReloadScript(Script::Ptr script)
	{
		bool success = false;
		LockGIL();

		activeResourceId = script->GetResourceId();

		// Notify the script of the reload
		script->BeforeReload();

		success = script->InitScript();

		// Notify the script that the reload has finished
		script->AfterReload();

		activeResourceId = -1;

		ReleaseGIL();
		// return success;
	}

	bool ScriptManager::DestroyScript(Script::Ptr script)
	{
		bool success = false;

		BehaviourList::iterator behaviour = std::find_if(behaviourList.begin(), behaviourList.end(), [&](Script::Ptr scr){
			return scr == script;
		});

		if (behaviour != behaviourList.end())
		{
			try
			{
				// Firstly call the OnDestroy function
				if (!script->InError())
				{
					activeResourceId = script->GetResourceId();
					script->OnDestroy();
					activeResourceId = -1;
				}

				// Remove the script
				behaviourList.erase(behaviour);
			}
			catch (const error_already_set&)
			{
				// If errors occur, do the standard error and postfix with destroy error.
				if (PyErr_Occurred())
				{
					PrintPythonError();
				}
				Log("ScriptManager", boost::str(format("Error while destroying script: %s") % script->GetFilename()) );
			}
		}
		
		return success;
	}

	void ScriptManager::StartBehaviours()
	{
		// Run start for each behaviour
		if ( startingBehaviours.size() > 0 )
		{
			BehaviourList::iterator behaviour = startingBehaviours.begin();

			while ( behaviour != startingBehaviours.end() )
			{
				try
				{
					if (!(*behaviour)->InError())
					{
						activeResourceId = (*behaviour)->GetResourceId();
						(*behaviour)->OnStart();
						activeResourceId = -1;
					}
				}
				catch (const error_already_set&)
				{
					if (PyErr_Occurred()) 
					{
						PrintPythonError();
					}
				}
				// Remove this behaviour from the starting behaviours list
				behaviour = startingBehaviours.erase(behaviour);

				if ( behaviour == startingBehaviours.end() || startingBehaviours.size() == 0 )
				{
					break;
				}
			}
		}
		
		// Empty the list so that start is not run again
		startingBehaviours.clear();
	}
	
	void ScriptManager::Update(float dt)
	{
		LockGIL();

		// Process the Python Engine
		engineCoreScript->Update(dt);

		// TODO: Move to an OnFrameStart or equivalent function
		// so that scripts can receive the start and update events
		// in the same frame.
		StartBehaviours();

		for ( BehaviourList::iterator behaviour = behaviourList.begin(); behaviour != behaviourList.end(); behaviour++)
		{
			try
			{
				if (!(*behaviour)->InError())
				{
					activeResourceId = (*behaviour)->GetResourceId();
					(*behaviour)->Update(dt);
					activeResourceId = -1;
				}
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}

		// While we still have the GIL, reload any changed scripts
		ProcessChangedResources();

		ReleaseGIL();
	}

	void ScriptManager::Destroy()
	{
		LockGIL();

		// Send Destroy events to all of the behaviours
		for ( BehaviourList::iterator behaviour = behaviourList.begin(); behaviour != behaviourList.end(); behaviour++)
		{
			try
			{
				if (!(*behaviour)->InError())
				{
					activeResourceId = (*behaviour)->GetResourceId();
					(*behaviour)->OnDestroy();
					activeResourceId = -1;
				}
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}

		// Notify the Python Engine
		engineCoreScript->OnDestroy();

		ReleaseGIL();
	}

	void ScriptManager::RefreshResources(ResourceIdVector changedResources)
	{
		// If there are currently queued items in the resources changed queue
		if (resourceChangedQueue[currentChangedQueue].size() > 0)
		{
			// Merge the queues
			ResourceIdVector tmp;
			tmp.reserve(resourceChangedQueue[currentChangedQueue].size() + changedResources.size());
			std::merge(changedResources.begin(), changedResources.end(), resourceChangedQueue[currentChangedQueue].begin(), resourceChangedQueue[currentChangedQueue].end(), std::back_inserter(tmp));
			resourceChangedQueue[currentChangedQueue].swap(tmp);
		}
		else
		{
			// otherwise just assign
			resourceChangedQueue[currentChangedQueue] = changedResources;
		}
		
	}

	void ScriptManager::ProcessChangedResources()
	{
		if (resourceChangedQueue[currentChangedQueue].size() > 0)
		{
			// Swap the queues
			currentChangedQueue = !currentChangedQueue;

			Log("ScriptManager", boost::str(format("Refreshing Scripts: #: %d") % resourceChangedQueue[!currentChangedQueue].size()));

			// Firstly check if the ScriptEngineCore needs to be refreshed.
			if (std::find(resourceChangedQueue[!currentChangedQueue].begin(), resourceChangedQueue[!currentChangedQueue].end(), engineCoreScript->GetResourceId()) != resourceChangedQueue[!currentChangedQueue].end())
			{
				// Restart the engine core.
				ReloadScript(engineCoreScript);

				// Technically, shouldn't everything be refreshed here?
			}

			// Now check each of the scripts and behaviours
			std::for_each(resourceChangedQueue[!currentChangedQueue].begin(), resourceChangedQueue[!currentChangedQueue].end(), [&](std::size_t resourceId){
				// Check Scripts
				ScriptList::iterator foundScript = std::find_if(scriptList.begin(), scriptList.end(), [&](Script::Ptr script){
					return script->GetResourceId() == resourceId;
				});

				if (foundScript != scriptList.end())
				{
					Log("ScriptManager", "Reloading Script: " + (*foundScript)->GetFilepath().GetString() );
					ReloadScript(*foundScript);
				}

				// Check Behaviours
				BehaviourList::iterator foundBehaviour = std::find_if(behaviourList.begin(), behaviourList.end(), [&](ScriptBehaviour::Ptr behaviour){
					return behaviour->GetResourceId() == resourceId;
				});

				if (foundBehaviour != behaviourList.end())
				{
					Log("ScriptManager", "Reloading Behaviour: " + (*foundBehaviour)->GetFilepath().GetString() );

					ReloadScript(*foundBehaviour);
				}
			});

			// empty out the changed resources
			resourceChangedQueue[!currentChangedQueue].clear();
		}
	}

}