//
//  VertexBuffer.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-20.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DVertexBuffer.h"

namespace my3d
{

    GLenum bufferUsage2GL(BufferUsage usage)
    {
        switch(usage)
        {
            case BufferUsage::Dynamic:
                return GL_DYNAMIC_DRAW;
                
            default:
                return GL_STATIC_DRAW;
        };
    }

    GLenum bufferType2GL(BufferType type)
    {
        switch(type)
        {
            case BufferType::Vertex:
                return GL_ARRAY_BUFFER;
                
            case BufferType::Index:
                return GL_ELEMENT_ARRAY_BUFFER;
                
            default:
                assert("unsupported buffer type!");
                return -1;
        };
    }

    //////////////////////////////////////////////////////////////////////

    BufferBase::BufferBase(BufferType type, BufferUsage usage)
    : m_type(type)
    , m_usage(usage)
    , m_size(0)
    , m_vb(0)
    {
        init();
    }

    BufferBase::~BufferBase()
    {
        destroy();
    }

    bool BufferBase::isValid() const
    {
        return m_vb != 0;
    }

    void BufferBase::resize(size_t size, void *data)
    {
        if(isValid())
        {
            bind();
            glBufferData(bufferType2GL(m_type), size, data, bufferUsage2GL(m_usage));
            unbind();
            
            m_size = size;
        }
    }

    void BufferBase::setVertex(size_t offset, size_t size, void *data)
    {
        bind();
        glBufferSubData(bufferType2GL(m_type), offset, size, data);
        unbind();
    }


    bool BufferBase::init()
    {
        glGenBuffers(1, &m_vb);
        return m_vb != 0;
    }

    void BufferBase::bind()
    {
        glBindBuffer(bufferType2GL(m_type), m_vb);
    }

    void BufferBase::unbind()
    {
        glBindBuffer(bufferType2GL(m_type), 0);
    }

    void BufferBase::destroy()
    {
        if(isValid())
        {
            glDeleteBuffers(1, &m_vb);
            m_vb = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////

    VertexBuffer::VertexBuffer(BufferUsage usage, size_t size, void *data)
    : BufferBase(BufferType::Vertex, usage)
    {
        resize(size, data);
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    //////////////////////////////////////////////////////////////////////

    IndexBuffer::IndexBuffer(BufferUsage usage, size_t size, void *data)
    : BufferBase(BufferType::Index, usage)
    {
        resize(size, data);
    }

    IndexBuffer::~IndexBuffer()
    {
        
    }
        

}//end namespace my3d
