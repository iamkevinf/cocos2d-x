//
//  TestTextureDraw.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestTextureDraw.h"


TestTextureDrawNode::TestTextureDrawNode()
: m_nIndices(0)
{
    
}
TestTextureDrawNode::~TestTextureDrawNode()
{
    
}

bool TestTextureDrawNode::initTest3D()
{
    //create vertex buffer
    const float size = 1.0f;
    
    const int numVertices = 8;
    my3d::VertexXYZUV vertices[numVertices];
    
    vertices[0].position.set(-size, -size, size);
    vertices[0].uv.set(0.0f, 1.0f);
    
    vertices[1].position.set(-size, size, size);
    vertices[1].uv.set(0.0f, 0.0f);
    
    vertices[2].position.set(size, size, size);
    vertices[2].uv.set(1.0f, 0.0f);
    
    vertices[3].position.set(size, -size, size);
    vertices[3].uv.set(1.0f, 1.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        vertices[i + 4] = vertices[i];
        vertices[i + 4].position.z = -size;
    }
    
    vertices[4].uv.set(1, 1);
    vertices[5].uv.set(1, 0);
    vertices[6].uv.set(0, 0);
    vertices[7].uv.set(0, 1);
    
    m_vertexBuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
                                            numVertices * sizeof(my3d::VertexXYZUV), &vertices[0]);
    
    //create index buffer
    
    const int nIndices = 6 * 2 * 3;
    my3d::uint8 indices[nIndices] = {
        0, 2, 1,  0, 3, 2, //front
        3, 6, 2,  3, 7, 6, //right
        7, 5, 6,  7, 4, 5, //back
        4, 1, 5,  4, 0, 1, //left
        1, 6, 5,  1, 2, 6, //top
        4, 3, 0,  4, 7, 3, //bottom
    };
    
    m_indexBuffer = new my3d::IndexBuffer(my3d::BufferUsage::Static,
                                          my3d::IndexType::Index8, nIndices * sizeof(my3d::uint8), indices);
    m_nIndices = nIndices;
    
    m_vertexDecl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZUV::getType());
    m_effect = my3d::EffectMgr::instance()->get("effect/test2.shader");
    m_texture = my3d::TextureMgr::instance()->get("HelloWorld.png");
    
    return true;
}

void TestTextureDrawNode::draw()
{
    this->rotateY(0.01f);
    
    my3d::renderDev()->pushWorld(this->getWorldMatrix());
    
    TestBaseNode::draw();
    
    if(m_vertexBuffer && m_indexBuffer && m_vertexDecl)
    {
        m_vertexBuffer->bind();
        m_indexBuffer->bind();
        m_vertexDecl->bind();
        
        if (m_effect && m_effect->begin())
        {
            my3d::EffectConstant *pConst = m_effect->getConstant(my3d::EffectConstType::Texture0);
            if(pConst) pConst->bindValue(m_texture);
            
            my3d::renderDev()->drawIndexedPrimitive(my3d::PrimitiveType::TriangleList, 0, m_nIndices);
            
            m_effect->end();
        }
        
        m_vertexDecl->unbind();
        m_indexBuffer->unbind();
        m_vertexBuffer->unbind();
    }
    
    my3d::renderDev()->popWorld();
}