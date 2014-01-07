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

		static Scene::Ptr Create();

		explicit Scene(const private_struct &);
		~Scene(void);
	
		std::string GetName() { return name; }

		Node::Ptr Root() { return rootNode; }
		Camera::Ptr GetActiveCamera() { return activeCamera; }

		bool operator==(Scene::Ptr other);
		bool operator==(std::string name);
	private:
		
		std::string name;
		Node::Ptr rootNode;
		Camera::Ptr activeCamera;
	};

}