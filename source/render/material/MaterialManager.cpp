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
		// Search the ResourceManager for all files with the extension "shader"
		ResourceList results = ResourceManager::GetInstance().FindResources(".*\.shader");
		
		// For each result returned
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
			Shader::Ptr newShader = Shader::CreateFromDefinition(resource->GetFilepath().GetString());

			// ignore shaders with duplicate names
			if (shaders.find(newShader->GetName()) != shaders.end())
			{
				Log("MaterialManager", "Ignoring shader with duplicate name: " + newShader->GetName());
			}
			else
			{
				shaders[newShader->GetName()] = newShader;

				// Just add new materials
				Material::Ptr newMaterial = Material::Create();
				newMaterial->SetShader(newShader);

				// Set the name of the material to the shader name
				std::string shaderName = newShader->GetName();
				std::vector<std::string> pair = split(shaderName, '.');
				if (pair.size() == 2)
				{
					std::string name = pair[0];

					materials[name] = newMaterial;
				}
			}
			
		});
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

	Shader::Ptr MaterialManager::GetShaderByName(std::string name)
	{
		Shader::Ptr foundShader;
		if (shaders.find(name) != shaders.end())
		{
			foundShader = shaders[name];
		}
		return foundShader;
	}
}