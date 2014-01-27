#pragma once

#include "scene/Node.h"
#include "render/Camera.h"

namespace epsilon
{
	class Scene :
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
	
		std::string GetName() { return name; }

		Node::Ptr Root() { return rootNode; }
		
		// Camera Handling
		Camera::Ptr GetActiveCamera() { return activeCamera; }
		void SetActiveCamera(Camera::Ptr camera);
		void SetActiveCamera(std::string name);

		bool AddCamera(Camera::Ptr newCamera);
		Camera::Ptr GetCamera(std::string name);

		bool operator==(Scene::Ptr other);
		bool operator==(std::string name);
	private:
		
		std::string name;
		Node::Ptr rootNode;
		Camera::Ptr activeCamera;
		CameraList sceneCameras;
	};

}