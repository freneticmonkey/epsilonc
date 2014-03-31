#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "scene/Transform.h"
#include "render/RenderState.h"
#include "render/Colour.h"

#include "render/material/ShaderStage.h"
#include "resource/ResourceOwner.h"

namespace epsilon
{
	class Shader :
        public ResourceOwner
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Shader> Ptr;

		static Shader::Ptr Create();
		static Shader::Ptr CreateFromDefinition(std::string definitionFile);

		explicit Shader(const private_struct &);
		~Shader(void);

		std::string GetName() { return name; }

		void SetupFromDefinition(std::string definitionFile);

		void Setup();
        
        void AddStage(std::string sourceFile, ShaderStageType::Type type);

		void SetMaterialFile(std::string materialFile);

		GLuint GetUniformId(std::string uniformName);

		bool SetColourUniform(GLuint uId, const Colour &colour);
		bool SetFloatUniform(GLuint uId, const float &value);

		bool UseShader();
		bool UseShader(RenderStateStack::Ptr stateStack);
		bool DisableShader();

		bool Active() { return shaderActive; }
		bool Compiled() { return shaderCompiled; }
		bool InError() { return compileError; }
        
        void RefreshResources(ResourceIdVector resources);

	private:
		std::string name;

		bool CompileShader();

		bool shaderCompiled;
		bool shaderActive;

		bool compileError;
        
        // The Shader program Id
        GLuint programId;
        
        ShaderStage::Ptr stages[ShaderStageType::MAX_STAGES];

		GLuint positionUnf;
		GLuint rotationUnf;

		GLuint viewMatUnf;
		GLuint projMatUnf;

		// Hard code this in for now
		std::string materialStruct;
		std::string sourceVersion;
	};

}