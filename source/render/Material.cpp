#include "render/Material.h"

namespace epsilon
{
	Material::Ptr Material::Create()
	{
		return std::make_shared<Material>(private_struct());
	}

	Material::Material(const private_struct &) : ambient(0.3f), 
												 diffuse(Colour::GREY), 
												 specular(),
												 reflectance(0.8f)
	{
		shader = Shader::Create();
		SetupShader();
	}

	Material::~Material(void)
	{
	}

	void Material::SetupShader()
	{
		shader->SetMaterialDef(readfile("resources/shaders/material.frag"));
		shader->Setup();

		// Get the Unform values for the material
		ambientId = shader->GetUniformId("material.ambient");
		diffuseId = shader->GetUniformId("material.diffuse");
		specId = shader->GetUniformId("material.specular");
		reflectId = shader->GetUniformId("material.reflectance");
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

	void Material::Enable(RenderStateStack::Ptr stateStack)
	{
		// If the shader is ready to go
		if ( shader->UseShader(stateStack) )
		{
			// Set the material's colour values into the shader
			shader->SetColourUniform(ambientId, ambient);
			shader->SetColourUniform(diffuseId, diffuse);
			shader->SetColourUniform(specId, specular);
			shader->SetFloatUniform(reflectId, reflectance);
		}
	}

	void Material::Disable()
	{
		shader->DisableShader();
	}

}