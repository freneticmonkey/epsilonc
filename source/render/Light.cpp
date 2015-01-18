//
//  Light.cpp
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//

#include "render/Light.h"

#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;

	Light::Ptr Light::Create(int newID)
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct(), newID);
		return newLight;
	}
    
	Light::Ptr Light::Create(int newID, std::string name)
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct(), newID, name);
		return newLight;
	}
    
	Light::Light(const private_struct &, int newID) : NodeComponent("Light"), strength(10), 
																			  id(newID),
																			  type(0),
																			  spotCutoff(0.1f),
																			  spotExponent(2),
																			  shadowType(ShadowType::NONE),
																			  shadowsSetup(false),
																			  shadowsFailed(false),
																			  FramebufferName(0),
																			  depthTexture(0)
	{
		Setup();
	}
    
	Light::Light(const private_struct &, int newID, std::string name) : NodeComponent(name, "Light"), strength(10), 
																									  id(newID),
																									  type(0),
																									  spotCutoff(0.1f),
																									  spotExponent(2),
																									  shadowType(ShadowType::NONE),
																									  shadowsSetup(false),
																									  shadowsFailed(false),
																									  FramebufferName(0),
																									  depthTexture(0)
	{
		Setup();
	}
    
	void Light::Setup()
	{
		UniformBuffer::Ptr lights = ShaderManager::GetInstance().GetUniformBuffer("Lights");

		// Grab this light's uniforms from the Uniform Buffer.
		positionUnf		= lights->GetUniform(boost::str(format("lights[%d].position") % id));
		directionUnf = lights->GetUniform(boost::str(format("lights[%d].direction") % id));

		ambientUnf = lights->GetUniform(boost::str(format("lights[%d].ambient") % id));
		diffuseUnf = lights->GetUniform(boost::str(format("lights[%d].diffuse") % id));
		specularUnf = lights->GetUniform(boost::str(format("lights[%d].specular") % id));
		
		attenuationUnf = lights->GetUniform(boost::str(format("lights[%d].attenuation") % id));
		
		strengthUnf = lights->GetUniform(boost::str(format("lights[%d].strength") % id));
		spotCutoffUnf = lights->GetUniform(boost::str(format("lights[%d].spotCutoff") % id));
		spotExponentUnf = lights->GetUniform(boost::str(format("lights[%d].spotExponent") % id));

		typeUnf = lights->GetUniform(boost::str(format("lights[%d].type") % id));

		// Get the depth matrix from the ShadowMatrix UniformBuffer
		UniformBuffer::Ptr shadowMatrix = ShaderManager::GetInstance().GetUniformBuffer("ShadowMatrices");
		shadowMatrixUnf = shadowMatrix->GetUniform("depthModelViewProjection");
		shadowDepthBiasUnf = shadowMatrix->GetUniform("depthBias");

		// Grab the Global Matrices Uniform so that we can build the depth matrix in the shader
		// by overridding the view and proj matrices.
		UniformBuffer::Ptr globalMatrix = ShaderManager::GetInstance().GetUniformBuffer("GlobalMatrices");
		viewMatrixUnf = globalMatrix->GetUniform("viewMatrix");
		projMatrixUnf = globalMatrix->GetUniform("projectionMatrix");

		// Hacky as fuuuuccckkkk
		Shader::Ptr phong = ShaderManager::GetInstance().GetShaderByName("phong.shader");
		if (phong)
		{
			shadowDepthTexture = phong->GetUniform("shadowMap");
		}
	}

	void Light::SetupShadows()
	{
		// Ensure that this is only called once.
		if (!shadowsSetup && !shadowsFailed)
		{
			Shader::Ptr phong = ShaderManager::GetInstance().GetShaderByName("phong.shader");
			if (phong)
			{
				shadowDepthTexture = phong->GetUniform("shadowMap");
			}

			materialManager = &MaterialManager::GetInstance();

			// Get the shadow material
			shadowMaterial = materialManager->GetMaterialByName("shadow");

			// Get a frame buffer
			glGenFramebuffers(1, &FramebufferName);

			// Bind the shadow depth texture
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
			
			// Get a Depth texture. Slower than a depth buffer, but you can sample it later in your shader
			glGenTextures(1, &depthTexture);

			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				Log("There was a problem setting up the shadow framebuffer for Light: " + GetName());
				shadowsFailed = true;
			}

			glDrawBuffer(GL_NONE); // No color buffer is drawn to.

			shadowsSetup = true;
		}
	}
    
    void Light::OnSetParent()
    {
        transform = componentParent->GetComponent<Transform>();
    }

	void Light::OnDestroy()
	{

	}
    
    Vector3 Light::GetPosition()
    {
        return transform->GetPosition();
    }
    
    Vector3 Light::GetDirection()
    {
        return transform->Forward();
    }
    
	void Light::PreRender(Renderers renderItems)
	{	
		// Process Shadow
		if (shadowType != ShadowType::NONE)
		{
			// Point lights can't cast shadows.
			if (type != Type::POINT)
			{
				// Just one type of shadow for now.

				if (!shadowsSetup)
				{
					SetupShadows();
				}

				if (!shadowsFailed)
				{
					// Bind the shadow depth texture
					glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
					glViewport(0, 0, 1024, 1024);

					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					glBindTexture(GL_TEXTURE_2D, depthTexture);

					// Build Depth Matrix
					Matrix4 projMatrix = Matrix4::CreateOrthographic(-10, 10, -10, 10, -10, 20);
					//Vector3 target = GetPosition() - GetDirection();
					Vector3 target = Vector3(0,2,10) - GetDirection();
					Matrix4 viewMatrix = Matrix4::CreateLookAt(GetPosition(), target, Vector3::UP);
					Matrix4 model;
					Matrix4 depthMatrix = projMatrix * viewMatrix * model;

					Matrix4 depthBias(0.5f, 0.0f, 0.0f, 0.5f, 
									  0.0f, 0.5f, 0.0f, 0.5f,
									  0.0f, 0.0f, 0.5f, 0.5f,
									  0.0f, 0.0f, 0.0f, 1.0f);

					Matrix4 result = depthBias * depthMatrix;

					if (viewMatrixUnf)
						viewMatrixUnf->SetMatrix4(viewMatrix);

					if (projMatrixUnf)
						projMatrixUnf->SetMatrix4(projMatrix);

					if (shadowMatrixUnf)
						shadowMatrixUnf->SetMatrix4(depthMatrix);
					
					if (shadowDepthBiasUnf)
						shadowDepthBiasUnf->SetMatrix4(result);

					// Flush the matrix to the GPU
					ShaderManager::GetInstance().ProcessUniformBuffers();

					//Configure the shadow material
					//shadowMaterial->Enable(model);

					// Draw each of visible items with the shadow material.
					std::for_each(renderItems.begin(), renderItems.end(), [&](Renderer::Ptr renderer){
						renderer->Draw(shadowMaterial);
					});

					//shadowMaterial->Disable();
				}

				// Unbind shadow framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, 800, 600);

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Bind the Shadow Texture to Texture 0
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthTexture);

				// Totally hardcoding the shadow map to the first texture for all shaders. suck it.
				if (shadowDepthTexture )
					shadowDepthTexture->SetInt(2);
			}
		}

		// Push Data to Uniform Buffer if uniforms were successfully found.

		if (positionUnf)
		{
			positionUnf->SetVector3(GetPosition());
		}

		if (directionUnf)
		{
			directionUnf->SetVector3(GetDirection());
		}

		if (ambientUnf)
		{
			ambientUnf->SetVector4(ambient.ToVector4());
		}

		if (diffuseUnf)
		{
			diffuseUnf->SetVector4(diffuse.ToVector4());
		}

		if (specularUnf)
		{
			specularUnf->SetVector4(specular.ToVector4());
		}

		if (attenuationUnf)
		{
			attenuationUnf->SetVector3(attenuation);
		}

		if (strengthUnf)
		{
			strengthUnf->SetFloat(strength);
		}

		if (spotCutoffUnf)
		{
			spotCutoffUnf->SetFloat(spotCutoff);
		}

		if (spotExponentUnf)
		{
			spotExponentUnf->SetFloat(spotExponent);
		}

		if (typeUnf)
		{
			typeUnf->SetInt(type);
		}
	}

	void Light::PostRender()
	{
		// Process Shadow
		if (shadowType != ShadowType::NONE)
		{
			if (!shadowsFailed)
			{
				glActiveTexture(GL_TEXTURE0);
				// Unbind shadow depth texture
				glBindTexture(GL_TEXTURE_2D, 0);

				

			}
		}
	}
}
