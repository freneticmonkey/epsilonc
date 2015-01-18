#include "script/ScriptBehaviour.h"
#include "script/ScriptCommon.h"

// Events
#include "physics/PhysicsEvents.h"

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

	ScriptBehaviour::Ptr ScriptBehaviour::CreateFromFile(std::string scriptString)
	{
		return std::make_shared<ScriptBehaviour>(private_struct(), scriptString);
	}

	ScriptBehaviour::ScriptBehaviour(const private_struct &) : Script(Script::private_struct())
	{
	}

	ScriptBehaviour::ScriptBehaviour(const private_struct &, std::string scriptString, ScriptSource source) : Script(Script::private_struct(), scriptString, source)
	{
	}

	ScriptBehaviour::ScriptBehaviour(const private_struct &, std::string filename) : Script(Script::private_struct(), filename)
	{
	}
	
	ScriptBehaviour::~ScriptBehaviour(void)
	{
	}

	void ScriptBehaviour::RegisterScriptFunctions()
	{
		startFunction = FindPythonFunction("on_start");
		updateFunction = FindPythonFunction("on_update");
		destroyFunction = FindPythonFunction("on_destroy");
		drawGizmosFunction = FindPythonFunction("on_draw_gizmos");
		guiFunction = FindPythonFunction("on_gui");

		// Check the script for known event functions and store them if found

		object evtFunction = FindPythonFunction("on_collision");

		if (!evtFunction.is_none())
		{
			eventFunctions[Collision::GetType()] = evtFunction;

			collFunction = evtFunction;
		}
		
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
				HandlePythonError();
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
				HandlePythonError();
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
				HandlePythonError();
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
				HandlePythonError();
			}
		}
	}

	void ScriptBehaviour::OnGUI()
	{
		object result;
		if (!guiFunction.is_none())
		{
			try
			{
				result = guiFunction();
			}
			catch (const error_already_set&)
			{
				HandlePythonError();
			}
		}
	}

	void ScriptBehaviour::HandleEvent(EventDataBase::Ptr event)
	{
		// Look for the event function
		EventFunctions::iterator func = eventFunctions.find(event->GetType());

		object result;
		try
		{
			// if a function is registered
			if (func != eventFunctions.end())
			{
				if (event->GetType() == Collision::GetType())
				{
					std::shared_ptr<Collision> col = std::dynamic_pointer_cast<Collision>(event);

					// call it passing the event data
					func->second(col);
				}
			}
		}
		catch (const error_already_set&)
		{
			HandlePythonError();
		}
	}
}