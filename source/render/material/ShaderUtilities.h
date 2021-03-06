#pragma once

#include "render/RenderUtilities.h"

namespace epsilon
{
	inline int DisplayCompileError(GLuint shaderId)
	{
		GLint isCompiled = 0;
		GLint lineNumberError = -1;

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

				std::string buffer(errorLog.begin(), errorLog.end());

				// Use regular expressions to pull line number. (Currently NVIDIA Only)
				std::size_t pos = buffer.find(")");
				if (pos != std::string::npos)
				{
					std::string lineNumStr = buffer.substr(2, pos - 1);
					lineNumberError = std::stoi(lineNumStr);
				}

				Log("Shader Compile Error: \n" + buffer);
			}
			//Exit with failure.
			glDeleteShader(shaderId); //Don't leak the shader.
		}

		return lineNumberError;
	}
}