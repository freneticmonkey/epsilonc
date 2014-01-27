#include "render/Shader.h"

#include <iostream>
#include "utilities/Utilities.h"
#include "render/RenderUtilities.h"

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
		// Set Default Shader
		SetMaterialFile("resources/shaders/material.frag");
		SetVertexFile("resources/shaders/basic/basic.vert");
		SetFragmentFile("resources/shaders/basic/basic.frag");
	}

	Shader::~Shader(void)
	{
	}

	void Shader::Setup()
	{
		shaderCompiled = CompileShader();
	}

	void Shader::SetMaterialFile(std::string materialFile)
	{
		materialStruct = readfile(materialFile);
	}

	void Shader::SetVertexSource(std::string vertSource)
	{
		vertexSource = Format("%s\n\n%s\n\n", sourceVersion.c_str(), materialStruct.c_str() );
		vertexSource += vertSource;	
		vertexFile = "No File: Source code only";
	}

	void Shader::SetVertexFile(std::string vertFile)
	{
		vertexSource = Format("%s\n\n%s\n\n", sourceVersion.c_str(), materialStruct.c_str() );
		vertexSource += readfile(vertFile);
		vertexFile = vertFile;
	}
	
	void Shader::SetGeometrySource(std::string geomSource)
	{
		geometrySource = Format("%s\n\n", sourceVersion.c_str() );
		geometrySource += geomSource;
		geometryFile = "No File: Source code only";
	}

	void Shader::SetGeometryFile(std::string geomFile)
	{
		// Add version def
		geometrySource = Format("%s\n\n", sourceVersion.c_str() );
		geometrySource += readfile(geomFile);
		geometryFile = geomFile;
	}

	void Shader::SetFragmentSource(std::string fragSource)
	{
		fragmentSource = Format("%s\n\n", sourceVersion.c_str() );
		fragmentSource += fragSource;
		fragmentFile = "No File: Source code only";
	}

	void Shader::SetFragmentFile(std::string fragFile)
	{
		// Add version def
		fragmentSource = Format("%s\n\n", sourceVersion.c_str() );
		fragmentSource += readfile(fragFile);
		fragmentFile = fragFile;
	}

	bool Shader::CompileShader()
	{
		bool success = true;

		if ( HasVertexShader() )
		{
			const char * vSource = vertexSource.c_str();
			
			// Compile Vertex Shader
			vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShaderId, 1, &vSource, NULL);
			glCompileShader(vertexShaderId);
 
			success = CheckOpenGLError("Compiling Vertex Shader: " + vertexFile);
			
			DisplayCompileError(vertexShaderId);
		}

		if ( HasGeometryShader() )
		{
			const char * gSource = geometrySource.c_str();

			// Compile Vertex Shader
			vertexShaderId = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geomShaderId, 1, &gSource, NULL);
			glCompileShader(geomShaderId);
 
			success = CheckOpenGLError("Compiling Geometry Shader: " + geometryFile);

			DisplayCompileError(geomShaderId);
		}

		if ( HasFragmentShader() )
		{
			const char * fSource = fragmentSource.c_str();

			// Compile Fragment Shader
			fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragShaderId, 1, &fSource, NULL);
			glCompileShader(fragShaderId);

			success = CheckOpenGLError("Compiling Fragment Shader: " + fragmentFile);

			DisplayCompileError(fragShaderId);
		}

		if ( success )
		{
			// Compile/Link Shader Program
			programId = glCreateProgram();

			if ( HasVertexShader() )
			{
				glAttachShader(programId, vertexShaderId);
			}

			if ( HasGeometryShader() )
			{
				glAttachShader(programId, geomShaderId);
			}

			if ( HasFragmentShader() )
			{
				glAttachShader(programId, fragShaderId);
			}
			glLinkProgram(programId);

			success = CheckOpenGLError("Linking Shader");

			viewMatUnf = glGetUniformLocation(programId, "modelViewMatrix");
			projMatUnf = glGetUniformLocation(programId, "projMatrix");
		}

		return success;
	}

	void Shader::DisplayCompileError(GLuint shaderId)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
				GLint maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
				
				if (maxLength > 0 )
				{
					//The maxLength includes the NULL character
					std::vector<char> errorLog(maxLength);
					glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
					Log("Shader Compile Error: \n" + std::string(errorLog.begin(), errorLog.end()) );					
				}
				//Exit with failure.
				glDeleteShader(shaderId); //Don't leak the shader.
				return;
		}
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