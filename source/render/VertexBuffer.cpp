//
//  VertexBuffer.cpp
//  Epsilon
//
//  Created by Scott Porter on 11/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/VertexBuffer.h"

namespace epsilon
{
    template<class Type>
    VertexBuffer<Type>::VertexBuffer(VertexBuffer::List data, VertexBufferType type)
    {
        bufferId = 0;
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
    
    template<class Type>
    VertexBuffer<Type>::~VertexBuffer()
    {
        Destroy();
    }
    
    template<class Type>
    void VertexBuffer<Type>::Destroy()
    {
        if ( bufferId != 0 )
        {
            glDeleteBuffers(1, &bufferId);
            bufferId = 0;
        }
    }
    
    template<class Type>
    void VertexBuffer<Type>::BuildBuffer()
    {
        bufferSize = sizeof(Type) * bufferData.size();
        
        // Create Buffer
        GLenum ErrorCheckValue = glGetError();
        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferTypeGL, bufferId);
        glBufferData(bufferTypeGL, bufferSize, &bufferData[0], GL_STATIC_DRAW);
        
        Disable();
    }
    
    template<class Type>
    bool VertexBuffer<Type>::Enable()
    {
        bool success = false;
        if ( bufferId != 0 )
        {
            glBindBuffer(bufferTypeGL, bufferId);
            
            success = CheckOpenGLError("Binding Vertex buffer");
        }
        return success;
    }
    
    template<class Type>
    void VertexBuffer<Type>::Disable()
    {
        if ( bufferId != 0 )
        {
            glBindBuffer(bufferTypeGL, 0);
            
            bool success = CheckOpenGLError("Unbinding buffer");
            
            if ( !success )
            {
                Log("Problem unbinding buffer: " + std::to_string(bufferId));
                Log(";(");
            }
        }
    }
    
    template class VertexBuffer<GLfloat>;
    template class VertexBuffer<GLushort>;
}
