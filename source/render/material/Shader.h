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
        public ResourceOwner,
        public std::enable_shared_from_this<Shader>
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Shader> Ptr;

		static Shader::Ptr Create();

		explicit Shader(const private_struct &);
		~Shader(void);

		void Setup();
        
        Shader::Ptr AddStage(std::string sourceFile, ShaderStageType::Type type);

		void SetMaterialFile(std::string materialFile);

		GLuint GetUniformId(std::string uniformName);

		bool SetColourUniform(GLuint uId, const Colour &colour);
		bool SetFloatUniform(GLuint uId, const float &value);

		bool UseShader();
		bool UseShader(RenderStateStack::Ptr stateStack);
		bool DisableShader();

		bool Active() { return shaderActive; }
		bool Compiled() { return shaderCompiled; }
        
        void RefreshResources(ResourceIdVector resources);

	private:
        Shader::Ptr ThisPtr() { return shared_from_this(); }
        
		bool CompileShader();

		bool shaderCompiled;
		bool shaderActive;
        
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