//
//  VertexBuffer.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-20.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DVertexBuffer.h"
#include "M3DRenderDevice.h"

namespace my3d
{

    GLenum bufferUsage2GL(BufferUsage usage)
    {
        switch(usage)
        {
            case BufferUsage::Dynamic:
                return GL_DYNAMIC_DRAW;
                
            case BufferUsage::Static:
                return GL_STATIC_DRAW;
                
            default:
                CCAssert(0, "Invalid BufferUsage");
                return 0;
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
                CCAssert(0, "Invalid BufferType");
                return 0;
        };
    }
    
    GLenum indexType2Sys(IndexType type)
    {
        switch(type)
        {
            case IndexType::Index8:
                return GL_UNSIGNED_BYTE;
                
            case IndexType::Index16:
                return GL_UNSIGNED_SHORT;
                
            case IndexType::Index32:
                return GL_UNSIGNED_INT;
                
            default:
                CCAssert(0, "Invalid IndexType");
                return 0;
        };
    }
    
    size_t indexType2Size(IndexType type)
    {
        switch(type)
        {
            case IndexType::Index8:
                return sizeof(uint8);
            
            case IndexType::Index16:
                return sizeof(uint16);
            
            case IndexType::Index32:
                return sizeof(uint32);
            
            default:
                CCAssert(0, "Invalid IndexType");
            return 0;
        }
    }
    
    IndexType size2IndexType(size_t n)
    {
        if(n <= sizeof(uint8))
            return IndexType::Index8;
        
        else if(n <= sizeof(uint16))
            return IndexType::Index16;
        
        else
            return IndexType::Index32;
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

    void BufferBase::resize(size_t stride, size_t nCount, void *data)
    {
        if(isValid())
        {
            m_size = stride * nCount;
            
            bind();
            GL_ASSERT( glBufferData(bufferType2GL(m_type), m_size, data, bufferUsage2GL(m_usage)) );
            unbind();
        }
    }

    void BufferBase::fill(size_t stride, size_t iStart, size_t nCount, void *data)
    {
        CCAssert((iStart + nCount) * stride <= m_size, "BufferBase::fill - invalid offset and size!");
        
        bind();
        GL_ASSERT( glBufferSubData(bufferType2GL(m_type), iStart * stride, nCount * stride, data) );
        unbind();
    }


    bool BufferBase::init()
    {
        GL_ASSERT( glGenBuffers(1, &m_vb) );
        return m_vb != 0;
    }

    void BufferBase::destroy()
    {
        if(isValid())
        {
            GL_ASSERT( glDeleteBuffers(1, &m_vb) );
            m_vb = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////

    VertexBuffer::VertexBuffer(BufferUsage usage, size_t stride, size_t nVertex, void *data)
    : BufferBase(BufferType::Vertex, usage)
    {
        resize(stride, nVertex, data);
    }

    VertexBuffer::~VertexBuffer()
    {
        renderDev()->unsetVertexBuffer(this);
    }
    
    void VertexBuffer::bind()
    {
        GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_vb) );
        
        renderDev()->setVertexBuffer(this);
    }
    
    void VertexBuffer::unbind()
    {
        renderDev()->unsetVertexBuffer(this);
    }

    //////////////////////////////////////////////////////////////////////

    IndexBuffer::IndexBuffer(BufferUsage usage, size_t stride, size_t nCount, void *data)
    : BufferBase(BufferType::Index, usage)
    , m_indexType(size2IndexType(stride))
    {
        resize(stride, nCount, data);
    }

    IndexBuffer::~IndexBuffer()
    {
        renderDev()->unsetIndexBuffer(this);
    }
    
    void IndexBuffer::setIndexType(IndexType type)
    {
        m_indexType = type;
    }
    
    IndexType IndexBuffer::getIndexType()
    {
        return m_indexType;
    }
    
    void IndexBuffer::bind()
    {
        GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vb) );
        
        renderDev()->setIndexBuffer(this);
    }
    
    void IndexBuffer::unbind()
    {
        renderDev()->unsetIndexBuffer(this);
    }


}//end namespace my3d
