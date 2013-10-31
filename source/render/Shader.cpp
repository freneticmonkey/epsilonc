#include "render/Shader.h"

#include <iostream>
#include "utilities/Utilities.h"

namespace epsilon
{
	Shader::Ptr Shader::Create()
	{
		return std::make_shared<Shader>(private_struct());
	}

	Shader::Shader(const private_struct &)
	{
		shaderCompiled = false;
	}


	Shader::~Shader(void)
	{
	}

	void Shader::Setup()
	{
		shaderCompiled = CompileShader();
	}

	bool Shader::CompileShader()
	{
		bool success = true;

		vertexSource = readfile("resources/basic.vert");
		fragSource = readfile("resources/basic.frag");

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
		timeUnf = glGetUniformLocation(programId, "time");

		viewMatUnf = glGetUniformLocation(programId, "modelViewMatrix");
		projMatUnf = glGetUniformLocation(programId, "projMatrix");

		return success;
	}

	void Shader::UseShader()
	{
		if ( shaderCompiled )
		{
			glUseProgram(programId);
		}
	}

	void Shader::UseShader(Transform::Ptr transform, Matrix4 viewMatrix, Matrix4 projMatrix)
	{
		if ( shaderCompiled )
		{
			glUseProgram(programId);

			// Calculate the model view matrix
			Matrix4 modelMatrix = transform->_getFullTransform();
			Matrix4 modelViewMatrix = viewMatrix * modelMatrix;

			/*
			Vector3 p = viewMatrix.GetTranslation();
			if ( p.z == 0.f )
				p.z = -5.f;
			
			glm::mat4x4 proj = glm::perspectiveFov(45.0f, 800.0f, 600.0f, 0.1f, 100.0f);
			glm::mat4x4 view = glm::lookAt(glm::vec3(p.x,p.y,p.z), glm::vec3(0,0,0), glm::vec3(0,1,0));
			glm::mat4x4 model = glm::mat4(1.0f);

			glm::mat4 modelView = view * model;
			*/
			//glUniformMatrix4fv(viewMatUnf, 1, GL_FALSE, &modelView[0][0]);

			modelViewMatrix.Transpose();
			glUniformMatrix4fv(viewMatUnf, 1, GL_FALSE, &modelViewMatrix[0]);
			glUniformMatrix4fv(projMatUnf, 1, GL_FALSE, &projMatrix[0]);
			
			//glUniform4f(positionUnf, p.x, p.y, p.z, 0.f);
		}
	}

}