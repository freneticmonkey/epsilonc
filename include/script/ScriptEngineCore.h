#pragma once

#include "EpsilonCore.h"
#include "script/Script.h"

namespace epsilon
{
	// There is only ever going to be a single instance of this script running.
	// It wraps all of the Python side background activity that is separate from
	// scripts running in the scene.
	class ScriptEngineCore : 
		public Script
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<ScriptEngineCore> Ptr;

		static ScriptEngineCore::Ptr Create();
		
		explicit ScriptEngineCore(const private_struct &);
		explicit ScriptEngineCore(const private_struct &, std::string scriptString, ScriptSource scriptSource);
		~ScriptEngineCore(void);

		ScriptEngineCore::Ptr ThisPtr() { return static_pointer_cast<ScriptEngineCore,Script>(shared_from_this()); }

		void OnStart();
		void Update(float dt);
		void OnDestroy();

	protected:
		void RegisterScriptFunctions();
	private:

		object startFunction;
		object updateFunction;
		object destroyFunction;
	};

}