#pragma once

#include "EpsilonCore.h"

#include "resource/ResourceOwner.h"

#include "script/Script.h"
#include "script/ScriptBehaviour.h"
#include "script/ScriptEngineCore.h"
#include "script/ScriptCommon.h"
#include "script/PythonLogListener.h"

#include "script/python/MainModule.h"

#include <atomic>

using namespace boost::python;

namespace epsilon
{
	class ScriptManager : public ResourceOwner
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

		// Register an externally created behaviour with the ScriptManager, and in 
		// turn the ResourceManager
		void AddBehaviour(ScriptBehaviour::Ptr newBehaviour);

		void SetScriptsFolderPath(std::string scriptPath);
		void SetCoreScript(std::string coreScript);

		bool StartEngineCore();

		void Setup(void);
		void Update(float el);
		void Destroy(void);

		void BehaviourOnDrawGizmos();
		void BehaviourOnGUI();

		void ReloadScript(Script::Ptr script);

		bool DestroyScript(Script::Ptr script);

		// Is a script currently being processed.
		bool IsActive() { return activeResourceId != -1; }

		// Return the ResourceId of the Script currently being processed (if any)
		bool GetActiveResource() { return activeResourceId; }

		void RefreshResources(ResourceIdVector changedResources);

		void ProcessChangedResources();

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
        
    private:
        
		int gilLockCount;
		PyThreadState * threadState;

        void StartBehaviours();
        
		ScriptList scriptList;
		BehaviourList behaviourList;

		BehaviourList startingBehaviours;

		object pythonGlobalModule;
		dict pythonGlobalNamespace;

		ScriptEngineCore::Ptr engineCoreScript;

		object epsilonModule;

		std::string scriptsFolderPath;
		std::string coreScriptFilepath;

		PythonLogListener stdErrListener;
		PythonLogListener stdOutListener;

		ResourceIdVector  resourceChangedQueue[2];
		std::atomic<int>  currentChangedQueue;

		// The active script.
		long activeResourceId;

		


	};
}

