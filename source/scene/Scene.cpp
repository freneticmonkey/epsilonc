#include "scene/Scene.h"

namespace epsilon
{
	Scene::Ptr Scene::Create(std::string name)
	{
		Scene::Ptr newScene = std::make_shared<Scene>(private_struct(), name);
		newScene->Setup();
		return newScene;
	}

	Scene::Scene(const private_struct &, std::string sceneName) : name(sceneName)
	{   
	}

	Scene::~Scene(void)
	{
		if ( rootNode != NULL )
		{
			rootNode->OnDestroy();
		}
	}

	// Initialise the default values of the scene
	void Scene::Setup()
	{
		// Create a root node for the scene
		rootNode = SceneNode::Create("root");

		// Set the this scene as the current scene
		rootNode->SetScene(ThisPtr());

		// Get the new node's transform as the scene root transform
		rootTransform = rootNode->GetTransform();

		// Attach a node with a camera as the default scene camera
		SceneNode::Ptr camNode = rootNode->CreateChild("camera_node");
		
		// Set the new camera as the default / active camera
		SetActiveCamera(camNode->CreateCamera("main_camera"));

		// Move default camera to a suitable position
		camNode->transform->SetPosition(0, 1, -10);

		//camNode->transform->LookAt(Vector3(0, 1, 0));
	}

	bool Scene::operator==(Scene::Ptr other)
	{
		return name == other->name;
	}

	bool Scene::operator==(std::string otherName)
	{
		return name == otherName;
	}

	void Scene::Update(float el)
	{
		rootTransform->_update(true, false);
	}

	void Scene::SetActiveCamera(Camera::Ptr camera)
	{
		// Add the camera
		AddCamera(camera);
		
		// Set it as the active camera
		activeCamera = camera;

		std::for_each(sceneCameras.begin(), sceneCameras.end(), [](Camera::Ptr camera){
			camera->SetActive(false);
		});

		camera->SetActive(true);
	}

	void Scene::SetActiveCamera(std::string name)
	{
		// Find the camera
		bool found = false;

		CameraList::iterator cam = std::find_if(sceneCameras.begin(), sceneCameras.end(), [name](Camera::Ptr camera) {
			return camera->GetName() == name;
		});

		// if a camera with name 'name' wasn't found
		if (cam == sceneCameras.end())
		{
			Log("Error setting active camera.  Unknown camera: " + name);
		}
		else
		{
			// Set the found camera as the active camera
			SetActiveCamera(*cam);
		}
	}

	bool Scene::AddCamera(Camera::Ptr newCamera)
	{
		// Find the camera
		bool added = false;

		CameraList::iterator it = std::find_if(sceneCameras.begin(), sceneCameras.end(), [&](Camera::Ptr cam){
			return cam->GetId() == newCamera->GetId();
		});

		// if a new camera
		if (it == sceneCameras.end())
		{
			// keep track of it
			sceneCameras.push_back(newCamera);
			added = true;
		}

		// Return the result of the add
		return added;
	}

	bool Scene::RemoveCamera(Camera::Ptr camera)
	{
		// Find the camera
		bool found = false;

		CameraList::iterator it = std::find_if(sceneCameras.begin(), sceneCameras.end(), [&](Camera::Ptr cam){
			return cam == camera;
		});

		if (it != sceneCameras.end())
		{
			sceneCameras.erase(it);
			found = true;
		}

		// Return the result
		return found;
	}

	bool Scene::RemoveCamera(std::string name)
	{
		// Find the camera
		bool found = false;

		CameraList::iterator it = std::find_if(sceneCameras.begin(), sceneCameras.end(), [&](Camera::Ptr cam){
			return cam->GetName() == name;
		});

		if (it != sceneCameras.end())
		{
			sceneCameras.erase(it);
			found = true;
		}

		// Return the result
		return found;
	}

	Camera::Ptr Scene::GetCamera(std::string name)
	{
		Camera::Ptr foundCam;
		
		CameraList::iterator it = std::find_if(sceneCameras.begin(), sceneCameras.end(), [&](Camera::Ptr cam){
			return cam->GetName() == name;
		});

		if (it != sceneCameras.end())
		{
			foundCam = (*it);
		}

		return foundCam;
	}

	bool Scene::AddLight(Light::Ptr light)
	{
		bool success = false;

		LightList::iterator it = std::find_if(sceneLights.begin(), sceneLights.end(), [&](Light::Ptr light){
			return light->GetName() == name;
		});

		if (it == sceneLights.end())
		{
			sceneLights.push_back(light);
			success = true;
		}

		return success;
	}

	bool Scene::RemoveLight(Light::Ptr light)
	{
		bool success = false;

		LightList::iterator it = std::find_if(sceneLights.begin(), sceneLights.end(), [&](Light::Ptr sLight){
			return light == sLight;
		});

		if (it == sceneLights.end())
		{
			sceneLights.erase(it);
			success = true;
		}

		return success;
	}

	bool Scene::RemoveLight(std::string name)
	{
		bool success = false;

		LightList::iterator it = std::find_if(sceneLights.begin(), sceneLights.end(), [&](Light::Ptr light){
			return light->GetName() == name;
		});

		if (it == sceneLights.end())
		{
			sceneLights.erase(it);
			success = true;
		}

		return success;
	}

	Light::Ptr Scene::GetLight(std::string name)
	{
		Light::Ptr foundLight;

		LightList::iterator it = std::find_if(sceneLights.begin(), sceneLights.end(), [&](Light::Ptr light){
			return light->GetName() == name;
		});

		if (it != sceneLights.end())
		{
			foundLight = *it;
		}

		return foundLight;
	}

}
