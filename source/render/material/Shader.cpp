#include "render/material/Shader.h"

#include <iostream>
#include "utilities/Utilities.h"
#include "render/RenderUtilities.h"

namespace epsilon
{
	Shader::Ptr Shader::Create()
	{
		return std::make_shared<Shader>(private_struct());
	}

	Shader::Shader(const private_struct &) : shaderCompiled(false), 
											 shaderActive(false),
											 sourceVersion("#version 330")

	{
		// Set Default Shader
		SetMaterialFile("resources/shaders/material.frag");
        AddStage("resources/shaders/basic/basic.vert", ShaderStageType::Type::VERTEX);
        AddStage("resources/shaders/basic/basic.frag", ShaderStageType::Type::FRAGMENT);
	}

	Shader::~Shader(void)
	{
	}

	void Shader::Setup()
	{
		CompileShader();
	}
    
    Shader::Ptr Shader::AddStage(std::string sourceFile, ShaderStageType::Type type)
    {
        ShaderStage::Ptr newStage = ShaderStage::Create(sourceFile, type);
        RegisterResource(newStage);
        newStage->SetMaterialDefinition(materialStruct);
        if (stages[type])
        {
            stages[type]->RemoveOwner(GetId());
        }
        stages[type] = newStage;
        
        // invalidate shader
        shaderCompiled = false;
        
        return ThisPtr();
    }

	void Shader::SetMaterialFile(std::string materialFile)
	{
		materialStruct = readfile(materialFile);
	}

	bool Shader::CompileShader()
	{
		bool success = true;
        
        // Compile/Link Shader Program
        programId = glCreateProgram();

        // For each stage
        for (int i = 0; i < ShaderStageType::MAX_STAGES; i++ )
        {
            // If the stage is defined
            if (stages[i])
            {
                // Compile it
                if ( stages[i]->Compile() )
                {
                    // If successful
                    glAttachShader(programId, stages[i]->GetStageId());
                }
                else
                {
                    success = false;
                }
            }
        }
        
        if (success)
        {
        
            glLinkProgram(programId);

            success = CheckOpenGLError("Linking Shader");
        }

        if ( success )
        {
            viewMatUnf = glGetUniformLocation(programId, "modelViewMatrix");
            projMatUnf = glGetUniformLocation(programId, "projMatrix");
            
            shaderCompiled = true;
        }

		return success;
	}
    
    void Shader::RefreshResources(ResourceIdVector resources)
    {
        // Recompile the Shader
        CompileShader();
    }
    
	GLuint Shader::GetUniformId(std::string uniformName)
	{
		if ( shaderCompiled )
		{
			return glGetUniformLocation(programId, uniformName.c_str());
		}
		else
		{
			return -1;
		}
	}

	bool Shader::SetColourUniform(GLuint uId, const Colour &colour)
	{
		bool success = false;
		if ( shaderCompiled && shaderActive )
		{
			glUniform4f(uId, colour.r, colour.g, colour.b, colour.a);
			success = true;
		}
		return success;
	}

	bool Shader::SetFloatUniform(GLuint uId, const float &value)
	{
		bool success = false;
		if ( shaderCompiled && shaderActive )
		{
			glUniform1f(uId, value);
			success = true;
		}
		return success;
	}

	bool Shader::UseShader()
	{
		if ( shaderCompiled )
		{
			glUseProgram(programId);

			shaderActive = true;
		}
		return shaderActive;
	}

	bool Shader::UseShader(RenderStateStack::Ptr stateStack)
	{
		RenderState::Ptr state = stateStack->State();

		if ( shaderCompiled )
		{
			glUseProgram(programId);

			shaderActive = true;

			// Calculate the model view matrix
			Matrix4 modelViewMatrix = state->view * state->model;
			modelViewMatrix.Transpose();

			// Send it to the shader
			glUniformMatrix4fv(viewMatUnf, 1, GL_FALSE, &modelViewMatrix[0]);
			glUniformMatrix4fv(projMatUnf, 1, GL_FALSE, &state->projection[0]);
		}
		return shaderActive;
	}

	bool Shader::DisableShader()
	{
		if (shaderActive)
		{
			// Disable this shader as the active program
			glUseProgram(0);
			shaderActive = false;
		}
		return shaderActive;
	}

}