
#include <vector>
#include <string>

#include "render/texture/TextureData.h"

#include <boost/filesystem.hpp>

#pragma once 

namespace epsilon
{
	using namespace boost;

	typedef std::vector<std::string> FileExtensions;

	class TextureLoaderInterface
	{
	protected:
		struct private_struct {};
		TextureLoaderInterface(const private_struct &) {}

	public:
		typedef std::shared_ptr<TextureLoaderInterface> Ptr;

		const FileExtensions & GetExtensions() { return extensions; }

		bool CanLoad(std::string filepath)
		{
			bool result = false;
			
			filesystem::path filePath(filepath);

			std::string ext = filePath.extension().string();

			FileExtensions::iterator foundExt = std::find_if(extensions.begin(), extensions.end(), [ext](std::string extension){
				return ext == extension;
			});

			if (foundExt != extensions.end())
			{
				result = true;
			}

			return result;
		}

		static TextureLoaderInterface::Ptr Create() {}
		virtual TextureData * LoadTexture(std::string filename) = 0;
		virtual void RefreshTexture(TextureData ** textureData, std::string filename) = 0;

	protected:
		FileExtensions extensions;
		
	};
}