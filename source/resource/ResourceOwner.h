#pragma once
//
//  ResourceOwner.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//

#include "resource/ResourceOwnerInterface.h"
#include "resource/ResourceManager.h"
#include "resource/Resource.h"

namespace epsilon
{
	class ResourceManager;

	// Derived classes will all be Singletons so, 
	// this class doesn't need to have shared pointers.
    class ResourceOwner : public ResourceOwnerInterface
    {
	public:
        
		ResourceOwner() : ResourceOwnerInterface()
        {
			ResourceManager::GetInstance().AddResourceOwner(this);
        }
        
		void RegisterResource(Resource::Ptr newResource)
		{
			// Set this object as the owner
			newResource->AddOwner(uID);

			// Send it to the ResourceManager
			ResourceManager::GetInstance().AddResource(newResource);
		}

		// Should be redefinted by derived classes
		virtual void RefreshResources(ResourceIdVector changedResources) {};
    };
}