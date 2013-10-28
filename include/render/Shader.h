#pragma once

#include "EpsilonCore.h"

#include "logging/Logging.h"
#include "scene/Transform.h"

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

		void UseShader();
		void UseShader(Transform::Ptr transform, Matrix4 viewMatrix, Matrix4 projMatrix);

	private:
		bool CompileShader();

		bool shaderCompiled;
		GLuint vertexShaderId;
		GLuint fragShaderId;
		GLuint programId;

		GLuint positionUnf;
		GLuint rotationUnf;

		GLuint viewMatUnf;
		GLuint projMatUnf;

		GLuint timeUnf;

		std::string vertexSource;
		std::string fragSource;

		// Hard code this in for now
	};

}