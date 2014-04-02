#pragma once

#include <map>
#include <algorithm>
#include <ctype.h>
#include <string>

namespace epsilon
{
	/*
	 * ResourceType
	 * 
	 * Handles a currently hardcoded mapping between resource types and file extensions.
	 */
	class ResourceType
	{
	public:

		enum Type
		{
			UNKNOWN = 1,
			TEXT,
			SCRIPT,
			MESH,
			TEXTURE,
			SHADER,
			SOUND
		};

		typedef std::map<std::string, ResourceType::Type> ResourceTypeMap;
		typedef std::map<std::string, ResourceType::Type> ResourceTypeNamesMap;

		ResourceType();
		~ResourceType();

		static ResourceType::Type GetResourceTypeByExt(std::string ext)
		{
			// Ensure lowercase
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			ResourceType::Type type = Type::UNKNOWN;

			ResourceTypeMap::iterator it = typemap.find(ext);

			if (it != typemap.end())
			{
				type = typemap[ext];
			}
			return type;
		}

		static ResourceType::Type GetResourceTypeByName(std::string name)
		{
			// Ensure lowercase
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			ResourceType::Type type = Type::UNKNOWN;

			ResourceTypeNamesMap::iterator it = typenames.find(name);

			if (it != typenames.end())
			{
				type = typenames[name];
			}
			return type;
		}

	private:

		static ResourceTypeMap typemap;
		static ResourceTypeNamesMap typenames;
	};
}
