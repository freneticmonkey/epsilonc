#pragma once 

#include "render/RenderUtilities.h"

namespace epsilon
{
	class TextureData
	{
	public:
		TextureData(GLint iformat, int iwidth, int iheight, int dataSize, unsigned char * imageData);
		~TextureData();

		void ReplaceData(GLint iformat, int iwidth, int iheight, int dataSize, unsigned char * imageData);

		// Getters
		GLenum			GetFormat() { return format; }
		int				GetWidth() { return width; }
		int				GetHeight() { return height; }
		unsigned int	GetSize() { return size; }
		unsigned char * GetPointer() { return data; }		
		bool			HasChanged() { return changed; }
		bool			NeedNewBuffer() { return needNewBuffer; }
		bool			IsValid() { return (data != NULL);  }
		void			MarkRead();

	private:
		GLenum			format;
		int				width;
		int				height;
		unsigned int	size;
		unsigned char * data;

		bool changed;
		bool needNewBuffer;
	};
}