#include "render/Shader.h"

#include <iostream>
#include "utilities/Utilities.h"

namespace epsilon
{
	Shader::Ptr Shader::Create()
	{
		return std::make_shared<Shader>(private_struct());
	}

	Shader::Shader(const private_struct &) : shaderCompiled(false), 
											 shaderActive(false),
											 sourceVersion("#version 330")

	{
	}


	Shader::~Shader(void)
	{
	}

	void Shader::Setup()
	{
		shaderCompiled = CompileShader();
	}

	void Shader::SetMaterialDef(std::string materialDef)
	{
		materialStruct = materialDef;
	}

	bool Shader::CompileShader()
	{
		bool success = true;

		// Insert the version and material definition at the beginning of the source file
		vertexSource = Format("%s\n\n%s\n\n", sourceVersion.c_str(), materialStruct.c_str() );
		vertexSource += readfile("resources/shaders/basic.vert");
		
		// Add version def
		fragSource = Format("%s\n\n", sourceVersion.c_str() );
		fragSource += readfile("resources/shaders/basic.frag");

		GLenum ErrorCheckValue = glGetError();
		const char * vSource = vertexSource.c_str();
		const char * fSource = fragSource.c_str();
		
		// Compile Vertex Shader
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vSource, NULL);
		glCompileShader(vertexShaderId);
 
		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("ERROR: Compiling Vertex Shader");
			success = false;
		}

		// Compile Fragment Shader
		fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShaderId, 1, &fSource, NULL);
		glCompileShader(fragShaderId);

		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("ERROR: Compiling Fragment Shader");
			Log((const char *)gluErrorString(ErrorCheckValue));
			success = false;
		}

		// Compile/Link Shader Program
		programId = glCreateProgram();
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragShaderId);
		glLinkProgram(programId);

		ErrorCheckValue = glGetError();

		if (ErrorCheckValue != GL_NO_ERROR)
		{
			Log("ERROR: Compiling Shader Program");
			Log((const char *)gluErrorString(ErrorCheckValue));
			success = false;
		}

		// Get the position uniform
		positionUnf = glGetUniformLocation(programId, "object_Position");
		if ( (int)positionUnf == -1 )
		{
			ErrorCheckValue = glGetError();
			Log((const char *)gluErrorString(ErrorCheckValue));
		}

		viewMatUnf = glGetUniformLocation(programId, "modelViewMatrix");
		projMatUnf = glGetUniformLocation(programId, "projMatrix");

		return success;
	}

	GLuint Shader::GetUniformId(std::string uniformName)
	{
		if ( shaderCompiled )
		{
			return glGetUniformLocation(programId, uniformName.c_str());
		}
		else
		{
			return -1;
		}
	}

	bool Shader::SetColourUniform(GLuint uId, const Colour &colour)
	{
		bool success = false;
		if ( shaderCompiled && shaderActive )
		{
			glUniform4f(uId, colour.r, colour.g, colour.b, colour.a);
			success = true;
		}
		return success;
	}

	bool Shader::SetFloatUniform(GLuint uId, const float &value)
	{
		bool success = false;
		if ( shaderCompiled && shaderActive )
		{
			glUniform1f(uId, value);
			success = true;
		}
		return success;
	}

	bool Shader::UseShader()
	{
		if ( shaderCompiled )
		{
			glUseProgram(programId);

			shaderActive = true;
		}
		return shaderActive;
	}

	bool Shader::UseShader(RenderStateStack::Ptr stateStack)
	{
		RenderState::Ptr state = stateStack->State();

		if ( shaderCompiled )
		{
			glUseProgram(programId);

			shaderActive = true;

			// Calculate the model view matrix
			Matrix4 modelViewMatrix = state->view * state->model;
			modelViewMatrix.Transpose();

			// Send it to the shader
			glUniformMatrix4fv(viewMatUnf, 1, GL_FALSE, &modelViewMatrix[0]);
			glUniformMatrix4fv(projMatUnf, 1, GL_FALSE, &state->projection[0]);
		}
		return shaderActive;
	}

	bool Shader::DisableShader()
	{
		if (shaderActive)
		{
			// Disable this shader as the active program
			glUseProgram(0);
			shaderActive = false;
		}
		return shaderActive;
	}

}