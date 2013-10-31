#pragma once

#include "EpsilonCore.h"
#include "math/Vector.h"

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

		virtual void Enable() = 0;
		virtual void Disable() = 0;
	};

	template<class Type>
	class VertexAttrib : public VertexAttribBase
	{
	public:
		typedef std::vector<Type> List;

		VertexAttrib(List vData, VertexAttribType type)
		{
			data = vData;
			attribudeStride = -1;
			attributeIndex = -1;
			bufferStride = -1;

			unitNum = 3;
			unitType = GL_FLOAT;
			switch(type)
			{
				case TEXCOORD:
					unitNum = 2;
					//unitType = GL_FLOAT;
					break;
			}
		}

		~VertexAttrib() {}

		size_t GetStride() { return attribudeStride; }
		size_t UpdateStride(size_t currentStride)
		{
			attribudeStride = currentStride;
			return currentStride + (sizeof(GLfloat) * unitNum);
		}

		void SetBufferStride(size_t buffStride)
		{
			bufferStride = buffStride;
		}

		void SetAttribIndex(GLuint attribIndex)
		{
			attributeIndex = attribIndex;
		}

		int GetUnitNum() { return unitNum; }
		int DataLength() { return data.size(); }

		float GetUnitValue(int i, int u) { return data[i][u]; }

		void Enable()
		{
			if ( attribudeStride != -1 )
			{
				glVertexAttribPointer(attributeIndex, unitNum, unitType, GL_FALSE, bufferStride, (GLvoid*)attribudeStride);
				glEnableVertexAttribArray(attributeIndex);
			}
		}

		void Disable()
		{
			if ( attribudeStride != -1 )
			{
				glDisableVertexAttribArray(attributeIndex);
			}
		}

	private:
		int unitNum;
		int unitType;
		size_t attribudeStride;
		GLuint attributeIndex;
		size_t bufferStride;
		
		List data;
	};
}