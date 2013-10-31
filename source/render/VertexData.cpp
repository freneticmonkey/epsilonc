#include "render/VertexData.h"

namespace epsilon
{
	VertexData::Ptr VertexData::Create()
	{
		return std::make_shared<VertexData>(private_struct());
	}

	VertexData::VertexData(const private_struct &)
	{
		numVertices = 0;
		vertexIndex = -1;
		normalIndex = -1;
		colourIndex = -1;
		texCoordIndex = -1;

		/*
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
		*/
	}

	VertexData::~VertexData()
	{
		while( !buffers.empty() )
		{
			delete buffers.back();
			buffers.pop_back();
		}

		while( !attributes.empty() )
		{
			delete attributes.back();
			attributes.pop_back();
		}
	}

	VertexData& VertexData::SetVertices(VerticesData::List vertexData)
	{
		VerticesData * vertices = new VerticesData(vertexData, VertexAttribType::VERTEX);
		vertexIndex = attributes.size();
		vertices->SetAttribIndex(vertexIndex);
		numVertices = vertices->DataLength();
		attributes.push_back( vertices );
		return *this;
	}

	VertexData& VertexData::SetNormals(NormalData::List normalData)
	{
		NormalData * normals = new NormalData(normalData, VertexAttribType::NORMAL);
		normalIndex = attributes.size() + 1;
		normals->SetAttribIndex(normalIndex);
		attributes.push_back( normals );
		return *this;
	}

	VertexData& VertexData::SetColours(ColourData::List colourData)
	{
		ColourData * colours = new ColourData(colourData, VertexAttribType::COLOUR);
		colourIndex = attributes.size();
		colours->SetAttribIndex(colourIndex);
		attributes.push_back( colours );
		return *this;
	}

	VertexData& VertexData::SetTexCoords(TexCoordData::List texCoordData)
	{
		TexCoordData * texCoords = new TexCoordData(texCoordData, VertexAttribType::TEXCOORD);
		texCoordIndex = attributes.size();
		texCoords->SetAttribIndex(texCoordIndex);
		attributes.push_back( texCoords );
		return *this;
	}

	VertexData& VertexData::SetIndices(VertexIndicesBuffer::BufferList indicesData)
	{
		buffers.push_back(new VertexIndicesBuffer(indicesData, VertexBufferType::INDICES) );
		return *this;
	}

	void VertexData::BuildBuffers()
	{
		// Build the vertex buffer data
		size_t stride = 0;
		VertexDataBuffer::BufferList vertexData;

		int attribLength = attributes[0]->DataLength();
		int numUnits = -1;

		// For each vertex
		int attribSize = attributes.size();
		
		for ( int i = 0; i < attribLength; i++ )
		{
			// Cycle through each attribute
			for ( int a = 0; a < attribSize; a++ )
			{
				// Write each value of the attribute into the Vertex Buffer
				for (int u = 0; u < attributes[a]->GetUnitNum(); u++ )
				{
					vertexData.push_back( attributes[a]->GetUnitValue(i,u) );
				}
			}
		}

		// Create a Vertex Buffer and add it to the buffers list
		buffers.push_back(new VertexDataBuffer(vertexData, VertexBufferType::ELEMENT));

		// Calculate the vertex buffer attribute stride values
		for ( VertexAttribList::iterator attrib = attributes.begin(); attrib != attributes.end(); attrib++ )
		{
			stride = (*attrib)->UpdateStride(stride);
		}

		// Update the attributes with the total vertex buffer stride
		for ( VertexAttribList::iterator attrib = attributes.begin(); attrib != attributes.end(); attrib++ )
		{
			(*attrib)->SetBufferStride(stride);
		}

		// Now Send the Buffers to OpenGL
		for ( VertexBufferList::iterator buffer = buffers.begin(); buffer != buffers.end(); buffer++ )
		{
			(*buffer)->BuildBuffer();
		}
	}

	void VertexData::Enable()
	{
		// Enable the OpenGL Buffers
		for ( VertexBufferList::iterator buffer = buffers.begin(); buffer != buffers.end(); buffer++ )
		{
			(*buffer)->Enable();
		}

		// Enable the Vertex Attributes
		for ( VertexAttribList::iterator attrib = attributes.begin(); attrib != attributes.end(); attrib++ )
		{
			(*attrib)->Enable();
		}
	}

	void VertexData::Disable()
	{
		// Disable the OpenGL Buffers
		for ( VertexBufferList::iterator buffer = buffers.begin(); buffer != buffers.end(); buffer++ )
		{
			(*buffer)->Disable();
		}

		// Disable the Vertex Attributes
		for ( VertexAttribList::iterator attrib = attributes.begin(); attrib != attributes.end(); attrib++ )
		{
			(*attrib)->Disable();
		}
	}

	void VertexData::Draw()
	{
		GLenum ErrorCheckValue = glGetError();

		Enable();

		//glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);

		//Wireframe
		glDrawElements(GL_LINE_LOOP, numVertices, GL_UNSIGNED_SHORT, 0);
			
		Disable();

		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("Wahhhh woooowwwwww. OpenGL error");
		}
	}
}