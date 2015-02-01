#include "render/mesh/VertexData.h"

namespace epsilon
{
    VertexData::Ptr VertexData::currentlyBound = VertexData::Ptr();
	VertexData::Ptr VertexData::Create(GLenum drawType)
	{
		return std::make_shared<VertexData>(private_struct(), drawType);
	}

	VertexData::VertexData(const private_struct &, GLenum type)
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
		drawType = type;
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

		// Calculate Bounds
		Vector3 min;
		Vector3 max;

		if (vertexData.size() > 0)
		{
			min = vertexData[0];
			max = vertexData[0];

			std::for_each(vertexData.begin(), vertexData.end(), [&](Vector3 value){
				if (value.x < min.x)
					min.x = value.x;
				if (value.y < min.y)
					min.y = value.y;
				if (value.z < min.z)
					min.z = value.z;

				if (value.x > max.x)
					max.x = value.x;
				if (value.y > max.y)
					max.y = value.y;
				if (value.z > max.z)
					max.z = value.z;
			});
		}

		vertexBounds = Bounds(min, max);

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

	void VertexData::GenIndicesBuffer()
	{
		// Naively Generate an index buffer using the mesh's draw type
		// This assumes because an index buffer hasn't been provided
		// duplicate vertices and therefore attributes are in use.

		VertexIndicesBuffer::List indicesData;
		//int primitiveVerts = 0;
		//switch (drawType)
		//{
		//	case GL_TRIANGLES:
		//		primitiveVerts = 3;
		//		break;
		//	case GL_LINES:
		//		primitiveVerts = 2;
		//		break;
		//	// Add quad etc etc here later
		//	default:
		//		break;
		//}
        
        // If there are vertices defined. 
        if (vertexIndex > -1)
        {
            // For each vertex
            GLushort count = 0;
            for (int i = 0; i < attributes[vertexIndex]->DataLength(); i++)
            {
                indicesData.push_back(count++);
            }
            SetIndices(indicesData);            
        }

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
        
		// If an index buffer doesn't exist, build an index buffer
		if (!hasIndices)
		{
			GenIndicesBuffer();
		}

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
        if( bound )
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
        }
        
        bound = false;
	}

	bool VertexData::Draw()
	{
		bool success = Enable();
		if (success)
		{
			glDrawElements(drawType, numIndices, GL_UNSIGNED_SHORT, 0);
			CheckOpenGLError("DrawElements");
		}
        
        Disable();
        
		return success;
	}
}