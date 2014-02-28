//
//  TestLightDraw.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestLightDraw.h"

TestLightDrawNode::TestLightDrawNode()
{
    
}

TestLightDrawNode::~TestLightDrawNode()
{
    
}

bool TestLightDrawNode::initTest3D()
{
    //create vertex buffer
    
    typedef my3d::VertexXYZNUV VertexType;
    
    const float size = 1.0f;
    
    const int numVertices = 8;
    VertexType vertices[numVertices];
    
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
        vertices[i + 4].position = vertices[i].position;
        vertices[i + 4].position.z = -size;
    }
    
    vertices[4].uv.set(1, 1);
    vertices[5].uv.set(1, 0);
    vertices[6].uv.set(0, 0);
    vertices[7].uv.set(0, 1);
    
    for(int i = 0; i<numVertices; ++i)
    {
        //vertices[i].normal = vertices[i].position;
        //vertices[i].normal.normalize();
        vertices[i].normal.set(0, 0, 0);
    }
    
    //vertices[1].normal.set(0, 1, 0);
    
    my3d::VertexBufferPtr vb = new my3d::VertexBufferEx<VertexType>(
        my3d::BufferUsage::Static, numVertices, vertices);
    
    my3d::VertexDeclarationPtr decl = my3d::VertexDeclMgr::instance()->get(VertexType::getType());
    
    //create index buffer
    
    const int numIndices = 6 * 2 * 3;
    my3d::uint16 indices[numIndices] = {
        0, 2, 1,  0, 3, 2, //front
        3, 6, 2,  3, 7, 6, //right
        7, 5, 6,  7, 4, 5, //back
        4, 1, 5,  4, 0, 1, //left
        1, 6, 5,  1, 2, 6, //top
        4, 3, 0,  4, 7, 3, //bottom
    };
    
    my3d::IndexBufferPtr ib = new my3d::IndexBufferEx<my3d::uint16>(
        my3d::BufferUsage::Static, numIndices, indices);
    
    //create material
    
    my3d::MaterialPtr mtl = new my3d::Material();
    mtl->setEffect(my3d::EffectMgr::instance()->get("effect/test3.shader"));
    
    my3d::TextureStage stage;
    stage.setTexture(my3d::TextureMgr::instance()->get("HelloWorld.png"));
    stage.setUWrap(my3d::TextureWrap::Repeat);
    stage.setVWrap(my3d::TextureWrap::Repeat);
    stage.setMinFilter(my3d::TextureFilter::Linear);
    stage.setMagFilter(my3d::TextureFilter::Linear);
    mtl->addTextureStage(stage);
    
    my3d::MaterialColor color;
    color.ambient.set(0.5f, 0.5f, 0.5f, 1.0f);
    mtl->setMaterialColor(color);
    
    //create a sub mesh
    
    my3d::SubMeshPtr subMesh = new my3d::SubMesh();
    subMesh->setPrimitive(my3d::PrimitiveType::TriangleList, 0, numIndices);
    subMesh->setMaterial(mtl);
    
    //create mesh
    
    m_mesh = new my3d::Mesh();
    m_mesh->setVertexBuffer(vb);
    m_mesh->setVertexDecl(decl);
    m_mesh->setIndexBuffer(ib);
    m_mesh->addSubMeshes(subMesh);
    return true;
}

void TestLightDrawNode::draw()
{
    //this->rotateY(0.01f);
    
    my3d::renderDev()->pushWorld(this->getWorldMatrix());
    
    TestBaseNode::draw();
    if(m_mesh) m_mesh->draw();
    
    my3d::renderDev()->popWorld();
}