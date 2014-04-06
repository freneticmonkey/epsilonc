#pragma once

#include "scene/SceneBase.h"
#include "scene/SceneNode.h"
#include "render/Camera.h"
#include "render/Light.h"

namespace epsilon
{

	class SceneNode;

	class Scene :
		public SceneBase,
		public std::enable_shared_from_this<Scene>
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Scene> Ptr;
		typedef std::list<Camera::Ptr> CameraList;

		static Scene::Ptr Create(std::string name);

		explicit Scene(const private_struct &, std::string sceneName);
		~Scene(void);

		void Setup();
	
		std::string GetName() { return name; }

		SceneNode::Ptr Root() { return rootNode; }
		
		// Camera Handling
		Camera::Ptr GetActiveCamera() { return activeCamera; }
		void SetActiveCamera(Camera::Ptr camera);
		void SetActiveCamera(std::string name);

		bool AddCamera(Camera::Ptr newCamera);
		Camera::Ptr GetCamera(std::string name);

		bool AddLight(Light::Ptr newLight);
		Light::Ptr GetLight(std::string name);

		LightList GetLights() { return sceneLights; }

		bool operator==(Scene::Ptr other);
		bool operator==(std::string name);

		void Update(float el);
	private:
		Scene::Ptr ThisPtr() { return shared_from_this(); }
        
		std::string name;
		SceneNode::Ptr rootNode;
		Transform::Ptr rootTransform;
		Camera::Ptr activeCamera;
		CameraList sceneCameras;

		LightList sceneLights;
	};

}