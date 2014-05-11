#pragma once

#include "EpsilonCore.h"
#include "math/Vector.h"
#include "render/RenderUtilities.h"

namespace epsilon
{
	enum VertexAttribType
	{
		VERTEX = 0,
		NORMAL,
		COLOUR,
		TEXCOORD,
	};

	class VertexAttribBase
	{
	public:
		virtual size_t GetStride() = 0;
		virtual size_t UpdateStride(size_t currentStride) = 0;
		virtual void SetBufferStride(size_t buffStride) = 0;

		virtual void SetAttribIndex(GLuint attribIndex) = 0;

		virtual int GetUnitNum() = 0;
		virtual int DataLength() = 0;
		virtual float GetUnitValue(int i, int u) = 0;

		virtual bool Enable() = 0;
		virtual void Disable() = 0;
        virtual void Destroy() = 0;
	};

	template<class Type>
	class VertexAttrib : public VertexAttribBase
	{
	public:
		typedef std::vector<Type> List;

        VertexAttrib(List vData, VertexAttribType type);
		~VertexAttrib() {}

		size_t GetStride() { return attributeStride; }
		size_t UpdateStride(size_t currentStride);

		void SetBufferStride(size_t buffStride);

		void SetAttribIndex(GLuint attribIndex);

		int GetUnitNum() { return unitNum; }
		int DataLength() { return data.size(); }

		float GetUnitValue(int i, int u) { return data[i][u]; }
		
		bool Enable();
		void Disable();
        void Destroy();

	private:
		int unitNum;
		int unitType;
		size_t attributeStride;
		GLuint attributeIndex;
		size_t bufferStride;
		
		List data;
	};
}