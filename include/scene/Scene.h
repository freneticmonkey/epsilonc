#pragma once

#include "scene/Node.h"
#include "render/Camera.h"

namespace epsilon
{
	using namespace std;

	class Scene
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Scene> Ptr;

		explicit Scene(const private_struct &);
		~Scene(void);
		static Ptr Create();
	
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