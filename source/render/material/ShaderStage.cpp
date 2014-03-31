#include "render/material/ShaderStage.h"
#include "utilities/Utilities.h"

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
		// Build the source
		source	= Format("#version %d\n", version);
		source += Format("%s\n", materialDef);
		source += readfile(GetFilepath().GetString());

		// Compile the stage
		const char * stageSource = source.c_str();
		stageId = glCreateShader(ShaderStageType::GetStageConstant(stageType));
		glShaderSource(stageId, 1, &stageSource, NULL);
		glCompileShader(stageId);

		stageCompiled = CheckOpenGLError(Format("Compiling %s Shader: %s", ShaderStageType::GetStageName(stageType), GetFilepath().GetString()));

		if (!stageCompiled)
		{
			DisplayCompileError(stageId);
		}

		return stageCompiled;
	}

	
}