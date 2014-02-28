//
//  TestSimpleDraw.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestSimpleDraw.h"


TestSimpleDrawNode::TestSimpleDrawNode()
: m_vertexBuffer(nullptr)
, m_vertexDecl(nullptr)
, m_indexBuffer(nullptr)
{
    
}
    
TestSimpleDrawNode::~TestSimpleDrawNode()
{
}


bool TestSimpleDrawNode::initTest3D()
{
    //create vertex buffer
    
    const float size = 1.0f;
    
    const int numVertices = 8;
    my3d::VertexXYZColor vertices[numVertices];
    
    vertices[0].position.set(-size, -size, size);
    vertices[0].color.set(0.0f, 0.0f, 1.0f, 1.0f);
    
    vertices[1].position.set(-size, size, size);
    vertices[1].color.set(0.0f, 1.0f, 0.0f, 1.0f);
    
    vertices[2].position.set(size, size, size);
    vertices[2].color.set(0.0f, 1.0f, 1.0f, 1.0f);
    
    vertices[3].position.set(size, -size, size);
    vertices[3].color.set(1.0f, 0.0f, 0.0f, 1.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        vertices[i + 4] = vertices[i];
        vertices[i + 4].position.z = -size;
    }
    
    m_vertexBuffer = new my3d::VertexBufferEx<my3d::VertexXYZColor>(
        my3d::BufferUsage::Static, numVertices, &vertices[0]);
    
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
    
    m_indexBuffer = new my3d::IndexBufferEx<my3d::uint8>(
         my3d::BufferUsage::Static, nIndices, indices);
    m_nIndices = nIndices;
    
    //shader
    m_effect = my3d::EffectMgr::instance()->get("effect/test1.shader");
    
    //vertex declaration
    m_vertexDecl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZColor::getType());
    
    
    return true;
}

void TestSimpleDrawNode::draw()
{
    this->rotateY(0.01f);
    
    my3d::renderDev()->pushWorld(this->getWorldMatrix());
    
    TestBaseNode::draw();
    
    my3d::renderDev()->setRenderState(my3d::RenderState::CullFace, true);
    my3d::renderDev()->setCullFace(my3d::CullFace::Back);
    
    if(m_vertexBuffer && m_indexBuffer && m_vertexDecl)
    {
        m_vertexBuffer->bind();
        m_indexBuffer->bind();
        m_vertexDecl->bind();
        
        if(m_effect && m_effect->begin())
        {
            my3d::renderDev()->drawIndexedPrimitive(my3d::PrimitiveType::TriangleList, 0, m_nIndices);
            
            m_effect->end();
        }
        
        m_indexBuffer->unbind();
        m_vertexBuffer->unbind();
    }
    
    my3d::renderDev()->popWorld();
    
}