//
//  VertexBuffer.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-20.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__VertexBuffer__
#define __cocos3d_libs__VertexBuffer__

#include "M3DBase.h"

namespace my3d
{

    enum class BufferType
    {
        Vertex = 0,
        Index = 1
    };

    enum class BufferUsage
    {
        Static = 0,
        Dynamic = 1
    };
    
    enum class IndexType
    {
        Index8,
        Index16,
        Index32,
    };
    
    GLenum indexType2Sys(IndexType type);

    class BufferBase : public cocos2d::ISmartObject
    {
        BufferBase(const BufferBase &);
        const BufferBase & operator = (const BufferBase &);
        
    public:
        
        BufferBase(BufferType type, BufferUsage usage);
        virtual ~BufferBase();
        
        bool isValid() const;
        
        void resize(size_t size, void *data = nullptr);
        void fill(size_t offset, size_t size, void *data);
        
        virtual void bind() = 0;
        virtual void unbind() = 0;
        
    private:
        
        bool init();
        void destroy();
        
    protected:
        BufferType  m_type;
        BufferUsage m_usage;
        size_t m_size;
        GLuint m_vb;
    };

    //顶点缓冲区
    class VertexBuffer : public BufferBase
    {
    public:
        
        VertexBuffer(BufferUsage usage, size_t size, void *data=nullptr);
        ~VertexBuffer();
        
        virtual void bind();
        virtual void unbind();
    };

    //索引缓冲区
    class IndexBuffer : public BufferBase
    {
        IndexType m_indexType;
    public:
        
        IndexBuffer(BufferUsage usage, size_t size, void *data=nullptr);
        ~IndexBuffer();
        
        void setIndexType(IndexType type);
        IndexType getIndexType();
        
        virtual void bind();
        virtual void unbind();
    };
    
    typedef SmartPtr<VertexBuffer>  VertexBufferPtr;
    typedef SmartPtr<IndexBuffer>   IndexBufferPtr;
    
}//end namespace my3d


#endif /* defined(__cocos3d_libs__VertexBuffer__) */
