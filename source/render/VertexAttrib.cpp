//#include "render/VertexAttrib.h"

namespace epsilon
{
	/*
	template<class Type>
	VertexAttrib<Type>::VertexAttrib(List vData, VertexAttribType type)
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

	template<class Type>
	void VertexAttrib<Type>::Enable()
	{
		if ( attribudeStride != -1 )
		{
			glVertexAttribPointer(attributeIndex, unitNum, unitType, GL_FALSE, bufferStride, (GLvoid*)attribudeStride);
			glEnableVertexAttribArray(attributeIndex);
		}
	}

	template<class Type>
	void VertexAttrib<Type>::Disable()
	{
		if ( attribudeStride != -1 )
		{
			glDisableVertexAttribArray(attributeIndex);
		}
	}
	*/
}