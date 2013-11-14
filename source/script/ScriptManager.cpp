#include "script/ScriptManager.h"



namespace epsilon
{
	ScriptManager::ScriptManager()
	{
		scriptsFolderPath = std::string("resources/scripts/");
	}

	ScriptManager::~ScriptManager()
	{

	}

	Script::Ptr ScriptManager::CreateScript()
	{
		Script::Ptr newScript = Script::Create();
		newScript->InitScript(pythonGlobalNamespace);
		scriptList.push_back(newScript);
		return newScript;
	}

	Script::Ptr ScriptManager::CreateScript(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;

		Script::Ptr newScript = Script::Create(filename);
		newScript->InitScript(pythonGlobalNamespace);
		scriptList.push_back(newScript);
		return newScript;
	}

	ScriptBehaviour::Ptr ScriptManager::CreateBehaviour(std::string filename)
	{
		// Prepend the scripts folder path
		filename = scriptsFolderPath + filename;

		ScriptBehaviour::Ptr newBehaviour = ScriptBehaviour::Create(filename, ScriptSource::FILE);
		newBehaviour->InitScript(pythonGlobalNamespace);
		behaviourList.push_back(newBehaviour);
		startingBehaviours.push_back(newBehaviour);
		return newBehaviour;
	}

	void ScriptManager::Setup()
	{
		// Add Epsilon Module to the Python initalisation startup
		PyImport_AppendInittab( "epsilon", &initepsilon);

		// Start Python
		Py_Initialize();

		// Get Module Global namespace
		pythonGlobalModule = import("__main__");
		pythonGlobalNamespace = extract<dict>(pythonGlobalModule.attr("__dict__"));

		// Import and insert the Epsilon Module into the Global namespace
		epsilonModule = import("epsilon");
		pythonGlobalNamespace["epsilon"] = epsilonModule;
	}

	void ScriptManager::ReloadScript(Script::Ptr script)
	{
		script->InitScript(pythonGlobalNamespace);
	}

	void ScriptManager::StartBehaviours()
	{
		// Run start for each behaviour
		if ( startingBehaviours.size() > 0 )
		{
			for ( BehaviourList::iterator behaviour = startingBehaviours.begin(); behaviour != startingBehaviours.end(); behaviour++)
			{
				try
				{
					(*behaviour)->OnStart();
				}
				catch (error_already_set& e)
				{
					if (PyErr_Occurred()) 
					{
						PrintPythonError();
					}
				}
			}
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
			catch (error_already_set& e)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}
	}

}