#pragma once

#include "EpsilonCore.h"
#include "render/RenderUtilities.h"

namespace epsilon
{
	enum VertexBufferType
	{
		ELEMENT = 0,
		INDICES
	};

	class VertexBufferBase
	{
	public:
		virtual void BuildBuffer() = 0;
		virtual bool Enable() = 0;
		virtual void Disable() = 0;
	};

	template<class Type>
	class VertexBuffer : public VertexBufferBase
	{
	public:
		typedef std::vector<Type> List;

		VertexBuffer(List data, VertexBufferType type = ELEMENT)
		{
			bufferId = -1;
			bufferSize = -1;
			bufferStride = -1;

			bufferType = type;
			bufferTypeGL = -1;

			switch(bufferType)
			{
				case ELEMENT:
					bufferTypeGL = GL_ARRAY_BUFFER;
					break;
				case INDICES:
					bufferTypeGL = GL_ELEMENT_ARRAY_BUFFER;
					break;
			}

			bufferData = data;
		}

		~VertexBuffer()
		{
			if ( bufferId != -1 )
			{
				glDeleteBuffers(1, &bufferId);
			}
		}

		void BuildBuffer()
		{
			bufferSize = sizeof(Type) * bufferData.size();

			// Create Buffer
			GLenum ErrorCheckValue = glGetError();
			glGenBuffers(1, &bufferId);
			glBindBuffer(bufferTypeGL, bufferId);
			glBufferData(bufferTypeGL, bufferSize, &bufferData[0], GL_STATIC_DRAW);

			CheckOpenGLError("Creating Vertex buffer");
            Disable();
		}

		bool Enable()
		{
			bool success = false;
			if ( bufferId != -1 )
			{
                glBindBuffer(bufferTypeGL, bufferId);
                
				success = CheckOpenGLError("Binding Vertex buffer");
			}
			return success;
		}

		void Disable()
		{
			if ( bufferId != -1 )
			{
				glBindBuffer(bufferTypeGL, 0);
			}
		}

	private:
		int bufferType;
		GLuint bufferTypeGL;

		GLuint bufferId;
		GLsizei bufferSize;
		GLsizei bufferStride;

		List bufferData;
	};
}