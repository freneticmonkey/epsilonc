#include "script/python/ManagerModule.h"
#include "events/EventManager.h"
#include "scene/SceneManager.h"
#include "script/ScriptManager.h"
#include "script/Script.h"

void initManagers()
{
	// No namespace business here. These are going straight into 
	// the global (epsilon) namespace

	// Inject the singletons straight into the root namespace to remove get_instance calls

	object package = scope();
	
	class_<EventManager, boost::noncopyable>("EventManager", no_init)
		.def("get_instance", &EventManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("add_listener", &EventManager::AddListener)
		.staticmethod("add_listener")

		.def("remove_listener", &EventManager::RemoveListener)
		.staticmethod("remove_listener")

		.def("fire_event", &EventManager::FireEvent)
		.staticmethod("fire_event")
	;
	
	object sceneManager = class_<SceneManager, boost::noncopyable>("SceneManager", no_init)
		.def("get_instance", &SceneManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		.def("set_scene", (void (SceneManager::*)(Scene::Ptr) )(&SceneManager::SetScene) )
		.def("set_scene", (void (SceneManager::*)(std::string) )(&SceneManager::SetScene) )

		.def_readonly("current_scene", &SceneManager::CurrentScene)
	;
	// Injecting the get instance result into the namespace
	object smGI = sceneManager.attr("get_instance");
	package.attr("SceneManager") = smGI();

	Script::Ptr (ScriptManager::*CreateScriptStandard)(void) = &ScriptManager::CreateScript;
	Script::Ptr (ScriptManager::*CreateScriptName)(std::string) = &ScriptManager::CreateScript;

	object scriptManager = class_<ScriptManager, boost::noncopyable>("ScriptManager", no_init)
		.def("get_instance", &ScriptManager::GetInstance, return_value_policy<reference_existing_object>())
		.staticmethod("get_instance")

		//.def("create_script", (Script::Ptr (ScriptManager::*)
		.def("create_script", CreateScriptStandard)
		.def("create_script", CreateScriptName )

		.def("create_behaviour", &ScriptManager::CreateBehaviour)

		.def("reload_script", &ScriptManager::ReloadScript)
	;
	// Injecting the get instance result into the namespace
	smGI = scriptManager.attr("get_instance");
	package.attr("ScriptManager") = smGI();

}