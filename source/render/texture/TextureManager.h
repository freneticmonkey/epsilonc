#pragma once
//
//  TextureManager.h
//  Epsilon
//
//  Created by Scott Porter on 24/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/texture/Texture.h"
#include "render/texture/TextureLoaderInterface.h"
#include "resource/ResourceOwner.h"

namespace epsilon 
{
	class TextureManager :
		public ResourceOwner
	{
	private:
		TextureManager();

	public:
		typedef std::map<std::string, Texture::Ptr>         TextureMap;
		typedef std::vector<TextureLoaderInterface::Ptr>	TextureLoaders;
		static TextureManager & GetInstance()
		{
			static TextureManager instance;
			return instance;
		}
		~TextureManager();

		void        Setup();
		void		LoadTextures();

		Texture::Ptr GetTextureByName(std::string name);
		Texture::Ptr GetTextureByPath(std::string path);
		Texture::Ptr GetDefault() { return defaultTexture; }

		void RefreshResources(ResourceIdVector resources);

		// This will be called from the RenderManager
		void ProcessTextures();

	private:
		TextureMap		textures;
		Texture::Ptr	defaultTexture;

		TextureLoaders	textureLoaders;
	};

	void Setup();
}
