#include "scene/Scene.h"

namespace epsilon
{
	Scene::Ptr Scene::Create()
	{
		return make_shared<Scene>(private_struct());
	}

	Scene::Scene(const private_struct &)
	{
		rootNode = Node::Create("root");
		// Create and attach the default camera
		activeCamera = Camera::Create("main_camera");
		rootNode->AddChild(activeCamera);
	}

	Scene::~Scene(void)
	{
		if ( rootNode != NULL )
		{
			rootNode->Destroy();
		}
	}

	bool Scene::operator==(Scene::Ptr other)
	{
		return name == other->name;
	}

	bool Scene::operator==(string otherName)
	{
		return name == otherName;
	}

}
