#pragma once

//
//  UniformBuffer.h
//  Epsilon
//
//  Created by Scott Porter on 21/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/material/ShaderUniform.h"
#include <vector>

namespace epsilon
{
    class UniformBuffer :
        public std::enable_shared_from_this<UniformBuffer>
    {
    private:
        struct private_struct {};
    public:

		// Max # of Block Indices
		static const int MAX_BLOCK_INDICIES = 128;

        typedef std::shared_ptr<UniformBuffer> Ptr;
		typedef std::map<std::string, ShaderUniform::Ptr> UniformMap;
        
        static UniformBuffer::Ptr Create(std::string name, GLuint index)
		{
			return std::make_shared<UniformBuffer>(private_struct(), name, index);
		}
        
        explicit UniformBuffer(const private_struct &, std::string name, GLuint index);
        ~UniformBuffer();
        
        void Bind(GLuint shaderId, GLuint blockId);
        void Update();
        void Destroy();
        
        std::string GetName() { return name; }
        
        bool        IsBound() { return bound; }
        
        GLuint      GetBindingIndex() { return bindingIndex; }
        
        // Access to shader uniforms from script
		ShaderUniform::Ptr GetUniform(std::string name);
        
        // Add a Uniform to this buffer
        UniformBuffer::Ptr AddUniform(ShaderUniform::Ptr newUniform);
        
    private:
        UniformBuffer::Ptr ThisPtr() { return shared_from_this(); }
        
		std::string name;

        GLuint      bufferId;
        GLsizei     bufferSize;
        GLuint      bindingIndex;
        bool        bound;
        
        UniformMap	uniforms;
    };
}