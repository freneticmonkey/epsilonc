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

	Renderer::Ptr Renderer::Create(Mesh::Ptr newMesh, Material::Ptr newMaterial)
	{
		return make_shared<Renderer>(private_struct(), newMesh, newMaterial);
	}

	Renderer::Renderer(const private_struct &) : NodeComponent("Renderer")
	{
		mesh = Mesh::Create();
		material = Material::Create();
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh) : NodeComponent("Renderer")
	{
		mesh = newMesh;
		material = Material::Create();
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh, Material::Ptr newMaterial) : NodeComponent("Renderer")
	{
		mesh = newMesh;
		material = newMaterial;
	}

	Renderer::~Renderer(void)
	{
	}

	void Renderer::Draw(RenderStateStack::Ptr stateStack)//Matrix4 viewMatrix, Matrix4 projMatrix)
	{
		if ( material )
		{
			// Set the transform here as this is a NodeComponent, whereas a single material
			// maybe attached to multiple objects.  This concept will allow for multiple objects
			// to share a material and therefore shader instance.

			// This paves the way for a Material Manager in which the renderer requests a material
			// rather than creating it directly itself.

			// Make this only update if the transform has changed
			stateStack->State()->model = GetParent()->GetComponent<Transform>()->_getFullTransform();

			// Send the state to the applied material
			material->Enable(stateStack);
		}

		if ( mesh )
		{
			mesh->Draw();
		}

		if ( material )
		{
			material->Disable();
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

	void Renderer::SetMaterial(Material::Ptr newMaterial)
	{
		material = newMaterial;
	}

	Material::Ptr Renderer::GetMaterial()
	{
		return material;
	}
}