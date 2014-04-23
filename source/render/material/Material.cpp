#include "render/material/Material.h"
#include <algorithm>

#include <boost/format.hpp>

using namespace boost;

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
												 shaderReady(false),
												 hasRefreshed(false),
												 shaderCompileVersion(0)
	{
	}

	Material::Material(const private_struct &, std::string matName) : name(matName),
																	  ambient(0.3f),
																	  diffuse(Colour::GREY),
																	  specular(),
																	  reflectance(0.8f),
																	  shaderReady(false),
																	  hasRefreshed(false),
																	  shaderCompileVersion(0)
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

			// Get the Shader's uniforms
			if ( shader->Compiled() && 
				 // If the shader isn't in error
				 !shader->InError() )
			{
				// Get the Shader's Unform values for the material
				ambientUniform = shader->GetUniform("material.ambient");
				diffuseUniform = shader->GetUniform("material.diffuse");
				specularUniform = shader->GetUniform("material.specular");
				reflectanceUniform = shader->GetUniform("material.reflectance");
                
                // Get the Shader's uniform values for the transform
                modelUniform = shader->GetUniform("modelMatrix");
                
				shaderCompileVersion = shader->GetCompileVersion();
				hasRefreshed = true;
			}
		}
	}

	void Material::SetShader(Shader::Ptr newShader)
	{
		// If setting the same shader object ignore the set.
		shader = newShader;
		SetupShader();
	}

	Shader::Ptr Material::GetShader()
	{
		return shader;
	}

	bool Material::Enable(const Matrix4 & modelMatrix)
	{
		if (shader)
		{
			// Get the Shader's uniforms
			if (!shader->Compiled() ||
				// and the shader's compile version isn't the last version we checked.
				(shaderCompileVersion < shader->GetCompileVersion()))
			{
				// Trigger the Shader setup here.  This needs to happen so that it only 
				// occurs during render in the main thread. The constructor/SetShader
				// functions are exposed to python, and therefore other threads
				SetupShader();
			}

			// Set the material's colour values into the shader
			if (ambientUniform)
				ambientUniform->SetVector4(ambient.ToVector4());

			if (diffuseUniform)
				diffuseUniform->SetVector4(diffuse.ToVector4());

			if (specularUniform)
				specularUniform->SetVector4(specular.ToVector4());

			if (reflectanceUniform)
				reflectanceUniform->SetFloat(reflectance);
            
            // Set Transform
            if (modelUniform)
				modelUniform->SetMatrix4(modelMatrix);

			// Push the shader variables into the Shader on the GPU
			shaderReady = shader->UseShader();
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

	void Material::OnFrameStart()
	{
		hasRefreshed = false;
	}

}