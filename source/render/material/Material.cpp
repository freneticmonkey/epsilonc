#include "render/material/Material.h"

namespace epsilon
{
	Material::Ptr Material::Create()
	{
		return std::make_shared<Material>(private_struct());
	}

	Material::Ptr Material::Create(std::string matName)
	{
		return std::make_shared<Material>(private_struct(), matName);
	}

	Material::Material(const private_struct &) : ambient(0.3f), 
												 diffuse(Colour::GREY), 
												 specular(),
												 reflectance(0.8f),
                                                 shaderReady(false)
	{
	}

	Material::Material(const private_struct &, std::string matName) : name(matName),
																	  ambient(0.3f),
																	  diffuse(Colour::GREY),
																	  specular(),
																	  reflectance(0.8f),
																	  shaderReady(false)
	{
	}

	Material::~Material(void)
	{
	}

	void Material::SetupShader()
	{
		if (shader)
		{
			// Setup the shader, compile etc
			if (!shader->Compiled())
			{
				shader->Setup();
			}

			if (shader->Compiled() && !shader->InError())
			{
				// Get the Shader's Unform values for the material
				ambientId = shader->GetUniformId("material.ambient");
				diffuseId = shader->GetUniformId("material.diffuse");
				specId	  = shader->GetUniformId("material.specular");
				reflectId = shader->GetUniformId("material.reflectance");

				shaderReady = true;
			}
			else
			{
				shaderReady = false;
			}
		}
	}

	void Material::SetShader(Shader::Ptr newShader)
	{
		shader = newShader;
		SetupShader();
	}

	Shader::Ptr Material::GetShader()
	{
		return shader;
	}

	bool Material::Enable(RenderStateStack::Ptr stateStack)
	{
		if (shader)
		{
			// If the shader hasn't yet been setup
			if (!shader->Compiled() && !shader->InError())
			{
				// Trigger the Shader setup here.  This needs to happen so that it only 
				// occurs during render in the main thread. The constructor/SetShader
				// functions are exposed to python, and therefore other threads
				SetupShader();
			}
			
			if (shaderReady)
			{
				// If the shader is ready to go
				if (shader->UseShader(stateStack))
				{
					// Set the material's colour values into the shader
					shader->SetColourUniform(ambientId, ambient);
					shader->SetColourUniform(diffuseId, diffuse);
					shader->SetColourUniform(specId, specular);
					shader->SetFloatUniform(reflectId, reflectance);
				}
			}
		}
        
        return shaderReady;
	}

	void Material::Disable()
	{
		if (shader)
		{
			shader->DisableShader();
		}
	}

}