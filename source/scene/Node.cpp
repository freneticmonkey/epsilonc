#include "scene/Node.h"

namespace epsilon
{
	Node::Ptr Node::Create()
	{
		Node::Ptr newNode = make_shared<Node>(private_struct());
		newNode->CreateComponents();
		return newNode;
	}

	Node::Ptr Node::Create(string name)
	{
		Node::Ptr newNode = make_shared<Node>(private_struct(), name);
		newNode->CreateComponents();
		return newNode;
	}

	Node::Node(const private_struct &) : NodeComponent("Node")
	{
		components = make_shared<NodeComponentList>();
	}

	Node::Node(const private_struct &, string name) : NodeComponent(name, "Node")
	{
		components = make_shared<NodeComponentList>();
	}

	Node::~Node(void)
	{
	}

	void Node::Destroy()
	{
		// Destroy child Nodes
		/*
		TransformListPtr children = Transform()->GetChildren();
		for ( TransformList::iterator trans = children->begin(); trans != children->end(); trans++ ) 
		{
			//((Node *)(*trans)->node)->Destroy();
		}
		*/
		
		// Remove children from Transform
		//Transform()->Destroy();
		GetComponent<Transform>()->Destroy();

		// Remove components from this node.
		RemoveAllComponents();
	}

	void Node::CreateComponents()
	{
		//transform = Transform::Create();
		//AddComponent(transform);
		AddComponent(Transform::Create());

		//renderer = Renderer::Create();
		//AddComponent(renderer);
	}

	Node& Node::AddComponent(NodeComponent::Ptr newComponent)
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
			/*
			// Assign the new component to the appropriate Node property
			string componentName = newComponent->GetClass();
			if ( componentName == "Transform")
			{
				transform = newComponent;
			}
			else if (componentName == "Renderer")
			{
				renderer = newComponent;
			}
			*/

			components->push_back(newComponent);

		}
		return *this;
	}

	Node& Node::RemoveComponent(NodeComponent::Ptr removeComponent)
	{
		removeComponent->SetParent(nullptr);
		components->remove(removeComponent);
		return *this;
	}

	/*
	template<class C>
	shared_ptr<C> Node::GetComponent()
	{
		shared_ptr<C> foundComponent;
					
		// Assign the new transform to the Node's NodeComponents		
		for ( NodeComponentList::iterator component = components->begin(); component != components->end(); component++ )
		{
			foundComponent = dynamic_pointer_cast<C>( *component );
			if ( foundComponent != nullptr )
			{
				break;
			}
		}
		return foundComponent;
	}
	*/

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

	bool Node::operator==(string name)
	{
		return objectName == name;
	}

	Node::Ptr Node::CreateChildNode()
	{
		Node::Ptr newChild = Node::Create();

		// Attach new Node
		AddChild(newChild);
		return newChild;
	}

	Node& Node::AddChild(Node::Ptr newChild)
	{
		GetComponent<Transform>()->AddChild( newChild->GetComponent<Transform>() );
		return *this;
	}
}