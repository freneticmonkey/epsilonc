#include "script/Script.h"
#include "script/ScriptCommon.h"

namespace epsilon
{
	Script::Ptr Script::Create()
	{
		return make_shared<Script>(private_struct());
	}

	Script::Ptr Script::Create(std::string scriptString, ScriptSource source)
	{
		return make_shared<Script>(private_struct(), scriptString, source);
	}

	Script::Script(const private_struct &) : NodeComponent("Script"), 
											 filename(""),
											 text(""),
											 scriptSource(ScriptSource::NONE),
											 initialised(false)
	{
	}

	Script::Script(const private_struct &, std::string scriptString, ScriptSource source) : NodeComponent("Script"),
																							scriptSource(source),
																							initialised(false)
	{
		switch(scriptSource)
		{
			case ScriptSource::FILE:
				filename = scriptString;
				text = "";
				break;
			case ScriptSource::TEXT:
				filename = "";
				text = scriptString;
				break;
		}
	}
	
	Script::~Script(void)
	{
	}

	void Script::InitScript(dict pythonGlobalNamespace)
	{
		try
		{
			// TODO: Add better exception handling here
			// Load the python source into the global namespace.
			switch( scriptSource )
			{
				case ScriptSource::FILE:
					exec_file(filename.c_str(), pythonGlobalNamespace);
					break;
				case ScriptSource::TEXT:
					exec(text.c_str(), pythonGlobalNamespace);
					break;
				default:
					break;
			}
			
			// Register any functions that need to be exposed by this script.
			RegisterScriptFunctions(pythonGlobalNamespace);
		}
		catch (error_already_set& e)
		{
			if (PyErr_Occurred()) 
			{
				PrintPythonError();
			}	
		}
		initialised = true;
	}

	object Script::FindPythonFunction(std::string funcName, dict pythonNamespace)
	{
		object targetFunction;
		if ( pythonNamespace.contains(funcName) )
		{
			targetFunction = pythonNamespace[funcName];
		}
		else
		{
			std::string scriptName;
			switch( scriptSource )
			{
				case ScriptSource::FILE:
					scriptName = filename;
					break;
				case ScriptSource::TEXT:
					scriptName = ( text.size() > 10 ) ? text.substr(0, 10) : text;
					break;
				default:
					break;
			}
			Log(Format("%s method not found in script: %s", funcName, scriptName));
		}
		return targetFunction;
	}

	void Script::RegisterScriptFunctions(dict pythonNamespace)
	{
		//updateFunction = FindPythonFunction("update", pythonNamespace);
	}

	Script::Ptr Script::SetScriptFile(std::string scriptFilename)
	{
		filename = scriptFilename;
		// Clear text in preparation for next init
		text = "";
		scriptSource = ScriptSource::FILE;
		return ThisPtr();
	}

	Script::Ptr Script::SetScriptText(std::string scriptText)
	{
		text = scriptText;
		scriptSource = ScriptSource::TEXT;
		return ThisPtr();
	}
	/*
	void Script::Update(float dt)
	{
		object result;
		if ( !updateFunction.is_none() )
		{
			result = updateFunction(dt);
		}
	}
	*/
}