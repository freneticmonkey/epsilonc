#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "scene/Transform.h"
#include "render/RenderState.h"
#include "render/Colour.h"
#include "render/material/ShaderUniform.h"

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

		typedef std::map<std::string, ShaderUniform::Ptr> UniformMap;

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

		// Access to shader uniforms from script
		ShaderUniform::Ptr GetUniform(std::string name);

		bool UseShader();
		bool UseShader(RenderStateStack::Ptr stateStack);
		bool DisableShader();

		bool Active() { return shaderActive; }
		bool Compiled() { return shaderCompiled; }
		int  GetCompileVersion() { return compileVersion; }
		bool InError() { return compileError; }
        
        void RefreshResources(ResourceIdVector resources);

	private:
		std::string name;

		bool CompileShader();

		bool shaderCompiled;
		bool shaderActive;

		int compileVersion;

		bool compileError;
        
        // The Shader program Id
        GLuint programId;
        
        ShaderStage::Ptr stages[ShaderStageType::MAX_STAGES];

		GLuint positionUnf;
		GLuint rotationUnf;

        GLuint modelMatUnf;
		GLuint viewMatUnf;
		GLuint projMatUnf;

		// Hard code this in for now
		std::string materialStruct;
		std::string sourceVersion;

		// map access to Shader uniforms
		UniformMap uniforms;
	};

}