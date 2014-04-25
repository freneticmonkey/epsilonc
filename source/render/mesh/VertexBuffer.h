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

		VertexBuffer(List data, VertexBufferType type = ELEMENT);
		~VertexBuffer();

		void BuildBuffer();
		bool Enable();
		void Disable();
        
        void Destroy();

	private:
		int bufferType;
		GLuint bufferTypeGL;

		GLuint bufferId;
		GLsizei bufferSize;
		GLsizei bufferStride;

		List bufferData;
	};
}