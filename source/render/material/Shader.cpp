#include "render/material/Shader.h"

#include <iostream>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "render/material/ShaderManager.h"
#include "render/material/UniformBuffer.h"
#include "render/material/ShaderUtilities.h"

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
        
        // To prevent recompile spamming
        bool canRecompile = true;
        
        // If the last compile threw an error.
        if ( compileError )
        {
            canRecompile = false;
            
            // Check if any of the stages have changed
            // so a recompile could get a different result
            
            // For each stage
            for (int i = 0; i < ShaderStageType::MAX_STAGES; i++ )
            {
                // If the stage is has changed
                if (stages[i] && stages[i]->NeedReload())
                {
                    canRecompile = true;
                    break;
                }
            }
        }
        
        if ( canRecompile )
        {
            // Compile/Link Shader Program
            programId = glCreateProgram();
            
            compileError = false;
            
            // For each stage
            for (int i = 0; i < ShaderStageType::MAX_STAGES; i++ )
            {
                // If the stage is defined
                if (stages[i])
                {
					// Ensure that the Material Definition is set for the Fragment
					// Shader, in case it was found on disk after the fragment shader
					if (i == ShaderStageType::Type::FRAGMENT)
					{
						stages[i]->SetMaterialDefinition(materialStruct);
					}

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
                CheckOpenGLError("Getting # of Active Uniforms");
                
				for (int uniformIndex = 0; uniformIndex < total; ++uniformIndex)
                {
					ShaderUniform::Ptr shaderUniform = ShaderUniform::CreateFromShader(programId, uniformIndex);
                    
                    // Add to the uniform map
					uniforms[shaderUniform->GetName()] = shaderUniform;
                }
              
                // Extract any Active Uniform Blocks
                int numBlocks;
                
                glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
                CheckOpenGLError("Getting # of Active Uniform Blocks");
                
				for (int blockId = 0; blockId < numBlocks; ++blockId)
                {
                    int nameLen = -1;
                    char name[100];
                    GLuint uniformBlockIndex;
					glGetActiveUniformBlockiv(programId, GLuint(blockId), GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLen);
					glGetActiveUniformBlockName(programId, GLuint(blockId), nameLen, NULL, &name[0]);
                    CheckOpenGLError("Extracting Uniform Block");
                    
					// Get the Uniform Buffer from the ShaderManager
					UniformBuffer::Ptr buffer = ShaderManager::GetInstance().GetUniformBuffer(name);
	
					// If this is the first time the block has been bound, set it up.
					buffer->Bind(programId, blockId);
                }
                
                shaderCompiled = true;
                
                // Increment the compile version
                compileVersion++;
            }
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
			GLuint currProg;
			glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&currProg);

			if (currProg != programId)
			{
				glUseProgram(programId);
                CheckOpenGLError("Enabling Shader");
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
            CheckOpenGLError("Setting Shader Uniforms: " + name);
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