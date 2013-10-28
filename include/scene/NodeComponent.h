#pragma once

#include "scene/Object.h"
//#include "scene/Node.h"

//#define USE_NODE 1

namespace epsilon
{
	using namespace std;
	class NodeComponent;
	
	typedef list< shared_ptr<NodeComponent> > NodeComponentList;
	typedef shared_ptr<NodeComponentList> NodeComponentListPtr;

	class NodeComponent : public Object
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<NodeComponent> Ptr;
		
		NodeComponent(string className) : Object(className) {};
		NodeComponent(string name, string newClassName ) : Object(name, newClassName) {};
		virtual ~NodeComponent(void) {};

		void SetParent(NodeComponent::Ptr parent) { this->componentParent = parent; }
		NodeComponent::Ptr GetParent() { return this->componentParent; }

		template<class C>
		shared_ptr<C> GetComponent()
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
		
		// Component Fields
		//NodeComponent::Ptr GetTransform() { return transform; }
		//NodeComponent::Ptr GetRenderer() { return renderer; }

	protected:

		NodeComponent::Ptr componentParent;
			
		NodeComponentListPtr components;

		//NodeComponent::Ptr transform;
		//NodeComponent::Ptr renderer;
	};

}