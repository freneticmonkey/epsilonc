#pragma once 
#include "render/texture/TextureLoaderInterface.h"

namespace epsilon
{
	class TextureLoaderBMP : public TextureLoaderInterface
	{
	private:
		struct private_struct {};
		
	public:
		typedef std::shared_ptr<TextureLoaderBMP> Ptr;
		
		static TextureLoaderBMP::Ptr Create();
		explicit TextureLoaderBMP(const private_struct &);
		
		TextureData *	LoadTexture(std::string filename);
		void			RefreshTexture(TextureData ** textureData, std::string filename);

	private:
		bool	LoadTexture(TextureData ** textureData, std::string filename);

	};
}