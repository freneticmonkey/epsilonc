#pragma once
#include "EpsilonCore.h"

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
			MATRIX	= GL_FLOAT_MAT4
		};

		typedef std::shared_ptr<ShaderUniform> Ptr;

		static ShaderUniform::Ptr Create(GLuint loc, GLuint type)
		{
			return std::make_shared<ShaderUniform>(private_struct(), loc, type);
		}

		explicit ShaderUniform(const private_struct &, GLuint loc, GLuint itype) : location(loc), 
																					type(itype), 
																					changed(false),
                                                                                    size(0)
		{
		}

		~ShaderUniform() {};
        int GetSize() { return size; }
		bool HasChanged() { return changed; }
		GLuint GetType() { return type; }

        int GetInt() { return iVal; }
		void SetInt(const int & newInt)
		{
			iVal = newInt;
            size = sizeof(int);
			changed = true;
		}
        
		float GetFloat() { return fVal; }
		void SetFloat(const float & newFloat)
		{
			fVal = newFloat;
            size = sizeof(float);
			changed = true;
		}

		Vector2 GetVector2() { return vec2; }
		void SetVector2(const Vector2 & vec)
		{
			vec2 = vec;
            size = sizeof(float) * 2;
			changed = true;
		}

		Vector3 GetVector3() { return vec3; }
		void SetVector3(const Vector3 & vec)
		{
			vec3 = vec;
            size = sizeof(float) * 3;
			changed = true;
		}

		Vector4 GetVector4() { return vec4; }
		void SetVector4(const Vector4 & vec)
		{
			vec4 = vec;
            size = sizeof(float) * 4;
			changed = true;
		}

		Matrix4 GetMatrix4() { return mat4; }
		void SetMatrix4(const Matrix4 & mat)
		{
			mat4 = mat;
            size = sizeof(float) * 16;
			changed = true;
		}

		// This should only be called from the Shader class
		void SetShaderValue()
		{
			switch (type)
			{
            case GL_INT:
                glUniform1i(location, iVal);
                changed = false;
                break;
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
        
        // This should only be called from the UniformBuffer class
        void SetUniformBufferValue(GLuint offset)
        {
            switch (type)
			{
                case GL_INT:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &iVal);
                    changed = false;
                    break;
                case GL_FLOAT:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &fVal);
                    changed = false;
                    break;
                case GL_FLOAT_VEC2:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &vec2[0]);
                    changed = false;
                    break;
                case GL_FLOAT_VEC3:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &vec3[0]);
                    changed = false;
                    break;
                case GL_FLOAT_VEC4:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &vec4[0]);
                    changed = false;
                    break;
                case GL_FLOAT_MAT4:
                    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &mat4[0]);
                    changed = false;
                    break;
			}
        }
        
        void MapUniformBufferValue(GLubyte * destination)
        {
            switch (type)
			{
                case GL_INT:
                    *(GLuint*)destination = iVal;
                    changed = false;
                    break;
                case GL_FLOAT:
                    *(GLfloat*)destination = fVal;
                    changed = false;
                    break;
                case GL_FLOAT_VEC2:
                    *(GLfloat*)destination = vec2.x;
                    destination += sizeof(vec2.x);
                    *(GLfloat*)destination = vec2.y;
                    changed = false;
                    break;
                case GL_FLOAT_VEC3:
                    *(GLfloat*)destination = vec3.x;
                    destination += sizeof(vec3.x);
                    *(GLfloat*)destination = vec3.y;
                    destination += sizeof(vec3.y);
                    *(GLfloat*)destination = vec3.z;
                    changed = false;
                    break;
                case GL_FLOAT_VEC4:
                    *(GLfloat*)destination = vec4.x;
                    destination += sizeof(vec4.x);
                    *(GLfloat*)destination = vec4.y;
                    destination += sizeof(vec4.y);
                    *(GLfloat*)destination = vec4.z;
                    destination += sizeof(vec4.w);
                    *(GLfloat*)destination = vec4.w;
                    changed = false;
                    break;
                case GL_FLOAT_MAT4:
                    for ( int i = 0; i < 16; i++ )
                    {
                        *(GLfloat*)destination = mat4[i];
                        destination += sizeof(mat4[i]);
                    }
                    changed = false;
                    break;
			}
        }
	private:

		GLuint location;
		GLuint type;
        int     iVal;
		float	fVal;
		Vector2 vec2;
		Vector3 vec3;
		Vector4 vec4; // Not handling matrices for now.
		Matrix4 mat4;
		bool changed;
        int     size;

	};
}
