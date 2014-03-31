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
		// Ensure that the path is in the native format so that resources will hash to the same id
		filepath = HashedString(filesystem::path(theFilepath).make_preferred().string());
		type = iType;
	}

	Resource::~Resource()
	{

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

	void Resource::SetModifiedTime(long newModifiedTime)
	{
		modifiedTime = newModifiedTime;
	}
}