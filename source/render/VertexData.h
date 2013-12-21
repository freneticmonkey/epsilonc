#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "math/Vector.h"

#include "render/VertexAttrib.h"
#include "render/VertexBuffer.h"

namespace epsilon
{
	
	
	typedef VertexBuffer<GLfloat> VertexDataBuffer;
	typedef VertexBuffer<GLushort> VertexIndicesBuffer;

	typedef VertexAttrib<Vector3> VerticesAttrib;
	typedef VertexAttrib<Vector3> NormalAttrib;
	typedef VertexAttrib<Vector4> ColourAttrib;
	typedef VertexAttrib<Vector2> TexCoordAttrib;

	class VertexData :
		public enable_shared_from_this<VertexData>
	{
	private:
		typedef std::vector<VertexBufferBase *> VertexBufferList;
		typedef std::vector<VertexAttribBase *> VertexAttribList;

		struct private_struct {} ;
	public:
		typedef std::shared_ptr<VertexData> Ptr;

		static VertexData::Ptr Create();

		explicit VertexData(const private_struct &);
		~VertexData();
		
		VertexData::Ptr SetVertices(VerticesAttrib::List vertexData);
		VertexData::Ptr SetNormals(NormalAttrib::List normalData);
		VertexData::Ptr SetColours(ColourAttrib::List colourData);
		VertexData::Ptr SetTexCoords(TexCoordAttrib::List texCoordData);
		VertexData::Ptr SetIndices(VertexIndicesBuffer::List indicesData);

		void BuildBuffers();
		bool HasBuiltBuffers() { return buffersBuilt; }

		int GetVertexIndex() { return vertexIndex; }
		int GetNormalIndex() { return normalIndex; }
		int GetColourIndex() { return colourIndex; }
		int GetTexCoordIndex() { return texCoordIndex; }

		void Enable();
		void Draw();
		void Disable();

	private:
		VertexData::Ptr ThisPtr() { return shared_from_this(); }

		VertexBufferList buffers;
		VertexAttribList attributes;
		int numVertices;
		int numIndices;
		int vertexIndex;
		int normalIndex;
		int colourIndex;
		int texCoordIndex;

		bool hasIndices;

		bool buffersBuilt;



	};

}