#include "script/ScriptEngineCore.h"
#include "script/ScriptCommon.h"

namespace epsilon
{

	ScriptEngineCore::Ptr ScriptEngineCore::Create()
	{
		return std::make_shared<ScriptEngineCore>(private_struct());
	}

	ScriptEngineCore::ScriptEngineCore(const private_struct &) : Script(Script::private_struct(), 
																		std::string("resources/scripts/core/main.py"),
																		ScriptSource::FILE)
	{
	}

	ScriptEngineCore::~ScriptEngineCore(void)
	{
	}

	void ScriptEngineCore::RegisterScriptFunctions()
	{
		startFunction = FindPythonFunction("on_start");
		updateFunction = FindPythonFunction("on_update");
		destroyFunction = FindPythonFunction("on_destroy");
	}

	void ScriptEngineCore::OnStart()
	{
		object result;
		if ( !startFunction.is_none() )
		{
			try
			{
				result = startFunction();
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

	void ScriptEngineCore::Update(float dt)
	{
		object result;
		if ( !updateFunction.is_none() )
		{
			try
			{
				result = updateFunction(dt);
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

	void ScriptEngineCore::OnDestroy()
	{
		object result;
		if ( !destroyFunction.is_none() )
		{
			try
			{
				result = destroyFunction();
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