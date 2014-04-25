
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include "render/mesh/VertexData.h"

#pragma once 

namespace epsilon
{
	using namespace boost;

	typedef std::vector<std::string> FileExtensions;

	class MeshLoaderInterface
	{
	protected:
		struct private_struct {};
		MeshLoaderInterface(const private_struct &) {}

	public:
		typedef std::shared_ptr<MeshLoaderInterface> Ptr;

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

		static MeshLoaderInterface::Ptr Create() {}

		virtual void LoadMesh(std::string filename, VertexData::Ptr vertexData) = 0;
		virtual void RefreshMesh(std::string filename, VertexData::Ptr vertexData) = 0;

	protected:
		FileExtensions extensions;

	};
}