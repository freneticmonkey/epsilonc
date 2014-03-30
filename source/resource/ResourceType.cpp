#include "resource/ResourceType.h"
#include "utilities/Utilities.h"

namespace epsilon
{
	// TODO: Implement reading from file. At this point all resource require
	// C++ changes to load, so filetypes might as well be hardcoded for now.
	ResourceType::ResourceTypeMap ResourceType::typemap = {
		{ "txt", ResourceType::Type::TEXT },

		{ "py", ResourceType::Type::SCRIPT },

		{ "obj", ResourceType::Type::MESH },

		{ "png", ResourceType::Type::TEXTURE },
		{ "jpg", ResourceType::Type::TEXTURE },

		{ "vert", ResourceType::Type::SHADER },
		{ "frag", ResourceType::Type::SHADER },
		{ "geom", ResourceType::Type::SHADER },

		{ "wav", ResourceType::Type::SOUND }
	};

	ResourceType::ResourceTypeNamesMap ResourceType::typenames = {
		{ "unknown", ResourceType::Type::UNKNOWN },
		{ "text", ResourceType::Type::TEXT },
		{ "script", ResourceType::Type::SCRIPT },
		{ "mesh", ResourceType::Type::MESH },
		{ "texture", ResourceType::Type::TEXTURE },
		{ "shader", ResourceType::Type::SHADER },
		{ "sound", ResourceType::Type::SOUND }
	};

	ResourceType::ResourceType()
	{
		//ResourceType::ResourceType(std::string typeDefPath)

		// Read the file pointed to by string and fill the type definitions
		//std::string fileTypes = readfile(typeDefPath);

		// Split on newlines
		//std::vector<std::string> lines = split(fileTypes, '\n');

	}

	ResourceType::~ResourceType()
	{

	}
}