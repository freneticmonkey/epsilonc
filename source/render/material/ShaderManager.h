#pragma once
//
//  ShaderManager.h
//  Epsilon
//
//  Created by Scott Porter on 21/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/material/Shader.h"
#include "render/material/UniformBuffer.h"

namespace epsilon {
    
    class ShaderManager
    {
    private:
        ShaderManager();
        
    public:
		typedef std::map<std::string, Shader::Ptr>          ShaderList;
        typedef std::map<std::string, UniformBuffer::Ptr>   UniformBuffers;
        
        static ShaderManager & GetInstance()
		{
			static ShaderManager instance;
			return instance;
		}
		~ShaderManager();
        
        void        Setup();
        void        ProcessUniformBuffers();
        
		Shader::Ptr	GetShaderByName(std::string name);
        Shader::Ptr GetDefault() { return defaultShader; }
        
        UniformBuffer::Ptr GetUniformBuffer(std::string name);
        
        
    private:
		ShaderList  shaders;
        Shader::Ptr defaultShader;
        
        UniformBuffers uniformBuffers;
        GLuint         uniformBufferId;
    };
    
    void Setup();
}
