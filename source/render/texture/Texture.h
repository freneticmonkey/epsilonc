#pragma once
#include "EpsilonCore.h"

#include "resource/Resource.h"
#include "render/texture/TextureData.h"
#include "render/RenderUtilities.h"

#include "render/texture/TextureLoaderInterface.h"

namespace epsilon
{
	class Texture :
		public std::enable_shared_from_this<Texture>,
		public Resource
	{
	protected:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Texture> Ptr;

		// Use default constructor.  
		// Maybe texture buffers could be written into by scripts? 
		// Not sure how that would work with Resources though.
		static Texture::Ptr Create();

		static Texture::Ptr CreateFromFile(std::string filename);

		// Default constructor.  Maybe texture buffers could be written into by scripts?
		explicit Texture(const private_struct &);

		// Constructor for specifying a Texture from a file.
		explicit Texture(const private_struct &, std::string filename);

		~Texture(void);

		std::string GetName() { return name; }

		Texture::Ptr ThisPtr() { return shared_from_this(); }

		bool SetTextureLoader(TextureLoaderInterface::Ptr texLoader);

		// Do initial load of texture and creation of OpenGL buffers?
		void LoadTexture();

		void RefreshFromFile();

		// Refresh OpenGL texture data
		void RefreshBuffers();

		// Texture Control
		GLint GetBindLocation() { return bindLocation; }
		GLint GetGLBindLocation() { return bindLocation + GL_TEXTURE0; }

		// Where location 0<->31
		Texture::Ptr SetBindLocation(GLint location);

		bool Bind();
		bool Disable();
		bool IsActive() { return active; }

		// Python Info Getters
		int				GetWidth();
		int				GetHeight();
		unsigned int	GetSize();
		bool			OnGPU();

		bool			IsLoaded() { return loaded; }

	private:

		// Copy the texture data to the GPU
		void BindTextureData();

		std::string					name;
		TextureData				  * textureData;		
		TextureLoaderInterface::Ptr textureLoader;

		// 
		bool	active;
		GLuint	textureId;
		GLint	bindLocation;
		bool	genBuffer;

		bool	loaded;
	};

	typedef std::vector<Texture::Ptr> Textures;

}