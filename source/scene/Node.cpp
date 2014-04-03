#include "scene/Node.h"

namespace epsilon
{
	Node::Ptr Node::Create()
	{
		Node::Ptr newNode = std::make_shared<Node>(private_struct());
		return newNode;
	}

	Node::Ptr Node::Create(std::string name)
	{
		Node::Ptr newNode = std::make_shared<Node>(private_struct(), name);
		return newNode;
	}

	Node::Node(const private_struct &) : NodeComponent("Node")
	{
		components = std::make_shared<NodeComponentList>();
	}

	Node::Node(const private_struct &, std::string name) : NodeComponent(name, "Node")
	{
		components = std::make_shared<NodeComponentList>();
	}

	Node::~Node(void)
	{
	}

	void Node::OnDestroy()
	{
        // Propagate destroy event to all node components attached to this node.
        std::for_each(components->begin(), components->end(), [](NodeComponent::Ptr component){
            component->OnDestroy();
        });
        
        // Remove components from this node.
		RemoveAllComponents();
	}

	Node::Ptr Node::AddComponent(NodeComponent::Ptr newComponent)
	{
		NodeComponentList::iterator foundComponent;

		// If the component doesn't already exist in the list of components
		foundComponent = find_if(components->begin(), components->end(), [newComponent](NodeComponent::Ptr comp){ 
			return comp == newComponent; 
		});

		// if the end of the list was found, then the component is new
		if ( foundComponent == components->end() )
		{
			
			newComponent->SetParent(ThisPtr());

			components->push_back(newComponent);

		}
		return ThisPtr();
	}

	Node::Ptr Node::RemoveComponent(NodeComponent::Ptr removeComponent)
	{
		removeComponent->SetParent(nullptr);
		components->remove(removeComponent);
		return ThisPtr();
	}

	void Node::RemoveAllComponents()
	{
		for ( NodeComponentList::iterator component = components->begin(); component != components->end(); component++ )
		{
			if ( *component != nullptr )
			{
				(*component)->SetParent(nullptr);
			}
		}
		components->clear();
	}

	bool Node::operator==(Node::Ptr other)
	{
		return id == other->id;
	}

	bool Node::operator==(long otherId)
	{
		return id == otherId;
	}

	bool Node::operator==(std::string name)
	{
		return objectName == name;
	}
}