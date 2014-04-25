#include "render/mesh/VertexData.h"

namespace epsilon
{
    VertexData::Ptr VertexData::currentlyBound = VertexData::Ptr();
    VertexData::Ptr VertexData::Create()
	{
		return std::make_shared<VertexData>(private_struct());
	}

	VertexData::VertexData(const private_struct &)
	{
        vaoId = 0;
        bound = false;
        
		numVertices = 0;
		vertexIndex = -1;
		normalIndex = -1;
		colourIndex = -1;
		texCoordIndex = -1;

		hasIndices = false;
		buffersBuilt = false;
	}

	VertexData::~VertexData()
	{
		Destroy();
	}
    
    void VertexData::Destroy()
    {
        if ( vaoId != 0 )
        {        
            while( !buffers.empty() )
            {
                buffers.pop_back();
            }
            
            while( !attributes.empty() )
            {
                attributes.pop_back();
            }
        
            bool success;
            glDeleteVertexArrays(1, &vaoId);
            success = CheckOpenGLError("Deleting VAO");
            if ( success )
            {
                vaoId = 0;
            }
        }
    }

	VertexData::Ptr VertexData::SetVertices(VerticesAttrib::List vertexData)
	{
		VerticesAttrib * vertices = new VerticesAttrib(vertexData, VertexAttribType::VERTEX);
		vertexIndex = attributes.size();
		vertices->SetAttribIndex(vertexIndex);
		numVertices = vertices->DataLength();
		attributes.push_back( vertices );
		return ThisPtr();
	}

	VertexData::Ptr VertexData::SetNormals(NormalAttrib::List normalData)
	{
		NormalAttrib * normals = new NormalAttrib(normalData, VertexAttribType::NORMAL);
		normalIndex = attributes.size();
		normals->SetAttribIndex(normalIndex);
		attributes.push_back( normals );
		return ThisPtr();
	}

	VertexData::Ptr VertexData::SetColours(ColourAttrib::List colourData)
	{
		ColourAttrib * colours = new ColourAttrib(colourData, VertexAttribType::COLOUR);
		colourIndex = attributes.size();
		colours->SetAttribIndex(colourIndex);
		attributes.push_back( colours );
		return ThisPtr();
	}

	VertexData::Ptr VertexData::SetTexCoords(TexCoordAttrib::List texCoordData)
	{
		TexCoordAttrib * texCoords = new TexCoordAttrib(texCoordData, VertexAttribType::TEXCOORD);
		texCoordIndex = attributes.size();
		texCoords->SetAttribIndex(texCoordIndex);
		attributes.push_back( texCoords );
		return ThisPtr();
	}

	VertexData::Ptr VertexData::SetIndices(VertexIndicesBuffer::List indicesData)
	{
		buffers.push_back(new VertexIndicesBuffer(indicesData, VertexBufferType::INDICES) );
		numIndices = indicesData.size();
		hasIndices = true;
		return ThisPtr();
	}

	void VertexData::BuildBuffers()
	{
        if ( currentlyBound )
        {
            currentlyBound->Disable();
        }
        
        // Generate a VertexArrayObject
        CheckOpenGLError("Before Generating VAO");
        glGenVertexArrays(1, &vaoId);
        CheckOpenGLError("Generated VAO");
        
		// Build the vertex buffer data
		size_t stride = 0;
		VertexDataBuffer::List vertexData;

		// If the data exists from which to create attributes
		if ( attributes.size() > 0 )
		{
			int attribLength = attributes[0]->DataLength();
			
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

		buffersBuilt = true;
	}

	bool VertexData::Enable()
	{
		bool success = false;
        
        // if the VAO is not already bound
        GLint boundVAO;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
        
        bound = (boundVAO == vaoId);
        
        if ( !bound )
        {
            // Enable the VAO
            
            // Disable the current VAO if necessary
            if ( currentlyBound && (boundVAO > 0 ) )
            {
                currentlyBound->Disable();
            }
            
            glBindVertexArray(vaoId);
            CheckOpenGLError("Binding VAO");
            
            // Enable the OpenGL Buffers
            for ( VertexBufferList::iterator buffer = buffers.begin(); buffer != buffers.end(); buffer++ )
            {
                success = (*buffer)->Enable();
                
                if (!success)
                {
                    break;
                }
            }
            
            // If the buffers were successfully bound
            if (success)
            {
                // Enable the Vertex Attributes
                for (VertexAttribList::iterator attrib = attributes.begin(); attrib != attributes.end(); attrib++)
                {
                    success = (*attrib)->Enable();
                    
                    if (!success)
                    {
                        break;
                    }
                }
            }
            
            if ( success )
            {
                bound = true;
                currentlyBound = ThisPtr();
            }
            
        }
        else
        {
            // it's already bound so return success
            success = true;
        }
        
		return success;
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
        
        // Disabling the VAO
        glBindVertexArray(0);
        CheckOpenGLError("Disabling VAO");
        
        bound = false;
	}

	bool VertexData::Draw()
	{
		bool success = Enable();
		if (success)
		{
			// If drawing surfaces
			if (hasIndices)
			{
				glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
				CheckOpenGLError("DrawElements");
			}
			else
			{
				// Drawing Lines - this is a temporary hack for grids until Materials or some such are implemented.
				glDrawArrays(GL_LINES, 0, numVertices);
				CheckOpenGLError("DrawArrays");
			}
		}
        
        //Disable();
        
		return success;
	}
}