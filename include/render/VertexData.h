#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "math/Vector.h"

#include "render/VertexAttrib.h"
#include "render/VertexBuffer.h"

namespace epsilon
{
	
	typedef std::vector<Vector3> VectorList;
	typedef std::vector<Vector4> ColourList;
	typedef std::vector<GLshort> IndicesList;
	typedef std::vector<Vector2> TexCoordList;
	
	typedef std::vector<VertexBufferBase *> VertexBufferList;

	typedef VertexBuffer<GLfloat> VertexDataBuffer;
	typedef VertexBuffer<GLshort> VertexIndicesBuffer;

	typedef std::vector<VertexAttribBase *> VertexAttribList;

	typedef VertexAttrib<Vector3> VerticesData;
	typedef VertexAttrib<Vector3> NormalData;
	typedef VertexAttrib<Vector4> ColourData;
	typedef VertexAttrib<Vector2> TexCoordData;

	class VertexData
	{
	private:
		struct private_struct {} ;
	public:
		typedef std::shared_ptr<VertexData> Ptr;

		static VertexData::Ptr Create();

		explicit VertexData(const private_struct &);
		~VertexData();
		
		VertexData& SetVertices(VerticesData::List vertexData);
		VertexData& SetNormals(NormalData::List normalData);
		VertexData& SetColours(ColourData::List colourData);
		VertexData& SetTexCoords(TexCoordData::List texCoordData);
		VertexData& SetIndices(VertexIndicesBuffer::BufferList indicesData);

		void BuildBuffers();

		int GetVertexIndex() { return vertexIndex; }
		int GetNormalIndex() { return normalIndex; }
		int GetColourIndex() { return colourIndex; }
		int GetTexCoordIndex() { return texCoordIndex; }

		void Enable();
		void Draw();
		void Disable();

	private:
		VertexBufferList buffers;
		VertexAttribList attributes;
		int numVertices;
		int vertexIndex;
		int normalIndex;
		int colourIndex;
		int texCoordIndex;

	};

}