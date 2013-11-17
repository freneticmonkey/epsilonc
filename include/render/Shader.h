#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "scene/Transform.h"
#include "render/RenderState.h"
#include "render/Colour.h"

namespace epsilon
{
	class Shader
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Shader> Ptr;

		static Shader::Ptr Create();

		explicit Shader(const private_struct &);
		~Shader(void);

		void Setup();

		void SetMaterialDef(std::string materialDef);

		GLuint GetUniformId(std::string uniformName);

		bool SetColourUniform(GLuint uId, const Colour &colour);
		bool SetFloatUniform(GLuint uId, const float &value);

		bool UseShader();
		bool UseShader(RenderStateStack::Ptr stateStack);
		bool DisableShader();

		bool Active() { return shaderActive; }

	private:
		bool CompileShader();

		bool shaderCompiled;
		bool shaderActive;

		GLuint vertexShaderId;
		GLuint fragShaderId;
		GLuint programId;

		GLuint positionUnf;
		GLuint rotationUnf;

		GLuint viewMatUnf;
		GLuint projMatUnf;

		std::string vertexSource;
		std::string fragSource;

		// Hard code this in for now
		std::string materialStruct;
		std::string sourceVersion;
	};

}