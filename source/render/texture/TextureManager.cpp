//
//  TextureManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 24/04/2014.
//

#include "render/texture/TextureManager.h"
#include "render/texture/TextureLoaderBMP.h"

#include "resource/ResourceManager.h"
#include "resource/ResourcePath.h"

#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;

	TextureManager::TextureManager()
	{

	}

	TextureManager::~TextureManager()
	{

	}

	void TextureManager::Setup()
	{
		// Register Texture Loaders
		textureLoaders.push_back(TextureLoaderBMP::Create());

		// build a regular expression of supported texture formats
		std::string textureRegex;
		std::vector<std::string> exts;

		std::for_each(textureLoaders.begin(), textureLoaders.end(), [&](TextureLoaderInterface::Ptr textureLoader){
			FileExtensions ext = textureLoader->GetExtensions();
			exts.insert(exts.end(), ext.begin(), ext.end());
		});

		if (false)
		{
			// Pre-allocate the regex assuming a maximum extension length of 5
			textureRegex.reserve(exts.size() * 5);

			for (std::vector<std::string>::iterator e = exts.begin(); e != exts.end(); e++)
			{
				textureRegex += (*e);

				// If not the last extension, insert a pipe
				if (e < (exts.end() - 1))
				{
					textureRegex += "|";
				}
			}

			textureRegex = str(format(".*(%s)$") % textureRegex);

			// Search the ResourceManager for all files with supported texture extensions
			ResourceList results = ResourceManager::GetInstance().FindResources(textureRegex);
		}
		ResourceList results = ResourceManager::GetInstance().FindResourcesByExtension(exts);

		// For each of the results
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
			// Create a new texture
			Texture::Ptr newTexture = Texture::CreateFromFile(resource->GetFilepath().GetString());
			// Add it to the managed textures
			textures[newTexture->GetName()] = newTexture;
			// Register it for change events
			RegisterResource(newTexture);

			// Assign it a texture loader
			std::find_if(textureLoaders.begin(), textureLoaders.end(), [&](TextureLoaderInterface::Ptr texLoader){
				return newTexture->SetTextureLoader(texLoader) == true;
			});

			//Log("TextureManager", str(format("Registered Texture: %s") % newTexture->GetName()));
		});

		Log("TextureManager", boost::str(format("Registered %d textures.") % results.size()));

		// For now we'll just immediately load textures
		// TODO: Add update function which loads necessary textures
		LoadTextures();
	}

	void TextureManager::LoadTextures()
	{
		// Breaking this out into a separate function as it might be a little heavy.
		std::for_each(textures.begin(), textures.end(), [](std::pair < std::string, Texture::Ptr> texture){
			texture.second->LoadTexture();
		});
	}

	Texture::Ptr TextureManager::GetTextureByName(std::string name)
	{
		Texture::Ptr foundTexture;
		if (textures.find(name) != textures.end())
		{
			foundTexture = textures[name];
		}
		return foundTexture;
	}

	Texture::Ptr TextureManager::GetTextureByPath(std::string path)
	{
		Texture::Ptr foundTexture;
		std::string fullpath = ResourcePath::GetInstance().GetResourceFullPath(path);

		for (TextureMap::iterator it = textures.begin(); it != textures.end(); it++)
		{
			if (it->second->GetFilepath().GetString() == fullpath)
			{
				foundTexture = it->second;
				break;
			}
		}

		return foundTexture;
	}

	void TextureManager::RefreshResources(ResourceIdVector resources)
	{
		// Reload any changed files from disk.  This will not push them to the GPU.
		std::for_each(resources.begin(), resources.end(), [&](std::size_t resourceId){

			// Check if this texture is one of the changed textures.
			std::for_each(textures.begin(), textures.end(), [&](std::pair < std::string, Texture::Ptr> texture){
				if (texture.second->GetResourceId() == resourceId)
				{
					texture.second->RefreshFromFile();
				}
			});
		});
	}

	void TextureManager::ProcessTextures()
	{
		// Push any changed texture data to the GPU.
		std::for_each(textures.begin(), textures.end(), [](std::pair < std::string, Texture::Ptr> texture){
			texture.second->RefreshBuffers();
		});
	}
}