#pragma once
//
//  Resource.h
//  Epsilon
//
//  Created by Scott Porter on 25/03/2014.
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
        
        int					GetType() { return type; }
		const std::string & GetExtension() { return extension; }
        
        // Ensure that type based comparison is implemented?
//        virtual int operator=;

		bool IsValidFile() { return validFile; }

		// Check if path is path to resource.  Path is an absolute path to the resource
		bool IsMatchingPath(std::string path);
        
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
		void SetModifiedTime(long newModifiedTime)
		{
			modifiedTime = newModifiedTime;
		}

		// Get the modified time of the resource when it was last reloaded
		uintmax_t GetLastFileSize(){ return lastFileSize; }

		// Get the current modified time of the resource
		uintmax_t GetFileSize() { return fileSize; }

		// Set the modified time of the resource - Intended for use by the Resource Manager
		void SetFileSize(uintmax_t newFileSize)
		{
			fileSize = newFileSize;
		}

		// Indicate that the in-memory resource is out of date
		bool NeedReload() 
		{ 
			return (lastModified != modifiedTime) || (lastFileSize != fileSize); 
		}

		// Notify the resource that the in-memory copy is up to date.
		void SetReloaded() 
		{ 
			lastModified = modifiedTime;
			lastFileSize = fileSize;
		}
        
		// TODO!: Add file reading writing functionality in here. :)
    protected:
        // This is called from the ResourceManager.  Inheriting classes must call
		// ResourceManager::Update
        void SetFilename(std::string updatedFilename);

    private:
        OwnerIds		ownerIds;
        
		bool			validFile;

        int				type;
		std::string		extension;

		HashedString	filepath;
		// The modified time on last resource load.
		long			lastModified;
		// The modified time of the resource
		long			modifiedTime;

		// The filesize on last resource load.
		uintmax_t		lastFileSize;
		// The current filesize of the resource
		uintmax_t		fileSize;
        
    };
}