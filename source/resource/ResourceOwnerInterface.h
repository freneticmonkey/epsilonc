#pragma once
#include "resource/Resource.h"

namespace epsilon
{
	// A List of Resource Ptrs
	typedef std::list<Resource::Ptr> ResourceList;
	typedef std::vector<std::size_t> ResourceIdVector;
	
	class ResourceOwnerInterface
	{
	public:
		//ResourceOwnerInterface() {}
		//~ResourceOwnerInterface() {}

		void SetId(long newID) { uID = newID; }
		long GetId() { return uID; }

		// Should be redefined by derived classes
		virtual void RefreshResources(ResourceIdVector changedResources) {};

	protected:
		long uID;
	};
}