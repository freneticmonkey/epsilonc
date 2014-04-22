//
//  UniformBuffer.cpp
//  Epsilon
//
//  Created by Scott Porter on 21/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

// For OGL includes
#include "EpsilonCore.h"
#include "render/RenderUtilities.h"
#include "render/material/UniformBuffer.h"

namespace epsilon
{
    UniformBuffer::UniformBuffer(const private_struct &, std::string bufferName, GLuint index) : bufferId(-1),
                                                                                                bufferSize(0),
                                                                                                bindingIndex(index),
                                                                                                name(bufferName),
                                                                                                bound(false)
    {
    }
    
    UniformBuffer::~UniformBuffer()
    {
        Destroy();
    }
    
    void UniformBuffer::Bind()
    {
        if ( uniforms.size() > 0 )
        {
            // Create the buffer
            glGenBuffers(1, &bufferId);
            CheckOpenGLError("Gen Uniform Buffer");
            
            // Bind it for access
            glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
            CheckOpenGLError("Binding Uniform Buffer");
            
            // Determine the size of the uniform buffer from it's uniforms
            for_each(uniforms.begin(), uniforms.end(), [&](ShaderUniform::Ptr uniform){
                
                // move the offset for the next uniform
                bufferSize += uniform->GetSize();
            });
            
            // Create the necessary space on the GPU
            glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW);
            CheckOpenGLError("Binding Uniform Buffer Data");
            
            // Bind the buffer data to it's assigned uniform binding point
            glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, bufferId, 0, bufferSize);
            CheckOpenGLError("Binding Uniform Buffer to the Uniform Binding Point");
            
            // Disable this buffer for now
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            CheckOpenGLError("Disabling Uniform Buffer");
            
            // Mark this buffer as successfully bound.
            bound = true;
        }
        else
        {
            Log("UniformBuffer", "Warning: Cannot build buffer without any uniforms set.");
        }
    }
    
    void UniformBuffer::Update()
    {
        if ( bound )
        {
            GLuint offset = 0;
            
            glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
            CheckOpenGLError("Binding Uniform Buffer");
            
//            GLubyte* uniformMap = (GLubyte*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
            
            // Cycle through each of the uniforms and if they have changed, copy their data to the GPU
            for_each(uniforms.begin(), uniforms.end(), [&](ShaderUniform::Ptr uniform){
                
                if ( uniform->HasChanged())
                {
                    uniform->SetUniformBufferValue(offset);
//                    uniform->MapUniformBufferValue(uniformMap);
                }
                
                // move the offset for the next uniform
                offset += uniform->GetSize();
//                uniformMap += uniform->GetSize();
            });
            
            CheckOpenGLError("Updating Uniform Buffer");
            
//            glUnmapBuffer(GL_UNIFORM_BUFFER);
            CheckOpenGLError("Unmapping Uniform Buffer");
            
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            CheckOpenGLError("Disabling Uniform Buffer");
        }
    }
    
    void UniformBuffer::Destroy()
    {
        if ( bound )
        {
            if ( bufferId != 0 )
            {
                glDeleteBuffers(1, &bufferId);
                bufferId = 0;
            }
        }
    }
    
    UniformBuffer::Ptr UniformBuffer::AddUniform(ShaderUniform::Ptr newUniform)
    {
        uniforms.push_back(newUniform);
        return ThisPtr();
    }
}
