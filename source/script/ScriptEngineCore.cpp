#include "script/ScriptEngineCore.h"
#include "script/ScriptCommon.h"

namespace epsilon
{

	ScriptEngineCore::Ptr ScriptEngineCore::Create()
	{
		return std::make_shared<ScriptEngineCore>(private_struct());
	}

	ScriptEngineCore::ScriptEngineCore(const private_struct &) : Script(Script::private_struct(), 
																		std::string("resources/scripts/core/main.py"))
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

	bool ScriptEngineCore::OnStart()
	{
		bool success = false;
		object result;
		if ( !startFunction.is_none() )
		{
			try
			{
				result = startFunction();
				success = true;
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}
		return success;
	}

	bool ScriptEngineCore::Update(float dt)
	{
		bool success = false;
		object result;
		if ( !updateFunction.is_none() )
		{
			try
			{
				result = updateFunction(dt);
				success = true;
			}
			catch (const error_already_set&)
			{
				if (PyErr_Occurred()) 
				{
					PrintPythonError();
				}
			}
		}
		return success;
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