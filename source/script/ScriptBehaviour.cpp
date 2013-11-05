#include "script/ScriptBehaviour.h"

namespace epsilon
{

	ScriptBehaviour::Ptr ScriptBehaviour::Create()
	{
		return make_shared<ScriptBehaviour>(private_struct());
	}

	ScriptBehaviour::Ptr ScriptBehaviour::Create(std::string scriptString, ScriptSource source)
	{
		return make_shared<ScriptBehaviour>(private_struct(), scriptString, source);
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

	void ScriptBehaviour::RegisterScriptFunctions(dict pythonNamespace)
	{
		startFunction = FindPythonFunction("start", pythonNamespace);
		updateFunction = FindPythonFunction("update", pythonNamespace);
		destroyFunction = FindPythonFunction("destroy", pythonNamespace);
		drawGizmosFunction = FindPythonFunction("draw_gizmos", pythonNamespace);
	}

	void ScriptBehaviour::OnStart()
	{
		object result;
		if ( !startFunction.is_none() )
		{
			result = startFunction();
		}
	}

	void ScriptBehaviour::Update(float dt)
	{
		object result;
		if ( !updateFunction.is_none() )
		{
			result = updateFunction(dt);
		}
	}

	void ScriptBehaviour::OnDestroy()
	{
		object result;
		if ( !destroyFunction.is_none() )
		{
			result = destroyFunction();
		}
	}

	void ScriptBehaviour::OnDrawGizmos()
	{
		object result;
		if ( !drawGizmosFunction.is_none() )
		{
			result = drawGizmosFunction();
		}
	}
}