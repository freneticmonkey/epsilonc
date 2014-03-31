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
	ResourceManager::ResourceManager(void) :resourceId(0),
											ownerId(0),
											checkFrequency(1.0f), 
											elSinceCheck(0.f),
											// hardcoded to resources for now.
											basepath("resources")
    {        
    }
    
    ResourceManager::~ResourceManager(void)
    {        
    }

	void ResourceManager::SetBasePath(std::string newBasepath)
	{
		basepath = newBasepath;
	}

	void ResourceManager::BuildResourceInfo()
	{
		ParseDirectory(filesystem::path(basepath));
	}

	void ResourceManager::AddResourceOwner(ResourceOwnerInterface * owner)
	{
		owner->SetId(GetOwnerId());
		owners[owner->GetId()] = owner;
	}

	void ResourceManager::AddResource(Resource::Ptr newResource)
    {
		// Check that the resource has a filename.  If it doesn't the ResourceManager 
		// currently can't generate an id or check its state, so it isn't handled
		if (newResource->GetFilepath().GetString().length() > 0)
		{
			// Use the filepath hash as the Resource id.
			if (resources.find(newResource->GetResourceId()) == resources.end())
			{
				// If the resource isn't already known to the map, add it
				resources[newResource->GetResourceId()] = newResource;
			}
			else
			{
				// The Resource is known to the list.
				// Set the new resources modified time to the correct time
				Resource::Ptr existingResource = resources[newResource->GetResourceId()];
				newResource->SetModifiedTime(existingResource->GetModifiedTime());

				// Replace existing resource with the new resource in the resource map
				resources[newResource->GetResourceId()] = newResource;

				Log("ResourceManager", "Registering Resource from Manaager: " + newResource->GetFilepath().GetString());
			}
		}
    }
    
    void ResourceManager::AddDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
    {
		dependencies[parentResource->GetResourceId()].push_back(childResource->GetResourceId());
    }
    
    void ResourceManager::RemoveDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
    {
		// Remove Id of the child resource from the parent resource's dependency list
		ResourceIdVector * resVec = &dependencies[parentResource->GetResourceId()];

		resVec->erase(std::remove(resVec->begin(), resVec->end(), childResource->GetResourceId()), resVec->end());
    }

	void ResourceManager::Update(float dt)
	{
		// Check file changes here
		elSinceCheck += dt;
		if (elSinceCheck >= checkFrequency)
		{
			// Run a check
			CheckForResourceChanges();

			// Reset the wait period :)
			elSinceCheck = 0.f;
		}
	}

	void ResourceManager::ParseDirectory(filesystem::path dir)
	{
		filesystem::directory_iterator dir_end;
		filesystem::directory_iterator dir_itr(dir);
		for (dir_itr; dir_itr != dir_end; ++dir_itr)
		{
			try
			{
				if (filesystem::is_directory(dir_itr->status()))
				{
					// Continue processing into the folder 
					ParseDirectory(dir_itr->path());
				}
				else if (filesystem::is_regular_file(dir_itr->status()))
				{
					// Add to known resources

					// Get the type of Resource from its file extension
					ResourceType::Type type = resourceTypes.GetResourceTypeByExt(dir_itr->path().extension().string());

					// Create a new resource
					Resource::Ptr newResource = Resource::Create(dir_itr->path().string(), type);
					// Set the file's modified time
					newResource->SetModifiedTime(filesystem::last_write_time(dir_itr->path()));
					// Add it to the manager
					AddResource(newResource);

					// Display debug
					Log("ResourceManager", "Found Resource: " + std::string(dir_itr->path().string()));
				}
			}
			catch (const std::exception & ex)
			{
				Log("ResourceManager", "Error: Parsing filesystem: " + std::string(dir_itr->path().string()) + ": " + ex.what());
			}
		}
	}

	void ResourceManager::CheckForResourceChanges()
	{
		ChangedResourceMap changedResources;
		CycleCheck cycleCheck;

		// Check for changed Resources
		//for_each(resources.begin(), resources.end(), [&changedResources](std::pair<long, Resource::Ptr>& res){
		for_each(resources.begin(), resources.end(), [&](Resources::value_type & item){

			Resource::Ptr resource = item.second;

			// Create path
			filesystem::path p(resource->GetFilepath().GetString());

			// Get the file's modification time
			std::time_t modifiedTime = filesystem::last_write_time(p);

			// If the resource has been loaded, and it has changed since load, 
			// mark it as reloaded
			if ((resource->NeedReload()) && (resource->GetModifiedTime() != modifiedTime))
			{
				// Set the new modified time
				resource->SetModifiedTime(modifiedTime);

				// Add the resource to the changed resources list along with any resource 
				// dependencies
				MarkResourceChanged(changedResources, cycleCheck, resource->GetResourceId());
			}
		});

		// Notify the ResourceOwners about the changed resources
		for_each(changedResources.begin(), changedResources.end(), [&](ChangedResourceMap::value_type & resourceGroup){
			owners[resourceGroup.first]->RefreshResources(resourceGroup.second);
		});
	}

	void ResourceManager::MarkResourceChanged(ChangedResourceMap &changedResources, CycleCheck &cycleCheck, std::size_t resourceId)
	{
		// If the Resource is not already in the CycleCheck vector
		if (std::find(cycleCheck.begin(), cycleCheck.end(), resourceId) == cycleCheck.end())
		{
			// Add the current Resource to the cycle check list.
			cycleCheck.push_back(resourceId);

			// Add it to the map of changed Resources!
            std::for_each(resources[resourceId]->GetOwners().begin(), resources[resourceId]->GetOwners().end(), [&](long ownerId){
                    changedResources[ownerId].push_back(resourceId);
            });

			// Get the Resource's dependents.
			ResourceIdVector dependents = dependencies[resourceId];

			// Mark each of the dependents as changed
			for_each(dependents.begin(), dependents.end(), [&](std::size_t depResId){
				MarkResourceChanged(changedResources, cycleCheck, depResId);
			});
		}
	}


}

