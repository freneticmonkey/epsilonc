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
	}

	Material::~Material(void)
	{
	}

	void Material::SetupShader()
	{
		// Set the GLSL definition of the material in the shader
		shader->SetMaterialFile("resources/shaders/material.frag");

		// Compile the shader
		shader->Setup();

		// Get the Shader's Unform values for the material
		ambientId = shader->GetUniformId("material.ambient");
		diffuseId = shader->GetUniformId("material.diffuse");
		specId = shader->GetUniformId("material.specular");
		reflectId = shader->GetUniformId("material.reflectance");
	}

	void Material::SetShader(Shader::Ptr newShader)
	{
		shader = newShader;
	}

	Shader::Ptr Material::GetShader()
	{
		return shader;
	}

	void Material::Enable(RenderStateStack::Ptr stateStack)
	{
		// If the shader hasn't yet been setup
		if ( !shader->Compiled() )
		{
			// Trigger the Shader setup here.  This needs to happen so that it only 
			// occurs during render in the main thread. The constructor/SetShader
			// functions are exposed to python, and therefore other threads
			SetupShader();
		}

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