
#include "render/material/ShaderUniform.h"
#include "render/RenderUtilities.h"

namespace epsilon
{
	ShaderUniform::Ptr ShaderUniform::CreateFromShader(GLuint shaderId, GLuint uniformIndex)
	{
		GLuint		location;
		GLint		offset;
		GLint		size;
		GLint		arrayStride;
		GLint		matrixStride;
		GLint		matrixIsRowMajor;
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];

		// Extract name and type
		glGetActiveUniform(shaderId, uniformIndex, sizeof(name)-1, &name_len, &num, &type, name);
		name[name_len] = 0;

		// Extract location
		location = glGetUniformLocation(shaderId, name);
		
		// Extract Properties
		glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_OFFSET, &offset);
		glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_SIZE, &size);
		glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);
		glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);
		glGetActiveUniformsiv(shaderId, 1, &uniformIndex, GL_UNIFORM_IS_ROW_MAJOR, &matrixIsRowMajor);

		ShaderUniform::Ptr newUniform = std::make_shared<ShaderUniform>(private_struct());
		newUniform->location		= location;
		newUniform->type			= type;
		newUniform->name			= std::string(name);
		newUniform->offset			= offset;
		newUniform->size			= size;
		newUniform->arrayStride		= arrayStride;
		newUniform->matrixStride	= matrixStride;
		newUniform->matrixIsRowMajor = matrixIsRowMajor;
		
		//Log("ShaderUniform", "Extracted Uniform from Shader: " + std::string(name) );

		return newUniform;
	}

	ShaderUniform::ShaderUniform(const private_struct &) : changed(false),
															iVal(0),
															fVal(0.f),
															vec2(0.f),
															vec3(0.f),
															vec4(0.f)
	{
		mat4 = Matrix4();
	}

	ShaderUniform::~ShaderUniform() {};
	
	void ShaderUniform::SetInt(const int & newInt)
	{
		iVal = newInt;
		size = sizeof(GLint);
		changed = true;
	}

	void ShaderUniform::SetFloat(const float & newFloat)
	{
		fVal = newFloat;
		size = sizeof(GLfloat);
		changed = true;
	}

	void ShaderUniform::SetVector2(const Vector2 & vec)
	{
		vec2 = vec;
		size = sizeof(GLfloat) * 2;
		changed = true;
	}

	void ShaderUniform::SetVector3(const Vector3 & vec)
	{
		vec3 = vec;
		size = sizeof(GLfloat)* 3;
		changed = true;
	}

	void ShaderUniform::SetVector4(const Vector4 & vec)
	{
		vec4 = vec;
		size = sizeof(GLfloat)* 4;
		changed = true;
	}

	void ShaderUniform::SetMatrix4(const Matrix4 & mat)
	{
		mat4 = mat;
		size = sizeof(GLfloat)* 16;
		changed = true;
	}

	// This should only be called from the Shader class
	void ShaderUniform::SetShaderValue()
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
	void ShaderUniform::SetUniformBufferValue()
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

		CheckOpenGLError("Updating Uniform Buffer: " + name);
	}

	// Again only from the UniformBuffer class - Horribly slow. Do not use...
	void ShaderUniform::MapUniformBufferValue()
	{
		GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
		GLubyte* destination = (GLubyte*)glMapBufferRange(GL_UNIFORM_BUFFER, offset, size, access);

		if (destination != NULL)
		{
			void * data;
			switch (type)
			{
			case GL_INT:
				data = &iVal;
				changed = false;
				break;
			case GL_FLOAT:
				data = &fVal;
				changed = false;
				break;
			case GL_FLOAT_VEC2:
				data = &vec2[0];
				changed = false;
				break;
			case GL_FLOAT_VEC3:
				data = &vec3[0];
				changed = false;
				break;
			case GL_FLOAT_VEC4:
				data = &vec4[0];
				changed = false;
				break;
			case GL_FLOAT_MAT4:
				data = &mat4[0];
				changed = false;
				break;
			}
			memcpy(destination, data, size);

			glFlushMappedBufferRange(GL_UNIFORM_BUFFER, offset, size);
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

	}
}