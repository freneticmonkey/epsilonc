#pragma once

#include "scene/Node.h"
#include "render/Camera.h"

namespace epsilon
{
	using namespace std;

	class Scene :
		public enable_shared_from_this<Scene>
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Scene> Ptr;

		static Scene::Ptr Create();

		explicit Scene(const private_struct &);
		~Scene(void);
	
		string GetName() { return name; }

		Node::Ptr Root() { return rootNode; }
		Camera::Ptr GetActiveCamera() { return activeCamera; }

		bool operator==(Scene::Ptr other);
		bool operator==(string name);
	private:
		
		string name;
		Node::Ptr rootNode;
		Camera::Ptr activeCamera;
	};

}