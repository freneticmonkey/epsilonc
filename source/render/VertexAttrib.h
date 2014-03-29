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
	};

	template<class Type>
	class VertexAttrib : public VertexAttribBase
	{
	public:
		typedef std::vector<Type> List;

		VertexAttrib(List vData, VertexAttribType type)
		{
			data = vData;
			attributeStride = -1;
			attributeIndex = -1;
			bufferStride = -1;

			unitNum = 3;
			unitType = GL_FLOAT;
			switch(type)
			{
				case COLOUR:
					unitNum = 4;
					break;
				case TEXCOORD:
					unitNum = 2;
					break;
			}
		}

		~VertexAttrib() {}

		size_t GetStride() { return attributeStride; }
		size_t UpdateStride(size_t currentStride)
		{
			attributeStride = currentStride;
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

		bool Enable()
		{
            bool success = false;
			if ( attributeStride != -1 )
			{
                glVertexAttribPointer(attributeIndex, unitNum, unitType, GL_FALSE, bufferStride, (GLvoid*)attributeStride);
                success = CheckOpenGLError("Setting Vertex Attrib Pointer");
                
                if ( success )
                {
                    glEnableVertexAttribArray(attributeIndex);
                    success = CheckOpenGLError("Enabling Vertex Attrib");
                }
			}
            return success;
		}

		void Disable()
		{
			if ( attributeStride != -1 )
			{
				glDisableVertexAttribArray(attributeIndex);
			}
		}

	private:
		int unitNum;
		int unitType;
		size_t attributeStride;
		GLuint attributeIndex;
		size_t bufferStride;
		
		List data;
	};
}