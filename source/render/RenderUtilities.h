#pragma once
#include "EpsilonCore.h"

namespace epsilon
{
	inline bool CheckOpenGLError(std::string operationName)
	{
		bool success = true;
		GLenum errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			Log("Error " + operationName + ": " + std::to_string(errorCheckValue));
			Log((const char *)gluErrorString(errorCheckValue));
			success = false;
		}

		return success;
	}
}