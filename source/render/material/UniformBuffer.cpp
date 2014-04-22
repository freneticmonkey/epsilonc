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
    
	void UniformBuffer::Bind(GLuint shaderId, GLuint blockId)
    {
		GLuint uniformBlockIndex;

		if (!bound)
		{
			// Build the Buffer Info by extracting it from the Shader.
			int nameLen = -1;
			char tempName[100];
			
			glGetActiveUniformBlockiv(shaderId, blockId, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);
			glGetActiveUniformBlockName(shaderId, blockId, nameLen, NULL, &tempName[0]);
			CheckOpenGLError("Extracting Uniform Block Name");
			tempName[nameLen] = 0;

			uniformBlockIndex = glGetUniformBlockIndex(shaderId, tempName);
			
			name = std::string(tempName);

			CheckOpenGLError("Getting Uniform Block Index for: " + name);
			Log("Found Uniform Block Index for: " + name);

			int dataSize = 0;
			glGetActiveUniformBlockiv(shaderId, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
			Log("Size Required: " + std::to_string(dataSize));

			int activeBlockUniforms = 0;
			glGetActiveUniformBlockiv(shaderId, blockId, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeBlockUniforms);
			Log("# active uniforms: " + std::to_string(activeBlockUniforms));

			GLint blockUniformIndices[UniformBuffer::MAX_BLOCK_INDICIES];
			glGetActiveUniformBlockiv(shaderId, blockId, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, blockUniformIndices);

			// Query the uniforms and build the uniforms for the buffer.
			for (int i = 0; i < activeBlockUniforms; i++)
			{
				AddUniform(ShaderUniform::CreateFromShader(shaderId, blockUniformIndices[i]));
			}

			// If there are uniforms assigned to this buffer
			if (uniforms.size() > 0)
			{
				// Create the buffer
				glGenBuffers(1, &bufferId);
				CheckOpenGLError("Gen Uniform Buffer");

				// Bind it for access
				glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
				CheckOpenGLError("Binding Uniform Buffer");

				int alignSize = 0;

				glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignSize);
				Log("Uniform Buffer Alignment Size: " + std::to_string(alignSize));

				int maxblockSize = 0;

				glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxblockSize);
				Log("Uniform Buffer Max Block Size: " + std::to_string(maxblockSize));

				/*
				// Determine the size of the uniform buffer from it's uniforms
				for_each(uniforms.begin(), uniforms.end(), [&](std::pair<std::string, ShaderUniform::Ptr> uniform){

					// move the offset for the next uniform
					bufferSize += uniform.second->GetSize();
				});
				*/

				// Use the data size extracted from the Uniform Buffer info
				bufferSize = dataSize;

				bufferSize += alignSize - (bufferSize % alignSize);

				// Create the necessary space on the GPU
				glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW | GL_MAP_WRITE_BIT);
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
		else
		{
			// If the Uniform is already bound, then just extract the block index for this shader
			// using the name that has already been determined.
			uniformBlockIndex = glGetUniformBlockIndex(shaderId, name.c_str());
			CheckOpenGLError("Extracting the block index: " + std::string(name));
		}

		// Bind the current shader uniform block to the uniform buffer
		glUniformBlockBinding(shaderId, uniformBlockIndex, bindingIndex);
		CheckOpenGLError("Binding to Uniform Block: " + std::string(name));

		Log("Found Uniform Block Index for: " + name);

    }
    
    void UniformBuffer::Update()
    {
        if ( bound )
        {
            glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
            CheckOpenGLError("Binding Uniform Buffer");
            
            // Cycle through each of the uniforms and if they have changed, copy their data to the GPU
			for_each(uniforms.begin(), uniforms.end(), [&](std::pair<std::string, ShaderUniform::Ptr> uniform){
                
                if ( uniform.second->HasChanged())
                {
					uniform.second->SetUniformBufferValue();
                }
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
        uniforms[newUniform->GetName()] = newUniform;
        return ThisPtr();
    }

	ShaderUniform::Ptr UniformBuffer::GetUniform(std::string name)
	{
		ShaderUniform::Ptr foundUniform;
		if (uniforms.find(name) != uniforms.end())
		{
			foundUniform = uniforms[name];
		}
		return foundUniform;
	}
}
