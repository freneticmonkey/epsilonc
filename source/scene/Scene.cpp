#include "scene/Scene.h"

namespace epsilon
{
	Scene::Ptr Scene::Create(std::string name)
	{
		return std::make_shared<Scene>(private_struct(), name);
	}

	Scene::Scene(const private_struct &, std::string sceneName) : name(sceneName)
	{
		rootNode = Node::Create("root");
		// Create and attach the default camera
		activeCamera = Camera::Create("main_camera");
		sceneCameras.push_back(activeCamera);
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

	bool Scene::operator==(std::string otherName)
	{
		return name == otherName;
	}

	void Scene::SetActiveCamera(Camera::Ptr camera)
	{
		// Add the camera
		AddCamera(camera);
		
		// Set it as the active camera
		activeCamera = camera;
	}

	void Scene::SetActiveCamera(std::string name)
	{
		// Find the camera
		bool found = false;

		for ( CameraList::iterator cam = sceneCameras.begin(); cam != sceneCameras.end(); cam++ )
		{
			if ( (*cam)->GetName() == name )
			{
				// Set it as the active camera
				activeCamera = (*cam);
				found = true;
				break;
			}
		}

		// if a new camera
		if (!found)
		{
			Log(Format("Error setting active camera.  Unknown camera: %s", name.c_str()));
		}
	}

	bool Scene::AddCamera(Camera::Ptr newCamera)
	{
		// Find the camera
		bool found = false;

		for ( CameraList::iterator cam = sceneCameras.begin(); cam != sceneCameras.end(); cam++ )
		{
			if ( newCamera->GetId() == (*cam)->GetId() )
			{
				found = true;
				break;
			}
		}

		// if a new camera
		if (!found)
		{
			// keep track of it
			sceneCameras.push_back(newCamera);
		}
		// Return the result of the add
		return found;
	}

	Camera::Ptr Scene::GetCamera(std::string name)
	{
		Camera::Ptr foundCam;

		for ( CameraList::iterator cam = sceneCameras.begin(); cam != sceneCameras.end(); cam++ )
		{
			if ( (*cam)->GetName() == name )
			{
				// Set it as the active camera
				foundCam = (*cam);
				break;
			}
		}

		return foundCam;
	}

}
