//
//  VertexBuffer.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-20.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__VertexBuffer__
#define __cocos3d_libs__VertexBuffer__

#include "../EnumDef.h"
#include "cocos2d.h"

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

    class BufferBase : public cocos2d::Object
    {
        BufferBase(const BufferBase &);
        const BufferBase & operator = (const BufferBase &);
        
    public:
        
        BufferBase(BufferType type, BufferUsage usage);
        virtual ~BufferBase();
        
        bool isValid() const;
        
        void resize(size_t size, void *data = nullptr);
        void setVertex(size_t offset, size_t size, void *data);
        
        void bind();
        void unbind();
        
    private:
        
        bool init();
        void destroy();
        
    private:
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
        
    };

    //索引缓冲区
    class IndexBuffer : public BufferBase
    {
    public:
        
        IndexBuffer(BufferUsage usage, size_t size, void *data=nullptr);
        ~IndexBuffer();
    };
    
}//end namespace my3d


#endif /* defined(__cocos3d_libs__VertexBuffer__) */
