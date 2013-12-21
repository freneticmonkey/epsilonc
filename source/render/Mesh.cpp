#include "render/Mesh.h"

namespace epsilon
{
	Mesh::Ptr Mesh::Create()
	{
		return std::make_shared<Mesh>(private_struct());
	}

	Mesh::Mesh(const private_struct &)
	{
		vd = VertexData::Create();
	}

	Mesh::~Mesh(void)
	{
	}
	
	void Mesh::Draw()
	{
		if ( !vd->HasBuiltBuffers() )
		{
			vd->BuildBuffers();
		}
		vd->Draw();
	}

}