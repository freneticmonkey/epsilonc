#include "script/ScriptManager.h"

namespace epsilon
{
	ScriptManager::ScriptManager() : scriptsFolderPath("resources/scripts/"),
									 stdErrListener("error"), 
									 stdOutListener("")
	{
	}

	ScriptManager::~ScriptManager()
	{
	}

	Script::Ptr ScriptManager::CreateScript()
	{
		Script::Ptr newScript = Script::Create();
		newScript->InitScript();
		scriptList.push_back(newScript);
		return newScript;
	}

	Script::Ptr ScriptManager::CreateScript(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;

		Script::Ptr newScript = Script::Create(filename);
		newScript->InitScript();
		scriptList.push_back(newScript);
		return newScript;
	}

	ScriptBehaviour::Ptr ScriptManager::CreateBehaviour(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;

		ScriptBehaviour::Ptr newBehaviour = ScriptBehaviour::Create(filename, ScriptSource::FILE);
		newBehaviour->InitScript();
		behaviourList.push_back(newBehaviour);
		startingBehaviours.push_back(newBehaviour);
		return newBehaviour;
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

			// Insert the scripts path into the Python sys.path
			import("sys").attr("path").attr("insert")(0, str(scriptsFolderPath.c_str()));

			// Get Module Global namespace
			pythonGlobalModule = import("__main__");
			pythonGlobalNamespace = extract<dict>(pythonGlobalModule.attr("__dict__"));

			// Import and insert the Epsilon Module into the Global namespace
			epsilonModule = import("epsilon");
			pythonGlobalNamespace["epsilon"] = epsilonModule;

			// Immediately override std err & out to Log
			import("sys").attr("stderr") = stdErrListener;
			import("sys").attr("stdout") = stdOutListener;

		}
		catch (const error_already_set&)
		{
			if (PyErr_Occurred()) 
			{
				PrintPythonError();
			}
		}
	}

	void ScriptManager::ReloadScript(Script::Ptr script)
	{
		script->InitScript();
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
					(*behaviour)->OnStart();
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

				//++behaviour;
			}

			/*
			for ( BehaviourList::iterator behaviour = startingBehaviours.begin(); behaviour != startingBehaviours.end(); behaviour++)
			{
				try
				{
					(*behaviour)->OnStart();
				}
				catch (const error_already_set&)
				{
					if (PyErr_Occurred()) 
					{
						PrintPythonError();
					}
				}
			}
			*/
		}
		
		// Empty the list so that start is not run again
		startingBehaviours.clear();
	}
	
	void ScriptManager::Update(float dt)
	{
		// TODO: Move to an OnFrameStart or equivalent function
		// so that scripts can receive the start and update events
		// in the same frame.
		StartBehaviours();

		for ( BehaviourList::iterator behaviour = behaviourList.begin(); behaviour != behaviourList.end(); behaviour++)
		{
			try
			{
				(*behaviour)->Update(dt);
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}
	}

}