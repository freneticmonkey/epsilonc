#pragma once

#include "EpsilonCore.h"

#include <sstream>

#include "scene/Object.h"
#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "render/Renderer.h"

namespace epsilon
{
	class Node;

	typedef std::list< std::shared_ptr<Node> > NodeList;
	typedef std::shared_ptr<NodeList> NodeListPtr;
	
	class Node : 
		public std::enable_shared_from_this<Node>,
		public NodeComponent
	{
	protected:
		struct private_struct {};

	public:
		// Instantisation
		typedef std::shared_ptr<Node> Ptr;

		static Node::Ptr Create();
		static Node::Ptr Create(std::string name);

		void Destroy();

		explicit Node(const private_struct &);
		Node(const private_struct &, std::string name);
		~Node(void);

		// Components
		Node::Ptr AddComponent(NodeComponent::Ptr newComponent);
		Node::Ptr RemoveComponent(NodeComponent::Ptr rmComponent);

		// Operators
		bool operator==(Node::Ptr other);
		bool operator==(long otherId);
		bool operator==(std::string name);

		// CreateChildNode
		Node::Ptr CreateChildNode();
		Node::Ptr AddChild(Node::Ptr newChild);

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