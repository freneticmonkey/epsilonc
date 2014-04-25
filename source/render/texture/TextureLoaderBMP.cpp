#include "render/texture/TextureLoaderBMP.h"
#include "EpsilonCore.h"

namespace epsilon
{
	TextureLoaderBMP::Ptr TextureLoaderBMP::Create()
	{
		return std::make_shared<TextureLoaderBMP>(private_struct());
	}

	TextureLoaderBMP::TextureLoaderBMP(const private_struct &) : 
							TextureLoaderInterface(TextureLoaderInterface::private_struct())
	{
		extensions.push_back(".bmp");
		extensions.push_back(".BMP");
	}

	TextureData * TextureLoaderBMP::LoadTexture(std::string filename)
	{
		// Do bitmap loady things here
		TextureData * textureData = NULL;
		LoadTexture(&textureData, filename);

		return textureData;
	}

	void TextureLoaderBMP::RefreshTexture(TextureData ** textureData, std::string filename)
	{
		// Do bitmap refreshy things here
		LoadTexture(textureData, filename);
	}

	bool TextureLoaderBMP::LoadTexture(TextureData ** textureData, std::string filename)
	{
		bool result = false;

		// Data read from the header of the BMP file
		unsigned char header[54]; // Each BMP file begins by a 54-bytes header
		unsigned int dataPos;     // Position in the file where the actual data begins
		unsigned int width, height;
		unsigned int imageSize;   // = width*height*3
		// Actual RGB data
		unsigned char * data;

		FILE * file = fopen(filename.c_str(), "rb");
		// Could add some error handling business here.

		if (!file)
		{
			Log("TextureLoaderBMP", "Sadness couldn't open file: " + filename);
			result = false;
		}
		else
		{
			// If the file doesn't have a 54 byte header
			if ((fread(header, 1, 54, file) != 54) ||
				// or the first two bytes aren't B and M
				(header[0] != 'B' || header[1] != 'M'))
			{
				Log("TextureLoaderBMP", "Not a valid BMP file or corrupted header: " + filename);
				result = false;
			}
			else
			{
				// Read ints from the byte array
				dataPos = *(int*)&(header[0x0A]);
				imageSize = *(int*)&(header[0x22]);
				width = *(int*)&(header[0x12]);
				height = *(int*)&(header[0x16]);

				// Some BMP files are misformatted, guess missing information
				if (imageSize == 0)
					imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component

				if (dataPos == 0)
					dataPos = 54; // Skip over the header

				// Create a buffer
				data = new unsigned char[imageSize];

				// Read the actual data from the file into the buffer
				fread(data, 1, imageSize, file);

				//Everything is in memory now, the file can be closed
				fclose(file);

				// If this is a new texture
				if (*textureData == NULL)
				{
					// Assign the data to the TextureData object
					*textureData = new TextureData(GL_BGR, width, height, imageSize, data);
				}
				else
				{
					(*textureData)->ReplaceData(GL_BGR, width, height, imageSize, data);
				}

				result = true;
			}
		}

		return result;
	}
}