#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	inline void DisplayCompileError(GLuint shaderId)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			if (maxLength > 0)
			{
				//The maxLength includes the NULL character
				std::vector<char> errorLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
				Log("Shader Compile Error: \n" + std::string(errorLog.begin(), errorLog.end()));
			}
			//Exit with failure.
			glDeleteShader(shaderId); //Don't leak the shader.
			return;
		}
	}
}