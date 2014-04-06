#pragma once
#include "EpsilonCore.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include "render/material/Shader.h"

namespace epsilon
{

	// This encapsulates a shader's uniforms.
	// This class is designed to allow anything with access to a Shader object
	// to easily write and read the Shader's uniforms without having to OpenGL.
	// (i.e. Python scripts)
	class ShaderUniform
	{
		friend class Shader;
	private:
		struct private_struct {};

	public:

		// OpenGL Defines enum
		enum OpenGLTypes
		{
			FLOAT	= GL_FLOAT,
			VECTOR2 = GL_FLOAT_VEC2,
			VECTOR3 = GL_FLOAT_VEC3,
			VECTOR4 = GL_FLOAT_VEC4,
			MATRIX	= GL_FLOAT_MAT4
		};

		typedef std::shared_ptr<ShaderUniform> Ptr;

		static ShaderUniform::Ptr Create(GLuint loc, GLuint type)
		{
			return std::make_shared<ShaderUniform>(private_struct(), loc, type);
		}

		explicit ShaderUniform(const private_struct &, GLuint loc, GLuint itype) : location(loc), 
																					type(itype), 
																					changed(false)
		{
		}

		~ShaderUniform() {};

		bool HasChanged() { return changed; }
		GLuint GetType() { return type; }

		float GetFloat() { return fVal; }
		void SetFloat(const float & newFloat)
		{
			fVal = newFloat;
			changed = true;
		}

		Vector2 GetVector2() { return vec2; }
		void SetVector2(const Vector2 & vec)
		{
			vec2 = vec;
			changed = true;
		}

		Vector3 GetVector3() { return vec3; }
		void SetVector3(const Vector3 & vec)
		{
			vec3 = vec;
			changed = true;
		}

		Vector4 GetVector4() { return vec4; }
		void SetVector4(const Vector4 & vec)
		{
			vec4 = vec;
			changed = true;
		}

		Matrix4 GetMatrix4() { return mat4; }
		void SetMatrix4(const Matrix4 & mat)
		{
			mat4 = mat;
			changed = true;
		}

	protected:

		// This should only be called from the Shader class
		void SetShaderValue()
		{
			switch (type)
			{
			case GL_FLOAT:
				glUniform1f(location, fVal);
				changed = false;
				break;
			case GL_FLOAT_VEC2:
				glUniform2f(location, vec2.x, vec2.y);
				changed = false;
				break;
			case GL_FLOAT_VEC3:
				glUniform3f(location, vec3.x, vec3.y, vec3.z);
				changed = false;
				break;
			case GL_FLOAT_VEC4:
				glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
				changed = false;
				break;
			case GL_FLOAT_MAT4:
				glUniformMatrix4fv(location, 1, GL_FALSE, &mat4[0]);
				changed = false;
				break;
			}
		}
	private:

		GLuint location;
		GLuint type;
		float	fVal;
		Vector2 vec2;
		Vector3 vec3;
		Vector4 vec4; // Not handling matrices for now.
		Matrix4 mat4;
		bool changed;

	};
}
