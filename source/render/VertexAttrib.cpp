#include "render/VertexAttrib.h"

namespace epsilon
{
    template<class Type>
	VertexAttrib<Type>::VertexAttrib(std::vector<Type> vData, VertexAttribType type)
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
    
    template<class Type>
    void VertexAttrib<Type>::Destroy()
    {
        // Don't have to clean up attrib arrays?
    }
    
    template<class Type>
	size_t VertexAttrib<Type>::UpdateStride(size_t currentStride)
    {
        attributeStride = currentStride;
        return currentStride + (sizeof(GLfloat) * unitNum);
    }
    
    template<class Type>
    void VertexAttrib<Type>::SetBufferStride(size_t buffStride)
    {
        bufferStride = buffStride;
    }
    
    template<class Type>
    void VertexAttrib<Type>::SetAttribIndex(GLuint attribIndex)
    {
        attributeIndex = attribIndex;
    }
    
    template<class Type>
    bool VertexAttrib<Type>::Enable()
    {
        bool success = false;
        if ( attributeStride != -1 )
        {
            GLint currentBuffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBuffer);
            
            glEnableVertexAttribArray(attributeIndex);
            success = CheckOpenGLError("Enabling Vertex Attrib");
            if ( success )
            {
                glVertexAttribPointer(attributeIndex, unitNum, unitType, GL_FALSE, bufferStride, (GLvoid*)attributeStride);
                success = CheckOpenGLError("Setting Vertex Attrib Pointer");
            }
        }
        return success;
    }
    
    template<class Type>
    void VertexAttrib<Type>::Disable()
    {
        if ( attributeStride != -1 )
        {
            glDisableVertexAttribArray(attributeIndex);
        }
    }
    
    // Explict instantiation so that the linker knows the possible types that
    // can use the VertexAttrib class.
    template class VertexAttrib<Vector2>;
    template class VertexAttrib<Vector3>;
    template class VertexAttrib<Vector4>;
}