
#include "render/texture/TextureData.h"

namespace epsilon
{
	TextureData::TextureData(GLint iformat, int iwidth, int iheight, int dataSize, unsigned char * imageData) :
																								format(iformat),
																								width(iwidth),
																								height(iheight),
																								size(dataSize),
																								changed(true),
																								needNewBuffer(true),
																								data(imageData)
	{
	}

	TextureData::~TextureData()
	{
		// Cleanup memory
		if (data)
		{
			delete[] data;
			data = NULL;
		}
	}

	void TextureData::MarkRead()
	{
		changed = false;
		needNewBuffer = false;

		// Delete the existing data as we no longer need it
		if (data)
		{
			delete[] data;
			data = NULL;
		}
	}

	void TextureData::ReplaceData(GLint iformat, int iwidth, int iheight, int dataSize, unsigned char * imageData)
	{
		// If the dimensions or format has changed
		// mark that we need a new buffer
		if ((format != iformat) || (width != iwidth) || (height != iheight))
		{
			// set the new properties
			format = iformat;
			width = iwidth;
			height = iheight;

			// and that we need a new buffer
			needNewBuffer = true;
		}

		size = dataSize;

		// Set the new data
		data = imageData;

		// Mark the data as changed
		changed = true;
	}
}