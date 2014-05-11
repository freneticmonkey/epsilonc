#include "render/material/MaterialManager.h"

namespace epsilon
{
	MaterialManager::MaterialManager()
	{

	}

	MaterialManager::~MaterialManager()
	{

	}

	void MaterialManager::Setup()
	{
        defaultShader = ShaderManager::GetInstance().GetDefault();
	}

	Material::Ptr MaterialManager::CreateMaterial()
	{
		return CreateMaterial("Material_" + std::to_string(materials.size()));
	}

	Material::Ptr MaterialManager::CreateMaterial(std::string name)
	{
		Material::Ptr newMaterial;

		// Check if material with name already exists
		if (materials.find(name) != materials.end())
		{
            Log("MaterialManager", boost::str(format("Error: Material with name: %s already exists.") % name ));
		}
		else
		{
			// Create a new material
			newMaterial = Material::Create(name);
			// Set the default shader
			newMaterial->SetShader(defaultShader);
			materials[name] = newMaterial;
		}
		return newMaterial;
	}

	Material::Ptr MaterialManager::GetMaterialByName(std::string name)
	{
		Material::Ptr foundMaterial;

		if (materials.find(name) != materials.end())
		{
			foundMaterial = materials[name];
		}
		return foundMaterial;
	}

	

	void MaterialManager::OnFrameStart()
	{
		std::for_each(materials.begin(), materials.end(), [](std::pair<std::string, Material::Ptr> material){
			material.second->OnFrameStart();
		});
	}
}