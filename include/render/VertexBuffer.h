#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "math/Vector.h"

namespace epsilon
{
	typedef std::list<Vector3> VectorList;
	typedef std::list<Vector4> ColourList;
	typedef std::list<int> IndicesList;
	typedef std::list<Vector2> TexCoordList;

	class VertexBuffer
	{
	private:
		struct private_struct {} ;
	public:
		typedef std::shared_ptr<VertexBuffer> Ptr;

		static VertexBuffer::Ptr Create();

		void SetData(VectorList vertices, VectorList normals, IndicesList indices, TexCoordList texCoords);
		void SetData(VectorList vertices, VectorList normals, ColourList colours, IndicesList indices, TexCoordList texCoords);

		explicit VertexBuffer(const private_struct &);
		~VertexBuffer();

		int GetVertexIndex();
		int GetNormalIndex();
		int GetColourIndex();
		int GetTexCoordIndex();

		void Enable();
		void Draw();
		void Disable();

	private:
		//bool MakeBuffers(const void *vertexData, GLsizei vertSize, const void *indicesData, GLsizei indSize);
		bool MakeBuffers(std::vector<GLfloat> vertexData, std::vector<GLshort> indicesData);

		bool hasBuffers;

		long count;

		GLuint vertexBuffer;
		GLsizei vertexBufferSize;
		GLsizei vertexBufferStride;

		GLuint indicesBuffer;
		GLsizei indicesBufferSize;

		GLuint vertexAttribIndex;
		size_t vertexStride;
		GLuint normalAttribIndex;
		size_t normalStride;
		GLuint colourAttribIndex;
		size_t colourStride;
		GLuint texCoordAttribIndex;
		size_t texCoordStride;

		// Single texture for now - actually skipping textures for now.
		GLuint textureBuffer;

		std::vector<GLfloat> vertexData;
		std::vector<GLshort> indicesData;
		/*static const GLfloat verts[] = { -1, 0,-10, 0,0,-1, 0,0,
										  0, 1,-10, 0,0,-1, 0,0,
										  1, 0,-10, 0,0,-1, 0,0 };
		
		
		static const GLshort inds[] = {0, 1, 2, 3};*/
	};

}