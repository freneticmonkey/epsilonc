#include "script/ScriptBehaviour.h"
#include "script/ScriptCommon.h"

namespace epsilon
{

	ScriptBehaviour::Ptr ScriptBehaviour::Create()
	{
		return std::make_shared<ScriptBehaviour>(private_struct());
	}

	ScriptBehaviour::Ptr ScriptBehaviour::Create(std::string scriptString, ScriptSource source)
	{
		return std::make_shared<ScriptBehaviour>(private_struct(), scriptString, source);
	}

	ScriptBehaviour::ScriptBehaviour(const private_struct &) : Script(Script::private_struct())
	{
	}

	ScriptBehaviour::ScriptBehaviour(const private_struct &, std::string scriptString, ScriptSource source) : Script(Script::private_struct(), scriptString, source)
	{
	}
	
	ScriptBehaviour::~ScriptBehaviour(void)
	{
	}

	void ScriptBehaviour::RegisterScriptFunctions()
	{
		startFunction = FindPythonFunction("on_start");
		updateFunction = FindPythonFunction("on_update");
		//destroyFunction = FindPythonFunction("on_destroy");
		//drawGizmosFunction = FindPythonFunction("on_draw_gizmos");
	}

	void ScriptBehaviour::OnStart()
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

	void ScriptBehaviour::Update(float dt)
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

	void ScriptBehaviour::OnDestroy()
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

	void ScriptBehaviour::OnDrawGizmos()
	{
		object result;
		if ( !drawGizmosFunction.is_none() )
		{
			try
			{
				result = drawGizmosFunction();
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