
#include "render/texture/Texture.h"

#include "resource/ResourceManager.h"
#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;

	Texture::Ptr Texture::Create()
	{
		return std::make_shared<Texture>(private_struct());
	}

	Texture::Ptr Texture::CreateFromFile(std::string filename)
	{
		return std::make_shared<Texture>(private_struct(), filename);
	}

	Texture::Texture(const private_struct &) : Resource("", ResourceType::Type::TEXTURE), 
											   active(false),
											   textureId(0),
											   bindLocation(-1),
											   genBuffer(false),
											   loaded(false)
	{
			
	}

	Texture::Texture(const private_struct &, std::string filename) : Resource(filename, ResourceType::Type::TEXTURE),
																	active(false),
																	textureId(0),
																	bindLocation(-1),
																	genBuffer(false),
																	loaded(false)
	{
		// Name the Texture - This should really be done by the texture manager because of duplicates
		name = filename;
		
		// strip the path and just use the filename
		unsigned spos = filename.rfind("/");
		if (spos != std::string::npos)
		{
			name = filename.substr(spos + 1, std::string::npos);
		}
	}

	Texture::~Texture(void)
	{
		// Ensure we cleanup one of the only raw pointers we have.
		if (loaded && textureData)
		{
			delete textureData;
		}

		// Cleanup buffer
		if ( textureId != 0 )
		{
			glDeleteTextures(1, &textureId);
			textureId = 0;
		}
	}

	bool Texture::SetTextureLoader(TextureLoaderInterface::Ptr texLoader)
	{
		bool result = false;
		if (texLoader->CanLoad(GetFilepath().GetString()))
		{
			textureLoader = texLoader;
			result = true;
		}
		return result;
	}

	void Texture::LoadTexture()
	{
		// Load Texture	
		if (textureLoader)
		{
			// Read the file from disk
			textureData = textureLoader->LoadTexture(GetFilepath().GetString());

			if (textureData != NULL)
			{
				loaded = true;
			}

			// mark the file as reloaded regardless of whether the texture was loaded or not.
			SetReloaded();
		}
	}

	void Texture::BindTextureData()
	{
		if (textureData && textureData->IsValid())
		{
			// If the data is new or has changed dimensions
			if (textureData->NeedNewBuffer())
			{
				// Clean up the existing texture
				if (textureId != 0)
				{
					glDeleteTextures(1, &textureId);
					textureId = 0;
					CheckOpenGLError("Deleting Texture");
				}

				// Generate and Bind a new texture
				glGenTextures(1, &textureId);
				CheckOpenGLError("Generating Texture");
			}
			
			glBindTexture(GL_TEXTURE_2D, textureId);
			CheckOpenGLError("Binding Texture Id");

			glTexImage2D(GL_TEXTURE_2D,		 // GLenum target
				0,							 // GLint level
				GL_RGB, 					 // GLint internalformat
				textureData->GetWidth(),	 // GLsizei width
				textureData->GetHeight(),	 // GLsizei height
				0,  						 // GLint border
				textureData->GetFormat(),	 // GLenum format
				GL_UNSIGNED_BYTE, 			 // GLenum type
				textureData->GetPointer()	 // const GLvoid *pixels
				);

			CheckOpenGLError("Loading Texture Data for:" + name);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			// Mipmapping options

			/*
			// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			// Generate mipmaps, by the way.
			glGenerateMipmap(GL_TEXTURE_2D);
			*/

			glBindTexture(GL_TEXTURE_2D, 0);

			// Mark the texture data as read
			textureData->MarkRead();
		}
	}

	void Texture::RefreshFromFile()
	{
		// Do the refresh
		if (textureLoader)
		{
			// Load the texture data from disk
			textureLoader->RefreshTexture(&textureData, GetFilepath().GetString());
		}

		// mark the file as reloaded regardless of whether the texture was loaded or not.
		SetReloaded();
	}

	void Texture::RefreshBuffers()
	{
		// If the texture needs to be refresh continue.
		if (loaded && textureData->HasChanged())
		{
			// Push the new texture data to OpenGL
			BindTextureData();
		}
	}

	Texture::Ptr Texture::SetBindLocation(GLint location)
	{
		bindLocation = location;
		return ThisPtr();
	}

	// Texture Control
	bool Texture::Bind()
	{
		if (bindLocation != -1)
		{
			// Bind the texture to the bind location
			glActiveTexture(GL_TEXTURE0 + bindLocation);
			glBindTexture(GL_TEXTURE_2D, textureId);
			
			active = CheckOpenGLError("Binding Texture");

			if ( !active )
			{
				Log(str(format("Error Binding texture: %s to ID: %d") % name % textureId));
			}
		}
		return active;
	}

	bool Texture::Disable()
	{
		if (active)
		{
			// We don't need to unbind? as Bind automatically breaks previous bind.
			glActiveTexture(GL_TEXTURE0 + bindLocation);
			glBindTexture(GL_TEXTURE_2D, 0);

			active = false;
		}

		return active;
	}

	int	Texture::GetWidth() 
	{
		return (textureData) ? textureData->GetWidth() : 0;
	}

	int	Texture::GetHeight() 
	{
		return (textureData) ? textureData->GetHeight() : 0;
	}

	unsigned int Texture::GetSize() 
	{
		return (textureData) ? textureData->GetSize() : 0;
	}

	bool Texture::OnGPU()
	{
		return (textureData) ? !textureData->HasChanged() : true;
	}
}