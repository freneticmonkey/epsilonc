#pragma once
//
//  Resource.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//
#include "EpsilonCore.h"
#include "resource/ResourceType.h"
#include "utilities/HashedString.h"

namespace epsilon
{
    class ResourceManager;
 
	/*
	 * Resource
	 * 
	 * A Resource is a wrapper for a (at the moment) disk based file.
	 * The Resource class exposes the file state to the Resource Owners.
	 * This members of this class are managed by the ResourceManager
	 */
    class Resource
    {
        friend class ResourceManager;
     protected:
        struct private_struct {};
        
     public:
		typedef std::shared_ptr<Resource> Ptr;
        typedef std::vector<long> OwnerIds;
        
		static Resource::Ptr Create(std::string theFilePath, ResourceType::Type resType);
        
		explicit Resource(std::string theFilepath, ResourceType::Type iType = ResourceType::Type::TEXT);
		~Resource();
        
        int		GetType() { return type; }
        
        // Ensure that type based comparison is implemented?
//        virtual int operator=;
        
		long GetReourceId() { return filepath.GetHash();  }

		const OwnerIds & GetOwners() { return ownerIds; }
		void AddOwner(long owner);
        void RemoveOwner(long owner);
        bool IsOwner(long owner);
		
		// The Filepath cannot be changed as its hashed value is the id for the resource!
		HashedString GetFilepath() { return filepath;  }
		
		// Get the Id of the resource from the hash of its filepath.
		std::size_t GetResourceId() { return filepath.GetHash(); }

		// Get the modified time of the resource when it was last reloaded
		long GetLastModified(){ return lastModified; }
		
		// Get the current modified time of the resource
		long GetModifiedTime() { return modifiedTime; }

		// Set the modified time of the resource - Intended for use by the Resource Manager
		void SetModifiedTime(long newModifiedTime);

		// Indicate that the in-memory resource is out of date
		bool NeedReload() { return lastModified == modifiedTime; }

		// Notify the resource that the in-memory copy is up to date.
		void SetReloaded() { lastModified = modifiedTime; }
        
		// TODO!: Add file reading writing functionality in here. :)

    private:
        OwnerIds		ownerIds;
        
        int				type;

		HashedString	filepath;
		// The modified time on last resource load.
		long			lastModified;
		// The modified time of the resource
		long			modifiedTime;
        
    };
}