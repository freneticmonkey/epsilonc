#pragma once

#include "EpsilonCore.h"
#include "script/Script.h"
#include "events/Event.h"

namespace epsilon
{
	class ScriptBehaviour : 
		public Script
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<ScriptBehaviour> Ptr;
		typedef std::map<std::size_t, object>	 EventFunctions;

		static ScriptBehaviour::Ptr Create();
		static ScriptBehaviour::Ptr Create(std::string scriptString, ScriptSource source = ScriptSource::SS_TEXT);
		static ScriptBehaviour::Ptr CreateFromFile(std::string scriptString);
		
		explicit ScriptBehaviour(const private_struct &);
		explicit ScriptBehaviour(const private_struct &, std::string scriptString, ScriptSource scriptSource);
		explicit ScriptBehaviour(const private_struct &, std::string filename);
		~ScriptBehaviour(void);

		ScriptBehaviour::Ptr ThisPtr() { return std::static_pointer_cast<ScriptBehaviour,Script>(shared_from_this()); }

		void OnStart();
		void Update(float dt);
		void OnDestroy();
		void OnDrawGizmos();
		void OnGUI();

		void HandleEvent(EventDataBase::Ptr event);

	protected:
		void RegisterScriptFunctions();
	private:

		object startFunction;
		object updateFunction;
		object destroyFunction;
		object drawGizmosFunction;
		object guiFunction;

		// Event Functions
		EventFunctions eventFunctions;

		object collFunction;
	};

	// List define.
	typedef std::vector<ScriptBehaviour::Ptr> ScriptBehaviourList;

}
