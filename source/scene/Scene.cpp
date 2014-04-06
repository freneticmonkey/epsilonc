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
		return !found;
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
