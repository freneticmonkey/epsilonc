#pragma once

#include "EpsilonCore.h"

#include "render/VertexBuffer.h"

namespace epsilon
{
	class Mesh
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Mesh> Ptr;

		static Mesh::Ptr Create();
		
		void SetMeshData(VectorList vertices, VectorList normals, TexCoordList texCoords, IndicesList indices);
		void SetMeshData(VectorList vertices, VectorList normals, ColourList colours, TexCoordList texCoords, IndicesList indices);

		void Draw();

		explicit Mesh(const private_struct &);
		~Mesh(void);

	private:
		VertexBuffer::Ptr vb;
	};

}