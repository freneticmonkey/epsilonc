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

		// Multi-threading Python GIL Handling
		// Using a count rather than a boolean to prevent
		// multiple mutex locks means that many functions 
		// within the class can call lock/release, and call each other
		// without stalling on a second lock.
		void LockGIL() 
		{ 
			// Use a GIL Lock Count to track calls to the gil lock
			// to ensure that the GIL is only locked once
			if ( gilLockCount == 0 )
			{
				PyEval_RestoreThread(threadState); 
			}
			gilLockCount++;
		}
		void ReleaseGIL() 
		{ 
			// If the GIL Lock is the final release, actually release
			// the GIL Lock.
			if ( gilLockCount == 1 )
			{
				threadState = PyEval_SaveThread();
			}

			// Decrement the GIL Lock
			gilLockCount--;
		}
		int gilLockCount;
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

