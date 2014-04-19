#include "render/Renderer.h"
#include "render/material/MaterialManager.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	Renderer::Ptr Renderer::Create()
	{
		return std::make_shared<Renderer>(private_struct());
	}

	Renderer::Ptr Renderer::Create(Mesh::Ptr newMesh)
	{
		return std::make_shared<Renderer>(private_struct(), newMesh);
	}

	Renderer::Ptr Renderer::Create(Mesh::Ptr newMesh, Material::Ptr newMaterial)
	{
		return std::make_shared<Renderer>(private_struct(), newMesh, newMaterial);
	}

	Renderer::Renderer(const private_struct &) : NodeComponent("Renderer")
	{
		mesh = Mesh::Create();
		//material = MaterialManager::GetInstance().GetMaterialByName("default";
        std::string materialName = boost::str(format("Material_%d") % GetId());
		material = MaterialManager::GetInstance().CreateMaterial(materialName);
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh) : NodeComponent("Renderer")
	{
		mesh = newMesh;
		//material = MaterialManager::GetInstance().GetMaterialByName("default");
		std::string materialName = boost::str(format("Material_%d") % GetId());
		material = MaterialManager::GetInstance().CreateMaterial(materialName);
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh, Material::Ptr newMaterial) : NodeComponent("Renderer")
	{
		mesh = newMesh;
		material = newMaterial;
	}

	Renderer::~Renderer(void)
	{
	}
    
    void Renderer::Destroy()
    {
        mesh->Destroy();
    }

	void Renderer::Draw(RenderStateStack::Ptr stateStack)//Matrix4 viewMatrix, Matrix4 projMatrix)
	{
        // if a Material and a Mesh have been set. i.e. there is something to render and we know how to render it.
		if ( material && mesh)
		{
			// Set the transform here as this is a NodeComponent, whereas a single material
			// maybe attached to multiple objects.  This concept will allow for multiple objects
			// to share a material and therefore shader instance.

			// This paves the way for a Material Manager in which the renderer requests a material
			// rather than creating it directly itself.

			// Make this only update if the transform has changed
            Matrix4 m = GetParent()->GetComponent<Transform>()->_getFullTransform();
			stateStack->State()->model = m;

			// Enable the Material by sending the state to the applied material
            if ( material->Enable(stateStack) )
            {
                mesh->Draw();
            }
            
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