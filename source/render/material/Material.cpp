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
                viewUniform = shader->GetUniform("viewMatrix");
                projectionUniform = shader->GetUniform("projectionMatrix");

				// If possible set the uniforms for the light struct
                /*
				for (int i = 0; i < MAX_LIGHTS; i++)
				{
					// Try to get the first light uniform
					LightUniforms uniforms;
					uniforms.position = shader->GetUniform(boost::str(format("lights[%d].position") % i));

					// If successfull get the rest and keep track of it.
					if (uniforms.position)
					{
						uniforms.direction = shader->GetUniform(boost::str(format("lights[%d].direction") % i));
						uniforms.diffuse = shader->GetUniform(boost::str(format("lights[%d].diffuse") % i));
						uniforms.attenuation = shader->GetUniform(boost::str(format("lights[%d].attenuation") % i));
						lightUniforms.push_back(uniforms);
					}
				}
                */
                // Try to get the first light uniform
                LightUniforms uniforms;
                uniforms.position = shader->GetUniform(boost::str(format("light.position")));
                
                // If successfull get the rest and keep track of it.
                if (uniforms.position)
                {
                    uniforms.direction = shader->GetUniform(boost::str(format("light.direction") ));
                    uniforms.diffuse = shader->GetUniform(boost::str(format("light.diffuse") ));
                    uniforms.attenuation = shader->GetUniform(boost::str(format("light.attenuation") ));
                    lightUniforms.push_back(uniforms);
                }
                
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

	bool Material::Enable(RenderStateStack::Ptr stateStack)
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
			
			//if (!shader->InError())
			//{
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
                    modelUniform->SetMatrix4(stateStack->State()->model);
            
                if (viewUniform)
                    viewUniform->SetMatrix4(stateStack->State()->view);
                
                if (projectionUniform)
                    projectionUniform->SetMatrix4(stateStack->State()->projection);

				// Inject the lighting info into the shader
				LightList lights = stateStack->State()->lights;

				for (int i = 0; i < std::min((int)lights.size(), MAX_LIGHTS); i++)
				{
					if (i < (int)lightUniforms.size())
					{
						if (lightUniforms[i].position )
							lightUniforms[i].position->SetVector3(lights[i]->GetPosition());

						if (lightUniforms[i].direction)
							lightUniforms[i].direction->SetVector3(lights[i]->GetDirection());

						if (lightUniforms[i].diffuse)
							lightUniforms[i].diffuse->SetVector4(lights[i]->diffuse.ToVector4());

						if (lightUniforms[i].attenuation)
							lightUniforms[i].attenuation->SetVector3(lights[i]->attenuation);
					}
				}

				// Push the shader variables into the Shader on the GPU
				shaderReady = shader->UseShader(stateStack);
				/*
			}
			else
			{
				shaderReady = false;
			}
			*/
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