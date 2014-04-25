//
//  ResourceManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//
#include <regex>

#include "ResourceManager.h"
#include "utilities/Utilities.h"

#include <boost/format.hpp>

using namespace epsilon;

namespace epsilon
{
	ResourceManager::ResourceManager(void) :resourceId(0),
											ownerId(0),
											checkFrequency(1.0f), 
											elSinceCheck(0.f),
											// hardcoded to resources for now.
											basepath("")
    {
		basepath = filesystem::initial_path().generic_string() + "/resources";
    }
    
    ResourceManager::~ResourceManager(void)
    {        
    }

	void ResourceManager::SetBasePath(std::string newBasepath)
	{
        // Ensure that the new base path is absolute
        filesystem::path bp(newBasepath);
        if ( !bp.is_absolute() )
        {
            bp = filesystem::canonical(bp);
        }
		basepath = bp.generic_string();
        Log("ResourceManager", "Basepath: " + basepath);
	}
    
    std::string ResourceManager::GetResourceFullPath(std::string resourceRelativePath)
    {
        std::string fullpath;
        
        filesystem::path bp(basepath);
        filesystem::path rPath(resourceRelativePath);
        
        // If the path is relative and includes the resources folder, 
		// remove the resources folder from the path.
		if ((*rPath.begin()).generic_string() == "resources")
        {
            filesystem::path temp;
            
            for (filesystem::path::iterator it = (++rPath.begin()); it != rPath.end(); it++)
            {
                temp /= *it;
            }
            rPath = temp;
        }
        
        // Check if the path is already absolute
        if (rPath.has_root_directory())
        {
            // incase there is some relative wackiness going on.
            fullpath = filesystem::canonical(rPath).generic_string();
        }
        else
        {
        
            // Check if the path is a sub path under the resource folder.
            filesystem::path rFolder = bp / (*rPath.begin());
            if (filesystem::exists(rFolder))
            {
                // build the final path
                fullpath = filesystem::complete(bp / rPath).generic_string();
                Log("ResourceManager", "Resolved Resource Path: " + fullpath );
            }
            else
            {
                
                rPath = filesystem::complete(bp / rPath);
                Log("ResourceManager", "Error Resolving Resource Path. Path doesn't exist: " + rPath.generic_string());
                fullpath = "error";
            }
        }
        
        return fullpath;
        
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
            // Make the path to the resource absolute (if it isn't already)
            filesystem::path resPath(newResource->GetFilepath().GetString());
            if ( !resPath.is_absolute() )
            {
                newResource->filepath = HashedString(GetResourceFullPath(newResource->GetFilepath().GetString()));
            }
            
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
				newResource->SetFileSize(existingResource->GetFileSize());
				// Replace existing resource with the new resource in the resource map
				resources[newResource->GetResourceId()] = newResource;

				//Log("ResourceManager", "Registering Resource from Manager: " + newResource->GetFilepath().GetString());
			}
		}
    }
    
	void ResourceManager::AddDependency(long parentResourceId, long childResourceId)
	{
		dependencies[parentResourceId].push_back(childResourceId);
	}

    void ResourceManager::AddDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
    {
		dependencies[parentResource->GetResourceId()].push_back(childResource->GetResourceId());
    }
    
    void ResourceManager::RemoveDependency(long parentResourceId, long childResourceId)
    {
		// Remove Id of the child resource from the parent resource's dependency list
		ResourceIdVector * resVec = &dependencies[parentResourceId];

		resVec->erase(std::remove(resVec->begin(), resVec->end(), childResourceId), resVec->end());
    }

	void ResourceManager::RemoveDependency(Resource::Ptr parentResource, Resource::Ptr childResource)
	{
		// Remove Id of the child resource from the parent resource's dependency list
		ResourceIdVector * resVec = &dependencies[parentResource->GetResourceId()];

		resVec->erase(std::remove(resVec->begin(), resVec->end(), childResource->GetResourceId()), resVec->end());
	}

	ResourceList ResourceManager::FindResources(std::string searchExpression)
	{
		std::regex expression(searchExpression);
		ResourceList results;
		
		// Check the regular expression against each of the known resources
		std::for_each(resources.begin(), resources.end(), [&](std::pair<std::size_t, Resource::Ptr> resourcePair){

			// If the resource path matches the regular expression
			if (std::regex_match(resourcePair.second->GetFilepath().GetString(), expression))
			{
                //Log("Found matching resource: " + resourcePair.second->GetFilepath().GetString() );
				// Store it in the results
				results.push_back(resourcePair.second);
			}
		});

		return results;
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
			std::string resourcePath = dir_itr->path().generic_string();

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
					ResourceType::Type type = resourceTypes.GetResourceTypeByExt(dir_itr->path().extension().generic_string());

					// Create a new resource
					Resource::Ptr newResource = Resource::Create(resourcePath, type);
					// Set the file's modified time
					newResource->SetModifiedTime(filesystem::last_write_time(dir_itr->path()));
					// Set the file's size
					newResource->SetFileSize(filesystem::file_size(dir_itr->path()));
					
					// Add it to the manager
					AddResource(newResource);

					// Display debug
					//Log("ResourceManager", "Found Resource: " + resourcePath);
				}
			}
			catch (const std::exception & ex)
			{
				Log("ResourceManager", "Error: Parsing filesystem: " + resourcePath + ": " + ex.what());
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

			// Get the file's size
			uintmax_t fileSize = filesystem::file_size(p);

			// If the resource has been loaded, and it has changed since load, 
			// mark it as reloaded
			if ((!resource->NeedReload()) && 
				(resource->GetModifiedTime() != modifiedTime) || 
				(resource->GetFileSize() != fileSize) )
			{
				// Set the new modified time
				resource->SetModifiedTime(modifiedTime);

				// Set the new filesize
				resource->SetFileSize(fileSize);

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
			//Log("ResourceManager", "Change detected for resource: " + resources[resourceId]->GetFilepath().GetString());

			// Add the current Resource to the cycle check list.
			cycleCheck.push_back(resourceId);

			// Add it to the map of changed Resources!
			Resource::OwnerIds owners = resources[resourceId]->GetOwners();
			std::for_each(owners.begin(), owners.end(), [&](long ownerId){
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

