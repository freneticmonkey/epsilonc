#pragma once

#include "EpsilonCore.h"

#include "script/Script.h"
#include "script/ScriptBehaviour.h"
#include "script/ScriptEngineCore.h"
#include "script/ScriptCommon.h"
#include "script/PythonLogListener.h"

#include "script/python/MainModule.h"

#include <Python.h>

using namespace boost::python;

namespace epsilon
{
	class ScriptManager
	{
		typedef std::vector<Script::Ptr> ScriptList;
		typedef std::vector<ScriptBehaviour::Ptr> BehaviourList;

		ScriptManager(void);
	public:
		static ScriptManager & GetInstance()
		{
			static ScriptManager instance;
			return instance;
		};

		~ScriptManager(void);

		Script::Ptr CreateScript();
		Script::Ptr CreateScript(std::string filename);

		ScriptBehaviour::Ptr CreateBehaviour(std::string filename);

		void StartEngineCore();

		void Setup(void);
		void Update(float el);
		
		void ReloadScript(Script::Ptr script);

	private:
		void StartBehaviours();

		// Multi-threading
		void LockGIL() { PyEval_RestoreThread(threadState); }
		void ReleaseGIL() { threadState = PyEval_SaveThread(); }
		PyThreadState * threadState;

		ScriptList scriptList;
		BehaviourList behaviourList;

		BehaviourList startingBehaviours;

		object pythonGlobalModule;
		dict pythonGlobalNamespace;

		ScriptEngineCore::Ptr engineCoreScript;

		object epsilonModule;

		std::string scriptsFolderPath;

		PythonLogListener stdErrListener;
		PythonLogListener stdOutListener;

		


	};
}

