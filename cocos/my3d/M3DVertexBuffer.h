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
        IndexError,
    };
    
    GLenum indexType2Sys(IndexType type);
    IndexType size2IndexType(size_t n);

    class BufferBase : public cocos2d::ISmartObject
    {
        BufferBase(const BufferBase &);
        const BufferBase & operator = (const BufferBase &);
        
    public:
        
        BufferBase(BufferType type, BufferUsage usage);
        virtual ~BufferBase();
        
        bool isValid() const;
        
        void resize(size_t stride, size_t nCount, void *data = nullptr);
        void fill(size_t stride, size_t iStart, size_t nCount, void *data);
        
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
        
        VertexBuffer(BufferUsage usage, size_t stride, size_t nVertex, void *data=nullptr);
        ~VertexBuffer();
        
        virtual void bind();
        virtual void unbind();
    };
    
    
    template<typename TVertex>
    class VertexBufferEx : public VertexBuffer
    {
    public:
        
        VertexBufferEx(BufferUsage usage, size_t nVertex, TVertex *data=nullptr)
        : VertexBuffer(usage, sizeof(TVertex), nVertex, data)
        {
            
        }
        
        void resizeBuffer(size_t nVertex, TVertex *data = nullptr)
        {
            resize(nVertex * sizeof(TVertex), data);
        }
        
        void fillBuffer(size_t iStart, size_t nVertex, TVertex *data)
        {
            file(iStart * sizeof(TVertex), nVertex * sizeof(TVertex), data);
        }
        
    };
    

    //索引缓冲区
    class IndexBuffer : public BufferBase
    {
    public:
        IndexBuffer(BufferUsage usage, size_t stride, size_t nCount, void *data=nullptr);
        ~IndexBuffer();
        
        virtual void setIndexType(IndexType type);
        IndexType getIndexType();
        
        virtual void bind();
        virtual void unbind();
        
    private:
        IndexType m_indexType;
    };
    
    template<typename TIndex>
    class IndexBufferEx : public IndexBuffer
    {
    public:
        
        IndexBufferEx(BufferUsage usage, size_t nIndex, TIndex *data=nullptr)
        : IndexBuffer(usage, sizeof(TIndex), nIndex, data)
        {
        }
        
        void setIndexType(IndexType type){}
        
        void resizeBuffer(size_t nIndex, TIndex *data = nullptr)
        {
            resize(sizeof(TIndex), nIndex, data);
        }
        
        void fillBuffer(size_t iStart, size_t nIndex, TIndex *data)
        {
            file(sizeof(TIndex), iStart, nIndex, data);
        }
    };
    
    typedef SmartPtr<VertexBuffer>  VertexBufferPtr;
    typedef SmartPtr<IndexBuffer>   IndexBufferPtr;
    
}//end namespace my3d


#endif /* defined(__cocos3d_libs__VertexBuffer__) */
