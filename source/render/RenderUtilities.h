#pragma once

// OpenGL / GLEW
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
#include "logging/Logging.h"


namespace epsilon
{
	inline bool CheckOpenGLError(std::string operationName)
	{
		bool success = true;
		GLenum errorCheckValue = glGetError();
		if (errorCheckValue != GL_NO_ERROR)
		{
			Log("Error " + operationName + ": " + std::to_string(errorCheckValue));
//			Log((const char *)gluErrorString(errorCheckValue));
            Log((const char *)glewGetErrorString(errorCheckValue));
			success = false;
		}

		return success;
	}
}