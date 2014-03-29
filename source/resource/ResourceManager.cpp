//
//  ResourceManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "ResourceManager.h"

namespace epsilon
{
    ResourceManager::ResourceManager(void)
    {
        
    }
    
    ResourceManager::~ResourceManager(void)
    {
        
    }
    
    void ResourceManager::AddResource(Resource::Ptr newResource)
    {
        // At this point we need to ensure that the resource is unique somehow.
        
        // Assign the global resource id
        newResource->SetId(currResourceId++);
        
    }
    
    void ResourceManager::AddDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
    {
        
    }
    
    void ResourceManager::RemoveDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
    {
        
    }
}

