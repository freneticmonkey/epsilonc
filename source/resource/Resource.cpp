#include "resource/Resource.h"
#include <boost/filesystem.hpp>

using namespace boost;

namespace epsilon
{
	Resource::Ptr Resource::Create(std::string theFilePath, ResourceType::Type resType)
	{
		return std::make_shared<Resource>(theFilePath, resType);
	}

	Resource::Resource(std::string theFilepath, ResourceType::Type iType) : ownerId(0)
	{
		// Ensure that the path is in the native format so that resources will hash to the same id
		filepath = HashedString(filesystem::path(theFilepath).make_preferred().string());
		type = iType;
	}

	Resource::~Resource()
	{

	}
	
	void Resource::SetOwner(long owner)
	{
		ownerId = owner;
	}

	void Resource::SetModifiedTime(long newModifiedTime)
	{
		modifiedTime = newModifiedTime;
	}
}