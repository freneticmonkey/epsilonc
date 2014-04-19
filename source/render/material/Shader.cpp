#include "render/material/Shader.h"

#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "utilities/Utilities.h"
#include "render/RenderUtilities.h"

using namespace boost;

namespace epsilon
{
	Shader::Ptr Shader::Create()
	{
		return std::make_shared<Shader>(private_struct());
	}

	Shader::Ptr Shader::CreateFromDefinition(std::string definitionFile)
	{
		Shader::Ptr newShader = std::make_shared<Shader>(private_struct());
		newShader->SetupFromDefinition(definitionFile);
		return newShader;
	}

	Shader::Shader(const private_struct &) : ResourceOwner(),
											 shaderCompiled(false), 
											 shaderActive(false),
											 sourceVersion("#version 330"),
											 compileError(false),
											 compileVersion(0)

	{
	}

	Shader::~Shader(void)
	{
	}

	void Shader::SetupFromDefinition(std::string definitionFile)
	{
		std::string definitions = readfile(definitionFile);

		// pull apart the filename and use it as the shader name
		name = filesystem::path(definitionFile).filename().string();

		// Break into lines
		std::vector<std::string> lines = split(definitions, '\n');

		std::map<std::string, std::string> fileStages;

		std::for_each(lines.begin(), lines.end(), [&](std::string line) {

			// If the line contains a delimiter and is not a comment line
			if ((line.find("=") != std::string::npos) && (line[0] != '#') )
			{
				// strip line whitespace
				line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

				// grab the line contents in the form <shader_stage>=<filename>
				std::vector<std::string> pair = split(line, '=');
				std::transform(pair[0].begin(), pair[0].end(), pair[0].begin(), ::tolower);
				std::string type = pair[0];

				// Build the absolute filename
				filesystem::path filepath(pair[1]);

				// If a relative path, make it relative to the resources folder 
				// using the path of the definitions file as a guide
				if (filepath.is_relative())
				{
					filepath = filesystem::path(definitionFile).parent_path() / filepath;
				}
				
				// Store the stage
				fileStages[type] = filepath.string();
			}
		});

		// Process the results
		std::for_each(fileStages.begin(), fileStages.end(), [&](std::pair<std::string, std::string> stage){

			// If a material was defined
			if (stage.first == "material")
			{
				SetMaterialFile(stage.second);
			}
			else
			{
				// Otherwise try to process this item as a shader stage
				ShaderStageType::Type stageType = ShaderStageType::GetStageConstantByName(stage.first);

				if (stageType != ShaderStageType::Type::UNKNOWN)
				{
					AddStage(stage.second, stageType);
				}
			}
		});
	}

	void Shader::Setup()
	{
		CompileShader();
	}
    
    void Shader::AddStage(std::string sourceFile, ShaderStageType::Type type)
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
    }

	void Shader::SetMaterialFile(std::string materialFile)
	{
		materialStruct = readfile(materialFile);
	}

	bool Shader::CompileShader()
	{
		bool success = true;


		// Ensure that the shader isn't active when we try to compile it (if hotloading).
		DisableShader();

		// If the shader doesn't have any stages defined, use the basic shader
		bool hasStages = false;
		for (int i = 0; i < ShaderStageType::MAX_STAGES; i++)
		{
			if (stages[i])
			{
				hasStages = true;
				break;
			}
		}

		if (!hasStages)
		{
			// Set Default Shader
			SetMaterialFile("resources/shaders/basic/default.mat");
			AddStage("resources/shaders/basic/default.vert", ShaderStageType::Type::VERTEX);
			AddStage("resources/shaders/basic/default.frag", ShaderStageType::Type::FRAGMENT);
		}
        
        // Compile/Link Shader Program
        programId = glCreateProgram();

		compileError = false;

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
					compileError = true;
                }
            }
        }
        
        if (success)
        {
        
            glLinkProgram(programId);

            success = CheckOpenGLError("Linking Shader");

			if (!success)
			{
				DisplayCompileError(programId);
				compileError = true;
			}
        }

        if ( success )
        {
			// Extract the shader uniforms and build the ShaderValue Map
			// courtesy: http://stackoverflow.com/a/4970703
			int total = -1;
			glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &total);
			for (int i = 0; i < total; ++i)
			{
				int name_len = -1, num = -1;
				GLenum type = GL_ZERO;
				char name[100];
				glGetActiveUniform(programId, GLuint(i), sizeof(name)-1,
					&name_len, &num, &type, name);
				name[name_len] = 0;
				GLuint location = glGetUniformLocation(programId, name);

				// Add to the uniform map
				uniforms[std::string(name)] = ShaderUniform::Create(location, type);
			}

			// old code, but grab the modelView and proMatrix uniforms
//            viewMatUnf = glGetUniformLocation(programId, "modelViewMatrix");
//            projMatUnf = glGetUniformLocation(programId, "projMatrix");
            
            shaderCompiled = true;

			// Increment the compile version
			compileVersion++;
        }

		return success;
	}
    
    void Shader::RefreshResources(ResourceIdVector resources)
    {
        // Recompile the Shader
        //CompileShader();
		shaderCompiled = false;
		compileError = false;
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

	ShaderUniform::Ptr Shader::GetUniform(std::string name)
	{
		ShaderUniform::Ptr uniform;

		if (uniforms.find(name) != uniforms.end())
		{
			uniform = uniforms[name];
		}
		return uniform;
	}

	bool Shader::UseShader()
	{
		if ( shaderCompiled )
		{
			// Check if the shader is already bound, if I ever write batching, 
			// this will be result in a easy performance boost?
			/*
			GLuint currProg;
			glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &currProg);

			if (currProg != programId)
			{
				glUseProgram(programId);
			}
			*/

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
			// Check if the shader is already bound, if I ever write batching, 
			// this will be result in a easy performance boost?
			GLuint currProg;
			glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currProg);

			if (currProg != programId)
			{
				glUseProgram(programId);
			}

			shaderActive = true;
			
			// For each of the uniforms values exposed by the shader
			std::for_each(uniforms.begin(), uniforms.end(), [&](std::pair<std::string, ShaderUniform::Ptr> uniform){

				// If their value has changed since it was last set in the shader.
				if (uniform.second->HasChanged())
				{
					// Set it in the shader
					uniform.second->SetShaderValue();
				}
			});

			// Calculate the model view matrix
//			Matrix4 modelViewMatrix = state->view * state->model;
//			modelViewMatrix.Transpose();
//
//			// Send it to the shader
//			glUniformMatrix4fv(viewMatUnf, 1, GL_FALSE, &modelViewMatrix[0]);
//			glUniformMatrix4fv(projMatUnf, 1, GL_FALSE, &state->projection[0]);
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