#include "render/material/Material.h"
#include "render/texture/TextureManager.h"

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

				// Get any texture uniforms for the shader
				textureUniforms = shader->GetUniformsByType(GL_SAMPLER_2D);
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

			// Bind the textures to the shader sampler uniforms
			if (textures.size() > 0)
			{
				Textures::iterator texture = textures.begin();
				GLint textureLocation = 0;

				std::for_each(textureUniforms.begin(), textureUniforms.end(), [&](ShaderUniform::Ptr textureUniform){

					//// If the current uniform is not shadowMap uniform
					//if (textureUniform->GetName() == "shadowMap")
					//{
					//	// Move to the next texture
					//	textureLocation++;
					//}
					//else
					//{
						// If there is a current texture
						if (texture != textures.end())
						{
							// Bind the texture to the location
							(*texture)->SetBindLocation(textureLocation)->Bind();

							// Set the bind point in the shader
							textureUniform->SetInt(textureLocation);

							// Move to the next texture
							textureLocation++;
							texture++;
						}
					//}
				});
			}

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

		// Unbind any textures
		std::for_each(textures.begin(), textures.end(), [&](Texture::Ptr texture){
			texture->Disable();
		});

	}

	void Material::OnFrameStart()
	{
		hasRefreshed = false;
	}

	void Material::AddTexture(Texture::Ptr newTexture)
	{
		textures.push_back(newTexture);
	}

	void Material::AddTextureByName(std::string textureName)
	{
		Texture::Ptr newTexture = TextureManager::GetInstance().GetTextureByName(textureName);
		if (newTexture)
		{
			textures.push_back(newTexture);
		}
	}

	void Material::AddTextureByPath(std::string texturePath)
	{
		Texture::Ptr newTexture = TextureManager::GetInstance().GetTextureByPath(texturePath);
		if (newTexture)
		{
			textures.push_back(newTexture);
		}
	}

}