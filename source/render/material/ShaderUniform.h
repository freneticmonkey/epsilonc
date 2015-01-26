#pragma once
#include "render/RenderUtilities.h"

#include "math/Vector.h"
#include "math/Matrix.h"

namespace epsilon
{
	// This encapsulates a shader's uniforms.
	// This class is designed to allow anything with access to a Shader object
	// to easily write and read the Shader's uniforms without having to OpenGL.
	// (i.e. Python scripts)
	class ShaderUniform
	{
	private:
		struct private_struct {};

	public:
		// OpenGL Defines enum
		enum OpenGLTypes
		{
            INT     = GL_INT,
			FLOAT	= GL_FLOAT,
			VECTOR2 = GL_FLOAT_VEC2,
			VECTOR3 = GL_FLOAT_VEC3,
			VECTOR4 = GL_FLOAT_VEC4,
			MATRIX	= GL_FLOAT_MAT4,
			TEXTURE = GL_SAMPLER_2D,
			TEXTURE_SHADOW = GL_SAMPLER_2D_SHADOW
		};

		typedef std::shared_ptr<ShaderUniform> Ptr;

		static ShaderUniform::Ptr CreateFromShader(GLuint shaderId, GLuint uniformIndex);

		explicit ShaderUniform(const private_struct &);

		~ShaderUniform();

        int GetSize() { return size; }

		std::string GetName() { return name; }

		bool HasChanged() { return changed; }
		GLuint GetType() { return type; }

        int GetInt() { return iVal; }
		void SetInt(const int & newInt);      

		float GetFloat() { return fVal; }
		void SetFloat(const float & newFloat);

		Vector2 GetVector2() { return vec2; }
		void SetVector2(const Vector2 & vec);

		Vector3 GetVector3() { return vec3; }
		void SetVector3(const Vector3 & vec);

		Vector4 GetVector4() { return vec4; }
		void SetVector4(const Vector4 & vec);

		Matrix4 GetMatrix4() { return mat4; }
		void SetMatrix4(const Matrix4 & mat);

		// This should only be called from the Shader class
		void SetShaderValue();
        
        // This should only be called from the UniformBuffer class
		void SetUniformBufferValue();
        
		// - Horribly slow. Do not use...
		void MapUniformBufferValue();

	private:

		std::string name;
		GLuint		location;
		GLuint		type;
		GLuint		offset;
		GLint		arraySize;
		GLint		arrayStride;
		GLint		matrixStride;
		GLint		matrixIsRowMajor;

        int			iVal;
		float		fVal;
		Vector2		vec2;
		Vector3		vec3;
		Vector4		vec4;
		Matrix4		mat4;
		bool		changed;
        int			size;

	};

	typedef std::vector<ShaderUniform::Ptr> ShaderUniforms;
}
