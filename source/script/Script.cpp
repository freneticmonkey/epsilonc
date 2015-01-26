#include "script/Script.h"
#include "script/ScriptCommon.h"
#include "resource/ResourceManager.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

using namespace boost;

namespace epsilon
{
	const std::string Script::INSTANCE_VAR_NAME = "_instance";

	Script::Ptr Script::Create()
	{
		return std::make_shared<Script>(private_struct());
	}

	Script::Ptr Script::CreateFromFile(std::string filename)
	{
        return std::make_shared<Script>(private_struct(), filename, ScriptSource::SS_FILE);
	}

	Script::Ptr Script::CreateFromText(std::string scriptString)
	{
		return std::make_shared<Script>(private_struct(), scriptString, ScriptSource::SS_TEXT);
	}

	Script::Script(const private_struct &) : NodeComponent("Script"),
											 text(""),
											 scriptSource(ScriptSource::SS_NONE),
											 initialised(false),
											 compileError(false),
											 Resource("", ResourceType::Type::SCRIPT)
	{
		// Name the Script using its Object ID
		std::stringstream ss;
		ss << id;
		objectName = std::string("Script_") + ss.str();
	}

	Script::Script(const private_struct &, std::string filepath) : NodeComponent("Script"),
																	scriptSource(ScriptSource::SS_FILE),
																	initialised(false),
																	text(""),
																	compileError(false),
																	Resource(filepath, ResourceType::Type::SCRIPT)
	{
		// Name the script
		// strip the path and just use the filename
		unsigned spos = filepath.rfind("/");
		if (spos != std::string::npos)
		{
			objectName = filepath.substr(spos + 1, std::string::npos);
		}
		objectName = filepath;

		// Ensure that the Resource base class is notified of the filename
		SetFilename(filepath);
	}

	Script::Script(const private_struct &, std::string scriptString, ScriptSource source) : NodeComponent("Script"),
																							scriptSource(source),
																							initialised(false),
																							compileError(false),
																							Resource("", ResourceType::Type::SCRIPT)
	{
        switch(scriptSource)
		{
			case ScriptSource::SS_FILE:
            {
				// strip the path and just use the filename
				unsigned spos = scriptString.rfind("/");
				if (spos != std::string::npos)
				{
					objectName = scriptString.substr(spos + 1, std::string::npos);
				}
				objectName = scriptString;

				// Ensure that the Resource base class is notified of the filename
				SetFilename(scriptString);

				text = "";
				break;
            }
			case ScriptSource::SS_TEXT:
            {
				// Name the script using its Object ID
				std::stringstream ss;
				ss << id;
				objectName = std::string("Script_") + ss.str();
				text = scriptString;
				break;
            }
            default:
                break;
		}
	}
	
	Script::~Script(void)
	{
	}

	void Script::HandlePythonError()
	{
		if (PyErr_Occurred())
		{
			compileError = true;
			PrintPythonError();
		}

		PyErr_Clear();
	}

	bool Script::InitScript()
	{
		// If the Script is a text buffer
		// or if the script is a valid file
		// initialise it
		if (scriptSource == ScriptSource::SS_TEXT || (scriptSource == ScriptSource::SS_FILE && IsValidFile()) )
		{
			try
			{
				// Build a local namespace for this script, 
				// so scripts don't stomp over each others global namespace
				scriptLocalNamespace["__builtins__"] = import("__builtin__");
				scriptLocalNamespace["epsilon"] = import("epsilon");

				// TODO: Add better exception handling here
				// Load the python source into the local namespace.
				switch (scriptSource)
				{
				case ScriptSource::SS_FILE:
					exec_file(GetFilepath().GetString().c_str(), scriptLocalNamespace);
					break;
				case ScriptSource::SS_TEXT:
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

				// Register Hotloading callbacks
				beforeReloadFunction = FindPythonFunction("before_reload");
				afterReloadFunction = FindPythonFunction("after_reload");

				// For Hotloading.
				if (this->componentParent)
				{
					OnSetParent();
				}

				// Set the scripts state to a good state now that it has initialised
				initialised = true;
				compileError = false;
			}
			catch (const error_already_set&)
			{
				HandlePythonError();
			}
		}

		// Indicate that the Script Resource has been refreshed
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
			bool existingCompileState = compileError;
			// We have to wrap this in a try/catch because python will throw
			// an AttributeException if functions are not defined.
			
			try
			{
				if (PyObject_HasAttrString(behaviourClass.ptr(), funcName.c_str()))
				{
					targetFunction = behaviourClass.attr(funcName.c_str());
				}
			}
			catch (const error_already_set&)
			{
				// Don't display error for functions that we're trying to hook that don't exist.
				// It's not an error.
				PyErr_Clear();
			}

			// also make sure that the compile error state is reset here as well.
			compileError = existingCompileState;
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
				case ScriptSource::SS_FILE:
					scriptName = GetFilepath().GetString();
					break;
				case ScriptSource::SS_TEXT:
					scriptName = ( text.size() > 10 ) ? text.substr(0, 10) : text;
					break;
				default:
					break;
			}
//            Log(boost::str(format("%s method not found in script: %s") % funcName.c_str() % scriptName.c_str()));
            
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

	object Script::GetScriptObject()
	{
		return behaviourClass;
	}

	std::string Script::GetClassname()
	{
		if (!behaviourClass.is_none())
		{
			return extract<std::string>(behaviourClass.attr("__dict__").attr("__class__").attr("__name__"));
		}
		else
		{
			return std::string("None");
		}
	}
/*
	Script::Ptr Script::SetScriptFile(std::string scriptFilename)
	{
		// Ensuring that the path to the script is absolute
        filename = ResourceManager::GetInstance().GetResourceFullPath(scriptFilename);
        
		// Clear text in preparation for next init
		text = "";
		scriptSource = ScriptSource::SS_FILE;
		return ThisPtr();
	}
*/
	Script::Ptr Script::SetScriptText(std::string scriptText)
	{
		text = scriptText;
		scriptSource = ScriptSource::SS_TEXT;
		return ThisPtr();
	}

	void Script::BeforeReload()
	{
		object result;
		if (!beforeReloadFunction.is_none())
		{
			try
			{
				result = beforeReloadFunction();
			}
			catch (const error_already_set&)
			{
				HandlePythonError();
			}
		}
	}

	void Script::AfterReload()
	{
		object result;
		if (!afterReloadFunction.is_none())
		{
			try
			{
				result = afterReloadFunction();
			}
			catch (const error_already_set&)
			{
				HandlePythonError();
			}
		}
	}
}