#pragma once

#include "resource/Resource.h"

// Rendering Helpers
#include "render/RenderUtilities.h"

namespace epsilon
{

	class ShaderStageType
	{
	public:
        static const int MAX_STAGES = 5;
		enum Type
		{
			VERTEX = 1,
			TESSELATION,
			GEOMETRY,
			FRAGMENT,
			COMPUTE,
			UNKNOWN
		};

		typedef std::map<Type, std::string> StageNames;
		typedef std::map<Type, int>			StageConstants;
		typedef std::map<std::string, Type>	StageConstantNames;

		static std::string GetStageName(Type type)
		{
			return names[type];
		}

		static int GetStageConstant(Type type)
		{
			return constants[type];
		}

		static Type GetStageConstantByName(std::string name)
		{
			if (constantNames.find(name) != constantNames.end())
			{
				return constantNames[name];
			}
			else
			{
				return Type::UNKNOWN;
			}
		}

	private:
		static StageNames			names;
		static StageConstants		constants;
		static StageConstantNames	constantNames;
	};
	

	class ShaderStage : public Resource, public std::enable_shared_from_this<ShaderStage>
	{
	private:
		struct private_struct {};
	public:
		typedef std::shared_ptr<ShaderStage> Ptr;

		static ShaderStage::Ptr Create(std::string filename, ShaderStageType::Type type);

		explicit ShaderStage(const private_struct &, std::string filename, ShaderStageType::Type type);
		~ShaderStage(void);
        
        GLuint GetStageId() { return stageId; }

		ShaderStageType::Type GetStageType() { return stageType;  }

		ShaderStage::Ptr SetMaterialDefinition(std::string matDef);

		// Compile the Shader Stage
		bool Compile();

		// Is the Shader Stage compiled
		bool Compiled() { return stageCompiled; }

	private:
		ShaderStage::Ptr ThisPtr() { return shared_from_this(); }

		bool	stageCompiled;
		GLuint	stageId;

		ShaderStageType::Type stageType;
		std::string source;

		int			version;
		std::string materialDef;
	};
}