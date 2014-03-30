#pragma once

#include "EpsilonCore.h"
#include "script/Script.h"

namespace epsilon
{
	
	class ScriptBehaviour : 
		public Script
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<ScriptBehaviour> Ptr;

		static ScriptBehaviour::Ptr Create();
		static ScriptBehaviour::Ptr Create(std::string scriptString, ScriptSource source = ScriptSource::TEXT);
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

	protected:
		void RegisterScriptFunctions();
	private:

		object startFunction;
		object updateFunction;
		object destroyFunction;
		object drawGizmosFunction;
	};

}
