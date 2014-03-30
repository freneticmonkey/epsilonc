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
#include "resource/ResourceDependency.h"
#include "resource/ResourceOwnerInterface.h"
#include "resource/Resource.h"
#include "resource/ResourceType.h"

#include <boost/filesystem.hpp>

using namespace boost;


namespace epsilon
{
    enum ResourceError
    {
        UNKNOWN_RESOURCE = 1,
        // So on and so forth
    };

	/**
	 * ResourceManager
	 * 
	 * This class maintains a list of resources available to be used by
	 * the engine.  It scans the defined resource folders for files and tracks
	 * their state.  Resource Owners register the resources they are interested
	 * in and Resource Manager notifies them when the resource changes on disk.
	 */
    class ResourceManager
    {
		typedef std::map<long, ResourceOwnerInterface *>	ResourceOwners;
        typedef std::map<std::size_t, Resource::Ptr>		Resources;
		typedef std::map<std::size_t, ResourceIdVector>		ResourceDependencies;
		typedef std::vector<std::size_t>					CycleCheck;

		typedef std::map<long, ResourceIdVector>			ChangedResourceMap;
        
    private:
        ResourceManager(void);
        
    public:
        static ResourceManager & GetInstance()
        {
            static ResourceManager instance;
            return instance;
        }
        ~ResourceManager(void);

		void SetBasePath(std::string basepath);

		/*
		 * Build the initial map of resources in the resources folder
		 */
		void BuildResourceInfo();
        
		/*
		 * Add a new ResourceOwner
		 *
		 * ResourceOwners will be notified if any of the resources they own change.
		 */
		void AddResourceOwner(ResourceOwnerInterface *owner);

		/*
		 * Add a new Resource to the map of known resources.  If a Resource is new,
		 * it's just added to the map.  If a resource is already known, then the file
		 * information contained in the existing resource is copied to the new Resource.
		 * This implementation means that the ResourceManager knows about all Resources
		 * and ResourceOwners only have to know about the Resources they manage.
		 */
		void AddResource(Resource::Ptr newResource);
        
        void AddDependency(Resource::Ptr parentResource, Resource::Ptr childResource);
        void RemoveDependency(Resource::Ptr parentResource, Resource::Ptr childResource);
        
		void SetCheckFrequency(float checkFreq) { checkFrequency = checkFreq; }

		// Periodically checks the state of the files in the resource map
		void Update(float dt);

    private:
		// Internal function used to build the filemap
		void ParseDirectory(filesystem::path dir);
		
		// Internal function to check if resources have changed
		void CheckForResourceChanges();
		
		// Mark the resource and any of its dependents as changed
		void MarkResourceChanged(ChangedResourceMap &changedResources, CycleCheck &cycleCheck, std::size_t resourceId);

		long GetResourceId() { return resourceId++; }
		long				 resourceId;

		long GetOwnerId() { return ++ownerId; }
		long				 ownerId;

		ResourceType		 resourceTypes;

		ResourceOwners		 owners;
        Resources			 resources;
		ResourceDependencies dependencies;
		
		// The Frequency in seconds between file change checks
		float				checkFrequency;
		float				elSinceCheck;

		// The Resources basepath?
		std::string			basepath;
        
    };
}
