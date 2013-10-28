#include "render/VertexBuffer.h"

namespace epsilon
{
	VertexBuffer::Ptr VertexBuffer::Create()
	{
		return std::make_shared<VertexBuffer>(private_struct());
	}

	VertexBuffer::VertexBuffer(const private_struct &)
	{
		count = 0;
		// Initialise everything to -1
		vertexAttribIndex = -1;
		vertexStride = -1;
		normalAttribIndex = -1;
		normalStride = -1;
		colourAttribIndex = -1;
		colourStride = -1;
		texCoordAttribIndex = -1;
		texCoordStride = -1;

		hasBuffers = false;
	}

	VertexBuffer::~VertexBuffer()
	{
		if ( vertexBuffer != -1 )
		{
			glDeleteBuffers(1, &vertexBuffer);
		}
		if ( indicesBuffer != -1 )
		{
			glDeleteBuffers(1, &indicesBuffer);
		}
	}

	void VertexBuffer::SetData(VectorList vertices, VectorList normals, IndicesList indices, TexCoordList texCoords)
	{
		// Check that all of the required data is available
		if ( (vertices.size() == normals.size()) && ( normals.size() == texCoords.size() ) )
		{
			// Build vertex data
			count = vertices.size();

			// This is the number of floats for this vertex
			// pos + normal + texcoords
			vertexBufferStride = sizeof(GLfloat) * 8;
			
			VectorList::iterator vec = vertices.begin();
			VectorList::iterator norm = normals.begin();
			TexCoordList::iterator texCoord = texCoords.begin();

			while ( vec != vertices.end() )
			{
				vertexData.push_back((*vec).x);
				vertexData.push_back((*vec).y);
				vertexData.push_back((*vec).z);
				vertexData.push_back((*norm).x);
				vertexData.push_back((*norm).y);
				vertexData.push_back((*norm).z);
				vertexData.push_back((*texCoord).x);
				vertexData.push_back((*texCoord).y);

				vec++;
				norm++;
				texCoord++;
			}

			vertexAttribIndex = 1;
			vertexStride = 0;
			
			normalAttribIndex = 2;
			normalStride = sizeof(GLfloat) * 3;

			texCoordAttribIndex = 3;
			texCoordStride = normalStride + (sizeof(GLfloat) * 3);

			// Build indices data
			indicesData = std::vector<GLshort>(indices.begin(), indices.end());

			hasBuffers = MakeBuffers(vertexData, indicesData);
		}
		else
		{
			Log("ERROR: VertexBuffer::SetData: Invalid length mesh data.");
		}
	}

	void VertexBuffer::SetData(VectorList vertices, VectorList normals, ColourList colours, IndicesList indices, TexCoordList texCoords)
	{
		// Check that all of the required data is available
		if ( (vertices.size() == normals.size()) && ( normals.size() == colours.size() ) && ( colours.size() == texCoords.size() ) )
		{
			// Build vertex data
			count = vertices.size();

			// This is the number of floats for this vertex
			// pos + normal + colour + texcoords
			vertexBufferStride = sizeof(GLfloat) * 12;
			
			VectorList::iterator vec = vertices.begin();
			VectorList::iterator norm = normals.begin();
			ColourList::iterator colour = colours.begin();
			TexCoordList::iterator texCoord = texCoords.begin();

			while ( vec != vertices.end() )
			{
				vertexData.push_back((*vec).x);
				vertexData.push_back((*vec).y);
				vertexData.push_back((*vec).z);

				vertexData.push_back((*norm).x);
				vertexData.push_back((*norm).y);
				vertexData.push_back((*norm).z);

				vertexData.push_back((*colour).x);
				vertexData.push_back((*colour).y);
				vertexData.push_back((*colour).z);
				vertexData.push_back((*colour).w);

				vertexData.push_back((*texCoord).x);
				vertexData.push_back((*texCoord).y);

				vec++;
				norm++;
				colour++;
				texCoord++;
			}

			vertexAttribIndex = 1;
			vertexStride = 0;
			
			normalAttribIndex = 2;
			normalStride = sizeof(GLfloat) * 3;

			colourAttribIndex = 3;
			colourStride = normalStride + (sizeof(GLfloat) * 3);

			texCoordAttribIndex = 4;
			texCoordStride = colourStride + (sizeof(GLfloat) * 4);

			// Build indices data
			indicesData = std::vector<GLshort>(indices.begin(), indices.end());

			hasBuffers = MakeBuffers(vertexData, indicesData);
		}
		else
		{
			Log("ERROR: VertexBuffer::SetData: Invalid length mesh data.");
		}
	}

	//bool VertexBuffer::MakeBuffers(const void *vertexData, GLsizei vertSize, const void *indicesData, GLsizei indSize)
	bool VertexBuffer::MakeBuffers(std::vector<GLfloat> vertexData, std::vector<GLshort> indicesData)
	{	
		bool success = true;
		
		GLuint vb;
		GLuint ib;
		
		vertexBufferSize = sizeof(GLfloat) * vertexData.size();
		
		// Creating Vertex VBO
		GLenum ErrorCheckValue = glGetError();
		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, &vertexData[0], GL_STATIC_DRAW);
		
		vertexBuffer = vb;

		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("ERROR: Creating Vertex Buffer Object.");
			success = false;
		}

		// Creating Indices VBO
		static const GLshort inds[] = {0, 1, 2};

		indicesBufferSize = sizeof(GLshort) * indicesData.size();
		ErrorCheckValue = glGetError();
		
		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBufferSize, &indicesData[0], GL_STATIC_DRAW);
		
		indicesBuffer = ib;

		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("ERROR: Creating Index Buffer Object.");
			success = false;
		}

		return success;
	}

	void VertexBuffer::Enable()
	{
		if ( !hasBuffers )
		{
			Log("ERROR: VertexBuffer::Enable: Buffers have not been defined");
			return;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		
		if ( vertexStride != -1 )
		{
			glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, vertexBufferStride, (GLvoid*)0);
			glEnableVertexAttribArray(vertexAttribIndex);
		}

		if ( normalStride != -1 )
		{
			glVertexAttribPointer(normalAttribIndex, 3, GL_FLOAT, GL_FALSE, vertexBufferStride, (GLvoid*)normalStride);
			glEnableVertexAttribArray(normalAttribIndex);
		}
		if ( colourStride != -1 )
		{
			glVertexAttribPointer(colourAttribIndex, 4, GL_FLOAT, GL_FALSE, vertexBufferStride, (GLvoid*)colourStride);
			glEnableVertexAttribArray(colourAttribIndex);
		}
		if ( texCoordStride != -1 )
		{
			glVertexAttribPointer(texCoordAttribIndex, 2, GL_FLOAT, GL_FALSE, vertexBufferStride, (GLvoid*)texCoordStride);
			glEnableVertexAttribArray(texCoordAttribIndex);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	}

	void VertexBuffer::Draw()
	{
		if ( hasBuffers )
		{
			GLenum ErrorCheckValue = glGetError();

			Enable();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
			Disable();

			ErrorCheckValue = glGetError();

			if (ErrorCheckValue != GL_NO_ERROR)
			{
				Log("Wahhhh woooowwwwww. OpenGL error");
			}
		}
	}

	void VertexBuffer::Disable()
	{
		if ( !hasBuffers )
		{
			Log("ERROR: VertexBuffer::Disable: Buffers have not been defined");
			return;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		if ( vertexStride != -1 )
		{
			glDisableVertexAttribArray(vertexAttribIndex);
		}
		if ( normalStride != -1 )
		{
			glDisableVertexAttribArray(normalAttribIndex);
		}
		if ( colourStride != -1 )
		{
			glDisableVertexAttribArray(colourAttribIndex);
		}
		if ( texCoordStride != -1 )
		{
			glDisableVertexAttribArray(texCoordAttribIndex);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	int VertexBuffer::GetVertexIndex()
	{
		return vertexAttribIndex;
	}

	int VertexBuffer::GetNormalIndex()
	{
		return normalAttribIndex;
	}

	int VertexBuffer::GetColourIndex()
	{
		return colourAttribIndex;
	}

	int VertexBuffer::GetTexCoordIndex()
	{
		return texCoordAttribIndex;
	}

}