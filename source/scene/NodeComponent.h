#pragma once

#include <algorithm>

#include "scene/Object.h"
//#include "scene/Node.h"

#include "events/Event.h"

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
			// If the component is being removed
			if (this->componentParent != nullptr)
			{
				OnRemoveParent();
			}

			// If the component is being moved to a new node
			if (this->componentParent != parent)
			{
				OnChangeParent(this->componentParent, parent);
			}

			// Perform the parent action
			this->componentParent = parent;
			
			// If the component is set to a valid node, fire the parent event
			if (this->componentParent != nullptr)
			{
				OnSetParent();
			}
						
		}
		NodeComponent::Ptr GetParent() { return this->componentParent; }

		// Listen to parent set Event
		virtual void OnSetParent() {}

		// Listen to parent remove Event
		virtual void OnRemoveParent() {}

		// Listen to parent change Event
		virtual void OnChangeParent(NodeComponent::Ptr oldParent, NodeComponent::Ptr newParent) {}
        
        // Listen to Destroy
        virtual void OnDestroy() {}

		// Event Handling

		// Node Components will override this to handle events
		virtual void HandleEvent(EventDataBase::Ptr event) {}

		// Process event in child components
		void OnEvent(EventDataBase::Ptr event)
		{
			// Firstly send to myself
			HandleEvent(event);

			// Then send to my children (if I have any)
			if (components)
			{
				std::for_each(components->begin(), components->end(), [event](NodeComponent::Ptr component){
					component->OnEvent(event);
				});
			}
		}

		// Helper function for children to send events to other child NodeComponents.
		void AddParentEvent(EventDataBase::Ptr event)
		{
			if (this->componentParent != nullptr)
			{
				this->componentParent->OnEvent(event);
			}
		}

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