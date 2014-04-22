#include "render/material/ShaderStage.h"
#include "render/material/ShaderUtilities.h"
#include "utilities/Utilities.h"

#include <string>
#include <regex>
#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	/*
	 * ShaderStageType Definitions
	 */
	ShaderStageType::StageNames ShaderStageType::names = {
		{ Type::VERTEX,		"Vertex" },
		{ Type::TESSELATION,"Tesselation" },
		{ Type::GEOMETRY,	"Geometry" },
		{ Type::FRAGMENT,	"Fragment" },
		{ Type::COMPUTE,	"Compute" }
	};

	ShaderStageType::StageConstants ShaderStageType::constants = {
		{ Type::VERTEX,		GL_VERTEX_SHADER},
		{ Type::TESSELATION,GL_TESS_CONTROL_SHADER },
		{ Type::GEOMETRY,	GL_GEOMETRY_SHADER },
		{ Type::FRAGMENT,	GL_FRAGMENT_SHADER },
		{ Type::COMPUTE,	GL_COMPUTE_SHADER }
	};

	ShaderStageType::StageConstantNames ShaderStageType::constantNames = {
		{ "vertex",		Type::VERTEX },
		{ "tesselation",Type::TESSELATION },
		{ "geometry",	Type::GEOMETRY },
		{ "fragment",	Type::FRAGMENT },
		{ "compute",	Type::COMPUTE }
	};

	ShaderStage::Ptr ShaderStage::Create(std::string filename, ShaderStageType::Type type)
	{
		return std::make_shared<ShaderStage>(private_struct(), filename, type);
	}

	ShaderStage::ShaderStage(const private_struct &, std::string filename, ShaderStageType::Type type) : Resource(filename, ResourceType::Type::SHADER),
																										 stageCompiled(false),
																										 stageType(type),
																										 version(330)
	{
	}

	ShaderStage::~ShaderStage(void)
	{
	}

	ShaderStage::Ptr ShaderStage::SetMaterialDefinition(std::string matDef)
	{
		materialDef = matDef;
		return ThisPtr();
	}

	bool ShaderStage::Compile()
	{
        GLboolean canCompile;
        glGetBooleanv(GL_SHADER_COMPILER, &canCompile);
        
        if ( canCompile != GL_FALSE)
        {
            // Build the source
            source = str(format("#version %d\n") % version);
            source += str(format("%s\n") % materialDef);
            source += readfile(GetFilepath().GetString());
            
            // Compile the stage
            const char * stageSource = source.c_str();
            stageId = glCreateShader(ShaderStageType::GetStageConstant(stageType));
            CheckOpenGLError("Creating shader");
            glShaderSource(stageId, 1, &stageSource, NULL);
            CheckOpenGLError("Setting shader source");
            
            glCompileShader(stageId);
            
            GLint compileState;
            glGetShaderiv(stageId, GL_COMPILE_STATUS, &compileState);
            
            if ( compileState == GL_FALSE)
            {
                stageCompiled = false;
                
                GLint logLength;
                glGetShaderiv(stageId, GL_INFO_LOG_LENGTH, &logLength);
                
                if (logLength > 0 )
                {
                    GLchar * infoLog = (GLchar *)malloc(logLength);
                    glGetShaderInfoLog(stageId, logLength, &logLength, infoLog);
                    Log(std::string(infoLog));
                }
                
                std::string filename = GetFilepath().GetString();
                std::string stageName = ShaderStageType::GetStageName(stageType);
                CheckOpenGLError(str(format("Compiling %s Shader: %s") % stageName % filename ));
                
                if (!stageCompiled)
                {
					// Display an error message and get the shader code line in error
					int lineError = DisplayCompileError(stageId);

					// Insert line numbers and line error message into the shader source
					int lineNum = 1;
					source.insert(0, std::to_string(lineNum++) + ": ");
					
					std::size_t found = source.find("\n");
					bool showError = false;
					while (found != std::string::npos)
					{
						if (showError)
						{
							// Insert an error message before the next newline
							source.insert(--found, "\t<<========== ERROR");
							found+=20;
							showError = false;
						}
						std::string strLineNum = std::to_string(lineNum++) + ": ";
						source.insert(++found, strLineNum);
						found += strLineNum.length();
						found = source.find("\n", found);

						if (lineNum == (lineError+1))
						{
							showError = true;
						}
					}
                    Log(str(format("Shader: %s\n") % filename ),"\n" + source);
                }
            }
            else
            {
                stageCompiled = true;
            }
            
            // Mark the Resource as having loaded
            SetReloaded();
        }
        else
        {
            Log("ShaderStage","Shader Compiling not supported.");
            CheckOpenGLError("Can Compile Shaders");
        }

		return stageCompiled;
	}	
}