#include "script/python/ManagerModule.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "script/Script.h"

void initManagers()
{
	// No namespace business here. These are going straight into 
	// the global (epsilon) namespace
	
	class_<SceneManager, boost::noncopyable>("SceneManager", no_init)
		.def("get_instance", &SceneManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("set_scene", (void (SceneManager::*)(Scene::Ptr) )(&SceneManager::SetScene) )
		.def("set_scene", (void (SceneManager::*)(std::string) )(&SceneManager::SetScene) )

		.def("current_scene", &SceneManager::CurrentScene)
	;

	Script::Ptr (ScriptManager::*CreateScriptStandard)(void) = &ScriptManager::CreateScript;
	Script::Ptr (ScriptManager::*CreateScriptName)(std::string) = &ScriptManager::CreateScript;

	class_<ScriptManager, boost::noncopyable>("ScriptManager", no_init)
		.def("get_instance", &ScriptManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		//.def("create_script", (Script::Ptr (ScriptManager::*)
		.def("create_script", CreateScriptStandard)
		.def("create_script", CreateScriptName )

		.def("create_behaviour", &ScriptManager::CreateBehaviour)

		.def("reload_script", &ScriptManager::ReloadScript)
	;

}