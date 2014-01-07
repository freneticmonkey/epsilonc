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

		void SetMaterialFile(std::string materialFile);

		void SetVertexSource(std::string vertSource);
		void SetVertexFile(std::string vertFile);
		bool HasVertexShader() { return vertexSource.size() > 0; }
		
		void SetGeometrySource(std::string geomSource);
		void SetGeometryFile(std::string geomFile);
		bool HasGeometryShader() { return geometrySource.size() > 0; }

		void SetFragmentSource(std::string fragSource);
		void SetFragmentFile(std::string fragFile);
		bool HasFragmentShader() { return fragmentSource.size() > 0; }

		GLuint GetUniformId(std::string uniformName);

		bool SetColourUniform(GLuint uId, const Colour &colour);
		bool SetFloatUniform(GLuint uId, const float &value);

		bool UseShader();
		bool UseShader(RenderStateStack::Ptr stateStack);
		bool DisableShader();

		bool Active() { return shaderActive; }

	private:
		bool CompileShader();

		void DisplayCompileError(GLuint shaderId);

		bool shaderCompiled;
		bool shaderActive;

		GLuint vertexShaderId;
		GLuint geomShaderId;
		GLuint fragShaderId;
		GLuint programId;

		GLuint positionUnf;
		GLuint rotationUnf;

		GLuint viewMatUnf;
		GLuint projMatUnf;

		std::string vertexSource;
		std::string vertexFile;
		std::string geometrySource;
		std::string geometryFile;
		std::string fragmentSource;
		std::string fragmentFile;

		// Hard code this in for now
		std::string materialStruct;
		std::string sourceVersion;
	};

}