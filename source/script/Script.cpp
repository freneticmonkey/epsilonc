#include "script/Script.h"
#include "script/ScriptCommon.h"

namespace epsilon
{
	const std::string Script::INSTANCE_VAR_NAME = "_instance";

	Script::Ptr Script::Create()
	{
		return std::make_shared<Script>(private_struct());
	}

	Script::Ptr Script::CreateFromFile(std::string filename)
	{
		return std::make_shared<Script>(private_struct(), filename, ScriptSource::FILE);
	}

	Script::Ptr Script::CreateFromText(std::string scriptString)
	{
		return std::make_shared<Script>(private_struct(), scriptString, ScriptSource::TEXT);
	}

	Script::Script(const private_struct &) : NodeComponent("Script"),
											 filename(""),
											 text(""),
											 scriptSource(ScriptSource::NONE),
											 initialised(false),
											 Resource("", ResourceType::Type::SCRIPT)
	{
		// Name the Script using its Object ID
		std::stringstream ss;
		ss << id;
		objectName = std::string("Script_") + ss.str();
	}

	Script::Script(const private_struct &, std::string filepath) : NodeComponent("Script"),
																	scriptSource(ScriptSource::FILE),
																	filename(filepath),
																	initialised(false),
																	text(""),
																	Resource(filepath, ResourceType::Type::SCRIPT)
	{
		// Name the script
		// strip the path and just use the filename
		unsigned spos = filename.rfind("/");
		if (spos != std::string::npos)
		{
			objectName = filename.substr(spos + 1, std::string::npos);
		}
		objectName = filename;
	}

	Script::Script(const private_struct &, std::string scriptString, ScriptSource source) : NodeComponent("Script"),
																							scriptSource(source),
																							initialised(false),
																							Resource("", ResourceType::Type::SCRIPT)
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

		// Name the script

		// If a file
		if ( scriptSource ==  ScriptSource::FILE )
		{
			// strip the path and just use the filename
			unsigned spos = scriptString.rfind("/");
			if ( spos != std::string::npos )
			{
				objectName = scriptString.substr(spos+1, std::string::npos);
			}
			objectName = scriptString;
		}
		else
		{
			// Name the script using its Object ID
			std::stringstream ss;
			ss << id;
			objectName = std::string("Script_") + ss.str();
		}
	}
	
	Script::~Script(void)
	{
	}

	bool Script::InitScript()
	{
		try
		{
			// Build a local namespace for this script, 
			// so scripts don't stomp over each others global namespace
			scriptLocalNamespace["__builtins__"] = import("__builtin__");
			scriptLocalNamespace["epsilon"] = import("epsilon");

			// TODO: Add better exception handling here
			// Load the python source into the local namespace.
			switch( scriptSource )
			{
				case ScriptSource::FILE:
					exec_file(filename.c_str(), scriptLocalNamespace);
					break;
				case ScriptSource::TEXT:
					exec(text.c_str(), scriptLocalNamespace);
					break;
				default:
					break;
			}

			// Register the variable '_instance' in the local namespace as the modules
			// 'class'.  If the python module contains a class definition, creating an
			// instance of it named '_instance' will allow the script to link to functions
			// within the class
			RegisterPythonClass();
			
			// Register any functions that need to be exposed by this script.
			RegisterScriptFunctions();

			// For Hotloading.
			if (this->componentParent)
			{
				OnSetParent();
			}

			initialised = true;
		}
		catch (const error_already_set&)
		{
			if (PyErr_Occurred()) 
			{
				PrintPythonError();
			}	
		}

		// Indicated that the Script Resource has been refreshed
		SetReloaded();

		return initialised;
	}

	void Script::OnSetParent()
	{
		// If there's a class object
		if ( !behaviourClass.is_none() )
		{
			// Inject the node as a class property
			dict classDict = extract<dict>(behaviourClass.attr("__dict__"));
			classDict["node"] = this->componentParent;
		}
		else
		{
			// Otherwise just inject it into the local namespace
			scriptLocalNamespace["node"] = this->componentParent;
		}
	}

	object Script::FindPythonFunction(std::string funcName)
	{
		object targetFunction;

		// Firstly, check for a class instance
		if ( !behaviourClass.is_none() )
		{
			if ( behaviourClass.attr(funcName.c_str()) )
			{
				targetFunction = behaviourClass.attr(funcName.c_str());
			}
		}
		
		// Check the local namespace
		else if ( scriptLocalNamespace.has_key(funcName) )
		{
			targetFunction = scriptLocalNamespace[funcName];
		}
		
		// If search unsuccessfull show in the Log
		if ( targetFunction.is_none() )
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
			Log(Format("%s method not found in script: %s", funcName.c_str(), scriptName.c_str()));
		}
		return targetFunction;
	}

	void Script::RegisterPythonClass()
	{
		if ( scriptLocalNamespace.contains(INSTANCE_VAR_NAME) )
		{
			behaviourClass = scriptLocalNamespace[INSTANCE_VAR_NAME];
		}
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
}