#pragma once
//
//  ResourceManager.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//
#include <map>
#include <vector>
#include <algorithm>

#include "EpsilonCore.h"
#include "resource/ResourceOwner.h"
#include "resource/Resource.h"

namespace epsilon
{
    enum ResourceError
    {
        UNKNOWN_RESOURCE = 1,
        // So on and so forth
    };
    
    class ResourceDependency
    {
        typedef std::vector<Resource::Ptr> Dependencies;
    public:
        ResourceDependency(Resource::Ptr res)
        {
            resource = res;
        }
        
        void AddDependency(Resource::Ptr res)
        {
            dependencies.push_back(res);
        }
        
        void RemoveDependency(Resource::Ptr res)
        {
            Dependencies::iterator pos = find(dependencies.begin(), dependencies.end(), res);
            if ( pos != dependencies.end() )
            {
                dependencies.erase(pos);
            }
        }
        
        bool IsDependent(Resource::Ptr res)
        {
            return find(dependencies.begin(), dependencies.end(), res) != dependencies.end();
        }
        
        bool IsDependent(long res)
        {
            return find_if(dependencies.begin(), dependencies.end(), [res](Resource::Ptr resource) {
                return resource->GetId() == res;
            }) != dependencies.end();
        }
        
    private:
        Resource::Ptr resource;
        Dependencies  dependencies;
        
    };
    
    class ResourceManager
    {
        //TODO: What structure to store things in?
        
        //std::pair<long, Resource::Ptr> ResourceLookup;
        //typedef std::map<ResourceLookup> resourceMap;
        typedef std::vector<Resource::Ptr> Resources;
        
        // TODO: Need to also initialise a dependency structure here as well.
        
    private:
        ResourceManager(void);
        
    public:
        static ResourceManager & GetInstance()
        {
            static ResourceManager instance;
            return instance;
        }
        ~ResourceManager(void);
        
        void AddResource(Resource::Ptr newResource);
        
        void AddDependency(Resource::Ptr parentResource, Resource::Ptr childResource);
        void RemoveDependency(Resource::Ptr parentResource, Resource::Ptr childResource);
        
    private:
        Resources      resources;
        
        long currResourceId;
        
    };
}
