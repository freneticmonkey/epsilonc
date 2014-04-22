//
//  ShaderManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 21/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/material/ShaderManager.h"
#include "resource/ResourceManager.h"
#include "render/material/MaterialManager.h"

namespace epsilon {

    ShaderManager::ShaderManager() : uniformBufferId(0)
    {
        
    }
    
    ShaderManager::~ShaderManager()
    {
        
    }
    
    void ShaderManager::Setup()
    {
        // Search the ResourceManager for all files with the extension "shader"
		ResourceList results = ResourceManager::GetInstance().FindResources(".*\.shader");
		
		// For each result returned
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
			Shader::Ptr newShader = Shader::CreateFromDefinition(resource->GetFilepath().GetString());
            
			// ignore shaders with duplicate names
			if (shaders.find(newShader->GetName()) != shaders.end())
			{
				Log("ShaderManager", "Ignoring shader with duplicate name: " + newShader->GetName());
			}
			else
			{
				shaders[newShader->GetName()] = newShader;
                
                if ( newShader->GetName() == "default.shader" )
                {
                    defaultShader = newShader;
                }

				// Create new materials for each of the shaders found. 
				// Materials will be named using the shader without the '.shader' extension
				std::string materialName = newShader->GetName();
				materialName = materialName.substr(0, materialName.find("."));
				Material::Ptr shaderMaterial = MaterialManager::GetInstance().CreateMaterial(materialName);
				shaderMaterial->SetShader(newShader);
			}
		});
    }
    
    Shader::Ptr ShaderManager::GetShaderByName(std::string name)
	{
		Shader::Ptr foundShader;
		if (shaders.find(name) != shaders.end())
		{
			foundShader = shaders[name];
		}
		return foundShader;
	}
    
    UniformBuffer::Ptr ShaderManager::GetUniformBuffer(std::string name)
    {
        UniformBuffer::Ptr newBuffer;
        
        UniformBuffers::iterator uniformBuff = uniformBuffers.find(name);
        if ( uniformBuff != uniformBuffers.end() )
        {
            newBuffer = uniformBuff->second;
        }
        else
        {
            // Create a new buffer!
            newBuffer = UniformBuffer::Create(name, uniformBufferId++);
            uniformBuffers[name] = newBuffer;
        }
        
        return newBuffer;
    }
    
    void ShaderManager::ProcessUniformBuffers()
    {
        std::for_each(uniformBuffers.begin(), uniformBuffers.end(), [](std::pair<std::string, UniformBuffer::Ptr> buffer){
			if ( !buffer.second->IsBound() )
            {
                buffer.second->Bind();
            }
            buffer.second->Update();
		});
    }
}
