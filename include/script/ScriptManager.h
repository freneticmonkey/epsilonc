#pragma once

#include "EpsilonCore.h"

#include "script/Script.h"
#include "script/ScriptBehaviour.h"
#include "script/ScriptCommon.h"

#include "script/python/Module.h"

using namespace boost::python;

namespace epsilon
{
	class ScriptManager
	{
		typedef std::vector<Script::Ptr> ScriptList;
		typedef std::vector<ScriptBehaviour::Ptr> BehaviourList;
	public:
		ScriptManager(void);
		~ScriptManager(void);

		Script::Ptr CreateScript();
		Script::Ptr CreateScript(std::string filename);

		ScriptBehaviour::Ptr CreateBehaviour(std::string filename);

		void Setup(void);
		void Update(float el);
		
		void ReloadScript(Script::Ptr script);

	private:
		ScriptList scriptList;
		BehaviourList behaviourList;

		object pythonGlobalModule;
		dict pythonGlobalNamespace;

		object epsilonModule;

		std::string scriptsFolderPath;
	};
}

