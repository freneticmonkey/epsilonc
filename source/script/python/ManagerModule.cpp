#include "script/python/ManagerModule.h"
#include "scene/SceneManager.h"

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

}