//
//  TestGroundDraw.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestGroundDraw.h"


TestGroundDrawNode::TestGroundDrawNode()
{
    
}

TestGroundDrawNode::~TestGroundDrawNode()
{
    
}


bool TestGroundDrawNode::initTest3D()
{
    const int nRows = 10;
    const int nCols = 8;
    
    const int nRowVertices = nRows + 1;
    const int nColVertices = nCols + 1;
    
    const int nVertices = nRowVertices * nColVertices;
    
    const int nFaces = nRows * nCols * 2;
    const int nIndices = nFaces * 3;
    
    const float gridWidth = 4.0f;
    const float gridHeight = 3.0f;
    const float halfWidth = nCols * gridWidth * 0.5f;
    const float halfHeight = nRows * gridHeight * 0.5f;
    const float y = -4.0f;
    
    //create vertex buffer
    
    my3d::VertexXYZUV vertices[nVertices];
    for (int r = 0; r < nRowVertices; ++r)
    {
        for(int c = 0; c < nColVertices; ++c)
        {
            int i = r * nColVertices + c;
            vertices[i].position.set(c * gridWidth - halfWidth, y, r * gridHeight - halfHeight);
            vertices[i].uv.set(c, r);
        }
    }
    
    my3d::VertexBufferPtr vb = new my3d::VertexBuffer(
        my3d::BufferUsage::Static, nVertices * sizeof(my3d::VertexXYZUV), vertices );
    
    //create index buffer
    
    my3d::uint16 indices[nIndices];
    my3d::uint16 *p = indices;
    for(int r = 0; r < nRows; ++r)
    {
        for(int c = 0; c < nCols; ++c)
        {
            int i = r * nColVertices + c;
            p[0] = i;
            p[1] = i + nColVertices;
            p[2] = i + 1;
            
            p[3] = p[2];
            p[4] = p[1];
            p[5] = p[4] + 1;
            
            p += 6;
        }
    }
    
    my3d::IndexBufferPtr ib = new my3d::IndexBuffer(my3d::BufferUsage::Static,
        my3d::IndexType::Index16, sizeof(my3d::uint16) * nIndices, indices);
    
    //create material
    
    my3d::MaterialPtr mtl = new my3d::Material();
    mtl->setEffect(my3d::EffectMgr::instance()->get("effect/test2.shader"));
    
    my3d::TextureStage stage;
    stage.setTexture(my3d::TextureMgr::instance()->get("HelloWorld.png"));
    stage.setUWrap(my3d::TextureWrap::Repeat);
    stage.setVWrap(my3d::TextureWrap::Mirror);
    stage.setMinFilter(my3d::TextureFilter::Linear);
    stage.setMagFilter(my3d::TextureFilter::Linear);
    mtl->addTextureStage(stage);
    
    //create a sub mesh
    
    my3d::SubMeshPtr sub = new my3d::SubMesh();
    sub->setMaterial(mtl);
    sub->setPrimitive(my3d::PrimitiveType::TriangleList, 0, nIndices);
    
    //create vertex declaration
    
    my3d::VertexDeclarationPtr decl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZUV::getType());
    
    //create the ground mesh
    
    m_ground = new my3d::Mesh();
    m_ground->setVertexBuffer(vb);
    m_ground->setVertexDecl(decl);
    m_ground->setIndexBuffer(ib);
    m_ground->addSubMeshes(sub);
    
    return true;
}

void TestGroundDrawNode::draw()
{
    my3d::renderDev()->pushWorld(this->getWorldMatrix());

    TestBaseNode::draw();
    if(m_ground) m_ground->draw();
    
    my3d::renderDev()->popWorld();
}