#pragma once

#include "EpsilonCore.h"

#include "render/VertexData.h"

namespace epsilon
{
	class Mesh
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Mesh> Ptr;

		static Mesh::Ptr Create();
		
		VertexData::Ptr VertexData() { return vd; }

		/*
		void SetMeshData(VectorData::List vertices, VectorList normals, TexCoordList texCoords, IndicesList indices);
		void SetMeshData(VectorList vertices, VectorList normals, ColourList colours, TexCoordList texCoords, IndicesList indices);
		*/

		void Draw();

		explicit Mesh(const private_struct &);
		~Mesh(void);

	private:
		VertexData::Ptr vd;
	};

}