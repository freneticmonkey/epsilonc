#include "render/Mesh.h"

namespace epsilon
{
	Mesh::Ptr Mesh::Create()
	{
		return std::make_shared<Mesh>(private_struct());
	}

	Mesh::Mesh(const private_struct &) : drawOk(true)
	{
		vd = VertexData::Create();
	}

	Mesh::~Mesh(void)
	{
	}
    
    void Mesh::Destroy()
    {
        if ( vd->HasBuiltBuffers())
        {
            vd->Destroy();
        }
    }
	
	bool Mesh::Draw()
	{
		if ( !vd->HasBuiltBuffers() )
		{
			vd->BuildBuffers();
		}
		drawOk = vd->Draw();
		return drawOk;
	}

}