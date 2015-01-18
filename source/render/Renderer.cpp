#include "render/Renderer.h"
#include "render/material/MaterialManager.h"
#include "render/mesh/MeshManager.h"

#include "scene/Transform.h"

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
		SetMesh(newMesh);
		//material = MaterialManager::GetInstance().GetMaterialByName("default");
		std::string materialName = boost::str(format("Material_%d") % GetId());
		material = MaterialManager::GetInstance().CreateMaterial(materialName);
	}

	Renderer::Renderer(const private_struct &, Mesh::Ptr newMesh, Material::Ptr newMaterial) : NodeComponent("Renderer")
	{
		SetMesh(newMesh);
		material = newMaterial;
	}

	Renderer::~Renderer(void)
	{
	}
    
    void Renderer::OnDestroy()
    {
		// Not sure that we actually want to destroy the mesh here.
        //mesh->Destroy();
    }

	void Renderer::Draw()
	{
        // if a Material and a Mesh have been set. i.e. there is something to render and we know how to render it.
		if ( material && mesh)
		{
			// Set the transform here as this is a NodeComponent, whereas a single material
			// maybe attached to multiple objects.  This concept will allow for multiple objects
			// to share a material and therefore shader instance.

			// This paves the way for a Material Manager in which the renderer requests a material
			// rather than creating it directly itself.

			// Enable the Material by sending the state to the applied material
			if (material->Enable(GetParent()->GetComponent<Transform>()->_getFullTransform()))
            {
                mesh->Draw();
            }
            
            material->Disable();
		}
	}

	void Renderer::Draw(Material::Ptr drawMaterial)
	{
		if (drawMaterial && mesh)
		{
			if (drawMaterial->Enable(GetParent()->GetComponent<Transform>()->_getFullTransform()))
			{
				mesh->Draw();
			}
		}
	}

	void Renderer::SetMesh(Mesh::Ptr newMesh)
	{
		mesh = newMesh;
		UpdateBounds();
	}

	void Renderer::SetMeshByName(std::string meshName)
	{
		Mesh::Ptr newMesh = MeshManager::GetInstance().GetMeshByName(meshName);
		if (newMesh)
		{
			SetMesh(newMesh);
		}
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

	void Renderer::UpdateBounds()
	{
		// Update the Mesh Bounds of a Transform attached to the SceneNode parent
		Transform::Ptr trans = GetParent()->GetComponent<Transform>();
		if ( trans )
			trans->SetMeshBounds(mesh->GetBounds());
	}
}