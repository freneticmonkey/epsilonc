#include "render/Mesh.h"

namespace epsilon
{
	Mesh::Ptr Mesh::Create()
	{
		return std::make_shared<Mesh>(private_struct());
	}

	Mesh::Mesh(const private_struct &)
	{
		vb = VertexBuffer::Create();
	}

	Mesh::~Mesh(void)
	{
	}

	void Mesh::SetMeshData(VectorList vertices, VectorList normals, TexCoordList texCoords, IndicesList indices)
	{
		vb->SetData(vertices, normals, indices, texCoords);
	}

	void Mesh::SetMeshData(VectorList vertices, VectorList normals, ColourList colours, TexCoordList texCoords, IndicesList indices)
	{
		vb->SetData(vertices, normals, colours, indices, texCoords);
	}

	void Mesh::Draw()
	{
		vb->Draw();
	}

}