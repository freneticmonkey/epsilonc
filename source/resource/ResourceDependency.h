
#include "resource/Resource.h"

namespace epsilon
{
	class ResourceDependency
	{
		typedef std::vector<Resource::Ptr> Dependencies;
	public:
		ResourceDependency(Resource::Ptr res)
		{
			resource = res;
		}

		void AddDependency(Resource::Ptr res)
		{
			dependencies.push_back(res);
		}

		void RemoveDependency(Resource::Ptr res)
		{
			Dependencies::iterator pos = find(dependencies.begin(), dependencies.end(), res);
			if (pos != dependencies.end())
			{
				dependencies.erase(pos);
			}
		}

		bool IsDependent(Resource::Ptr res)
		{
			return find(dependencies.begin(), dependencies.end(), res) != dependencies.end();
		}

		bool IsDependent(long res)
		{
			return find_if(dependencies.begin(), dependencies.end(), [res](Resource::Ptr resource) {
				return resource->GetResourceId() == res;
			}) != dependencies.end();
		}

	private:
		Resource::Ptr resource;
		Dependencies  dependencies;

	};
}
