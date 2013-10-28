#pragma once

#include "EpsilonCore.h"

#include <sstream>

#include "scene/Object.h"
#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "render/Renderer.h"

namespace epsilon
{
	using namespace std;
	class Node;

	typedef list< shared_ptr<Node> > NodeList;
	typedef shared_ptr<NodeList> NodeListPtr;
	
	class Node : 
		public enable_shared_from_this<Node>,
		public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		// Instantisation
		typedef shared_ptr<Node> Ptr;

		static Node::Ptr Create();
		static Node::Ptr Create(string name);

		void Destroy();

		explicit Node(const private_struct &);
		Node(const private_struct &, string name);
		~Node(void);

		// Components
		Node& AddComponent(NodeComponent::Ptr newComponent);
		Node& RemoveComponent(NodeComponent::Ptr rmComponent);

		// Operators
		bool operator==(Node::Ptr other);
		bool operator==(long otherId);
		bool operator==(string name);

		// CreateChildNode
		Node::Ptr CreateChildNode();
		Node& AddChild(Node::Ptr newChild);

		// Node Getters
		//Transform::Ptr Transform() { return transform; }
		//Renderer::Ptr Renderer() { return renderer; }

		//template<class C> 
		//shared_ptr<C> GetComponent();

	protected:
		void CreateComponents();
		void RemoveAllComponents();

	private:
		Node::Ptr ThisPtr() { return shared_from_this(); }

		//NodeComponentListPtr components;

		//Transform::Ptr transform;
		//Renderer::Ptr renderer;
	};
}