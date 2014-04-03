#pragma once

#include <algorithm>

#include "scene/Object.h"
//#include "scene/Node.h"

//#define USE_NODE 1

namespace epsilon
{
	class NodeComponent;
	
	typedef std::list< std::shared_ptr<NodeComponent> > NodeComponentList;
	typedef std::shared_ptr<NodeComponentList> NodeComponentListPtr;

	class NodeComponent : public Object
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<NodeComponent> Ptr;
		
		NodeComponent(std::string className) : Object(className), resourceOwner(0) {};
		NodeComponent(std::string name, std::string newClassName ) : Object(name, newClassName), resourceOwner(0) {};
		virtual ~NodeComponent(void) {};
        
		void SetParent(NodeComponent::Ptr parent) 
		{ 
			this->componentParent = parent; 
			OnSetParent();
		}
		NodeComponent::Ptr GetParent() { return this->componentParent; }

		// Listen to parent set Event
		virtual void OnSetParent() {}
        
        // Listen to Destroy
        virtual void OnDestroy() {}

		template<class C>
		std::shared_ptr<C> GetComponent()
		{
			std::shared_ptr<C> foundComponent;
					
			// Assign the new transform to the Node's NodeComponents		
			for ( NodeComponentList::iterator component = components->begin(); component != components->end(); component++ )
			{
				foundComponent = std::dynamic_pointer_cast<C>( *component );
				if ( foundComponent != nullptr )
				{
					break;
				}
			}
			return foundComponent;
		}

		NodeComponent::Ptr GetComponent(std::string name)
		{
			NodeComponent::Ptr foundComponent;
					
			// Assign the new transform to the Node's NodeComponents		
			for ( NodeComponentList::iterator component = components->begin(); component != components->end(); component++ )
			{
				if ( ( (*component)->GetName() == name ) || ( (*component)->GetClass() == name ) )
				{
					foundComponent = (*component);
					break;
				}
			}
			return foundComponent;
		}
        
        // Handle the toggling of components
        void OnEnable()
        {
            std::for_each(components->begin(), components->end(), [](NodeComponent::Ptr component){
                component->Enable();
            });
        }
        
        void OnDisable()
        {
            std::for_each(components->begin(), components->end(), [](NodeComponent::Ptr component){
                component->Disable();
            });
        }
        
		// For Resource Management
		void SetResourceOwner(long ownerId) { resourceOwner = ownerId; }
        long GetResourceOwner() { return resourceOwner; }
        
	protected:
        
		NodeComponent::Ptr componentParent;
			
		NodeComponentListPtr components;
        
        // The resource that owns this object!
        long        resourceOwner;
	};

}