#include "resource/Resource.h"
#include <boost/filesystem.hpp>

#include "resource/ResourcePath.h"

using namespace boost;

namespace epsilon
{
	Resource::Ptr Resource::Create(std::string theFilePath, ResourceType::Type resType)
	{
		return std::make_shared<Resource>(theFilePath, resType);
	}

	Resource::Resource(std::string theFilepath, ResourceType::Type iType)
	{
		type = iType;
		SetFilename(theFilepath);
	}

	Resource::~Resource()
	{

	}
    
	void Resource::SetFilename(std::string updatedFilename)
    {
		validFile = false;
		try
		{
			// If the path is valid
			if (updatedFilename.size() > 0)
			{
				// Process the path into an absolute path
				updatedFilename = ResourcePath::GetInstance().GetResourceFullPath(updatedFilename);
				validFile = true;
			}
		}
		catch (ResourcePathException &rpe)
		{
			// Ignore invalid path exceptions.  
			// The resource file will be marked as invalid by default
		}
		
		filesystem::path fullpath(updatedFilename);

		// Ensure that the path is in the native format so that resources will hash to the same id
		filepath = HashedString(fullpath.make_preferred().generic_string());
		extension = fullpath.extension().generic_string();
		//Log("Resource: " + filepath.GetString() + " Ext: " + extension);
    }
	
	bool Resource::IsMatchingPath(std::string path)
	{
		return filepath.GetString() == path;
	}

	void Resource::AddOwner(long owner)
	{
		ownerIds.push_back(owner);
	}
    
    void Resource::RemoveOwner(long owner)
    {
        ownerIds.erase(std::find(ownerIds.begin(), ownerIds.end(), owner));
    }
    
    bool Resource::IsOwner(long owner)
    {
        return std::find(ownerIds.begin(), ownerIds.end(), owner) != ownerIds.end();
    }
}