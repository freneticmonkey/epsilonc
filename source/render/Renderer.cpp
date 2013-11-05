#include "render/Renderer.h"

namespace epsilon
{
	Renderer::Ptr Renderer::Create()
	{
		return make_shared<Renderer>(private_struct());
	}

	Renderer::Ptr Renderer::Create(Mesh::Ptr newMesh)
	{
		return make_shared<Renderer>(private_struct(), newMesh);
	}

	Renderer::Renderer(const private_struct &) : NodeComponent("Renderer")
	{
		mesh = Mesh::Create();
		shader = Shader::Create();
		shader->Setup();
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh) : NodeComponent("Renderer")
	{
		mesh = newMesh;
		shader = Shader::Create();
		shader->Setup();
	}

	Renderer::~Renderer(void)
	{
	}

	void Renderer::Draw(Matrix4 viewMatrix, Matrix4 projMatrix)
	{
		if ( shader )
		{
			// This needs to be finished after node component refactoring is complete.
			Transform::Ptr transform = GetParent()->GetComponent<Transform>();
			if ( transform )
			{
				shader->UseShader(transform, viewMatrix, projMatrix);
			}
			else
			{
				shader->UseShader();
			}
		}

		if ( mesh )
		{
			mesh->Draw();
		}
	}

	void Renderer::SetMesh(Mesh::Ptr newMesh)
	{
		mesh = newMesh;
	}

	Mesh::Ptr Renderer::GetMesh()
	{
		return mesh;
	}

	void Renderer::SetShader(Shader::Ptr newShader)
	{
		shader = newShader;
	}

	Shader::Ptr Renderer::GetShader()
	{
		return shader;
	}
	
}