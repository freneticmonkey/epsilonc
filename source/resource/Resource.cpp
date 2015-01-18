#include "resource/Resource.h"
#include <boost/filesystem.hpp>

using namespace boost;

namespace epsilon
{
	Resource::Ptr Resource::Create(std::string theFilePath, ResourceType::Type resType)
	{
		return std::make_shared<Resource>(theFilePath, resType);
	}

	Resource::Resource(std::string theFilepath, ResourceType::Type iType)
	{
		filesystem::path fullpath(theFilepath);
		// Ensure that the path is in the native format so that resources will hash to the same id
		filepath = HashedString(fullpath.make_preferred().generic_string());
		type = iType;
		extension = fullpath.extension().generic_string();
		//Log("Resource: " + filepath.GetString() + " Ext: " + extension);
	}

	Resource::~Resource()
	{

	}
    
    void Resource::UpdateResourceFilename(std::string updatedFilename)
    {
		filepath = HashedString(filesystem::path(updatedFilename).make_preferred().generic_string());
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