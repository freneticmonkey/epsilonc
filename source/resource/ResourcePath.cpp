
#include "resource/ResourcePath.h"

namespace epsilon
{
	ResourcePath::ResourcePath(void)
	{
		// Set default path to 'resources'
		SetBasePath(filesystem::initial_path().generic_string() + "/resources/");
	}

	void ResourcePath::SetBasePath(std::string basepath)
	{
		// Ensure that the new base path is absolute
		filesystem::path bp(basepath);
		if (!bp.is_absolute())
		{
			bp = filesystem::canonical(bp);
		}
		resourcePath = bp.generic_string();
		Log("ResourcePath", "Basepath: " + basepath);

		// Extract the resource folder - used below for determining resource relative paths.

		// Get the slash before the resource path (ignoring possible trailing slashes)
		std::size_t sep = resourcePath.find_last_of("/", resourcePath.size() - 2) + 1;
		resourceFolder = resourcePath.substr(sep, (resourcePath.size() - (1 + sep)));
	}

	std::string ResourcePath::GetResourceFullPath(std::string resourceRelativePath)
	{
		std::string fullpath;

		filesystem::path bp(resourcePath);
		filesystem::path rPath(resourceRelativePath);

		try
		{
			// If the path is relative and includes the resources folder, 
			// remove the resources folder from the path.
			if (*rPath.begin() == resourceFolder)
			{
				filesystem::path temp;

				for (filesystem::path::iterator it = (++rPath.begin()); it != rPath.end(); it++)
				{
					temp /= *it;
				}
				rPath = temp;
			}

			// Check if the path is already absolute
			if (rPath.has_root_directory())
			{
				// incase there is some relative wackiness going on.
				fullpath = filesystem::canonical(rPath).generic_string();
			}
			else
			{

				// Check if the path is a sub path under the resource folder.
				filesystem::path rFolder = bp / (*rPath.begin());
                filesystem::path fPath = filesystem::complete(bp / rPath);
				if (filesystem::exists(rFolder))
				{
					// build the final path
					fullpath = filesystem::complete(bp / rPath).generic_string();
					//Log("ResourceManager", "Resolved Resource Path: " + fullpath );
				}
                // Check if it's a file in the root resources folder
                else if ( filesystem::exists(fPath) )
                {
                    fullpath = fPath.generic_string();
                }
				else
				{

					rPath = filesystem::complete(bp / rPath);
					Log("ResourcePath", "Error Resolving Resource Path. Path doesn't exist: " + rPath.generic_string());
					//throw ResourcePathException(resourceRelativePath);
				}
			}
		}
		catch (boost::filesystem::filesystem_error e)
		{
			Log("ResourcePath", "Error Resolving Resource Path. Path doesn't exist: " + rPath.generic_string());
			throw ResourcePathException(resourceRelativePath);
		}

		return fullpath;
	}
}
