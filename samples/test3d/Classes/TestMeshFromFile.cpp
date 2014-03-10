//
//  TestMeshFromFile.cpp
//  test3d
//
//  Created by zhoubao on 14-3-3.
//
//

#include "TestMeshFromFile.h"

/////////////////////////////////////////////////////////////////////

TestMeshFileNode::TestMeshFileNode()
: m_nRows(0)
, m_nCols(0)
{
    
}
TestMeshFileNode::~TestMeshFileNode()
{
    
}

bool TestMeshFileNode::initTest3D()
{
    int nRows = 4;
    int nCols = 5;
    
    if (!loadMaterial())
    {
        return false;
    }
    
    std::vector<int> rawData = {
      0, 0, 0, 0, 0,
      0, 1, 1, 1, 1,
      1, 2, 2, 2, 2,
      0, 1, 1, 1, 1,
    };
    
    generateGround(nRows, nCols, &rawData[0]);
    
    const int step = 4;
    std::vector<int> wall = {
        0, 2, 0, -1,
        5, 0, 2, 1,
        6, 2, 0, 1,
        11, 0, 2, 1000,
        2, 2, 0, -1,
        3, 2, 0, -1,
        4, 2, 0, -1,
        10, 2, 0, -1,
        21, 2, 0, -1,
        15, 2, 0, -1,
        20, 2, 0, -1,
        19, 2, 0, -1,
        28, 2, 0, 1000,
    };
    generateWall(nRows, nCols, &wall[0], wall.size() / step, step);
    
    
    cocos2d::C3DScene *pScene = cocos2d::C3DLayer::getMainLayer()->get3DScene();
    pScene->getActiveCamera()->setPosition(2, 8.0f, 8.0f);
    my3d::renderDev()->updateCamera();
    
    return true;
}

void TestMeshFileNode::draw()
{
    this->rotateY(0.01f);
    
    my3d::renderDev()->pushWorld(this->getWorldMatrix());
    
    TestBaseNode::draw();
    if(m_ground) m_ground->draw();
    if(m_wallMesh) m_wallMesh->draw();
    
    my3d::renderDev()->popWorld();
}

void TestMeshFileNode::generateGround(int nRows, int nCols, const int *rawData)
{
    std::map<int, std::vector<int>> primitiveGroup;//material index -> grid index
    generatePrimitive(nRows, nCols, rawData, primitiveGroup);
    
    const size_t nVertices = (nRows + 1) * (nCols + 1);
    std::vector<my3d::VertexXYZNUV> vertices(nVertices);
    generateVertices(nRows, nCols, &vertices[0]);
    my3d::VertexBufferPtr vb = new my3d::VertexBufferEx<my3d::VertexXYZNUV>(my3d::BufferUsage::Static, nVertices, &vertices[0]);
    
    my3d::VertexDeclarationPtr decl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZNUV::getType());
    
    m_ground = new my3d::Mesh();
    
    const size_t nIndices = nRows * nCols * 2 * 3;
    std::vector<my3d::uint16> indices(nIndices);
    generateIndices(nRows, nCols, &indices[0]);
    
    std::vector<my3d::uint16> dstIndices;
    dstIndices.reserve(nIndices);
    
    size_t start = 0;
    for (auto primitive : primitiveGroup)
    {
        if(primitive.second.empty()) continue;
        
        for(auto grid : primitive.second)//ig is the index of grid
        {
            size_t iIndex = grid * 6;
            for(size_t j = 0; j < 6; ++j)
            {
                dstIndices.push_back(indices[iIndex + j]);
            }
        }
        
        size_t n = primitive.second.size() * 6;
        
        my3d::SubMeshPtr mesh = new my3d::SubMesh();
        mesh->setMaterial(m_materials[primitive.first]);
        mesh->setPrimitive(my3d::PrimitiveType::TriangleList, start, n);
        m_ground->addSubMeshes(mesh);
        
        start += n;
        
    }
    
    my3d::IndexBufferPtr ib = new my3d::IndexBufferEx<my3d::uint16>(my3d::BufferUsage::Static, dstIndices.size(), &dstIndices[0]);
    m_ground->setIndexBuffer(ib);
    m_ground->setVertexBuffer(vb);
    m_ground->setVertexDecl(decl);
}

void TestMeshFileNode::generateVertices(int nRows, int nCols, my3d::VertexXYZNUV * pVertices)
{
    const int nRowVertices = nRows + 1;
    const int nColVertices = nCols + 1;
    
    const float gridWidth = 1.0f;
    const float gridHeight = 1.0f;
    const float y = 0.0f;
    
    for (int r = 0; r < nRowVertices; ++r)
    {
        for(int c = 0; c < nColVertices; ++c)
        {
            int i = r * nColVertices + c;
            pVertices[i].position.set(c * gridWidth, y, r * gridHeight);
            pVertices[i].normal.set(0, 1, 0);
            pVertices[i].uv.set(c, r);
        }
    }
}

void TestMeshFileNode::generateIndices(int nRows, int nCols, my3d::uint16 * pIndices)
{
    const int nColVertices = nCols + 1;
    
    my3d::uint16 *p = pIndices;
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
}

//将材质相同的格子，放到一起
void TestMeshFileNode::generatePrimitive(int nRows, int nCols, const int *rawData, std::map<int, std::vector<int>> & group)
{
    const int nGrid = nRows * nCols;
    for (int i = 0; i < nGrid; ++i)
    {
        int iMateral = rawData[i];
        group[iMateral].push_back(i);
    }
}

bool TestMeshFileNode::loadMaterial()
{
    {
        my3d::MaterialPtr mtl = new my3d::Material();
        mtl->setEffect(my3d::EffectMgr::instance()->get("effect/test3.shader"));
        
        my3d::TextureStage stage;
        stage.setTexture(my3d::TextureMgr::instance()->get("gui/stone.png"));
        stage.setUWrap(my3d::TextureWrap::Repeat);
        stage.setVWrap(my3d::TextureWrap::Repeat);
        stage.setMinFilter(my3d::TextureFilter::Linear);
        stage.setMagFilter(my3d::TextureFilter::Linear);
        mtl->addTextureStage(stage);
        
        my3d::MaterialColor color;
        color.ambient.set(0.5f, 0.5f, 0.5f, 1.0f);
        color.diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
        color.specular.set(0.8f, 0.8f, 0.8f, 1.0f);
        mtl->setMaterialColor(color);
        
        m_materials.push_back(mtl);
    }
    
    {
        my3d::MaterialPtr mtl = new my3d::Material();
        mtl->setEffect(my3d::EffectMgr::instance()->get("effect/test3.shader"));
        
        my3d::TextureStage stage;
        stage.setTexture(my3d::TextureMgr::instance()->get("gui/background.png"));
        stage.setUWrap(my3d::TextureWrap::Repeat);
        stage.setVWrap(my3d::TextureWrap::Repeat);
        stage.setMinFilter(my3d::TextureFilter::Linear);
        stage.setMagFilter(my3d::TextureFilter::Linear);
        mtl->addTextureStage(stage);
        
        my3d::MaterialColor color;
        color.ambient.set(0.5f, 0.5f, 0.5f, 1.0f);
        color.diffuse.set(1.0f, 1.8f, 1.0f, 1.0f);
        color.specular.set(0.8f, 0.8f, 0.8f, 1.0f);
        mtl->setMaterialColor(color);
        
        m_materials.push_back(mtl);
    }
    
    {
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
        color.ambient.set(0.0f, 0.0f, 0.0f, 1.0f);
        color.diffuse.set(1.0f, 1.2f, 1.2f, 1.2f);
        color.specular.set(0.8f, 0.8f, 0.8f, 1.0f);
        mtl->setMaterialColor(color);
        
        m_materials.push_back(mtl);
    }
    
    return true;
}

void TestMeshFileNode::generateWall(int nRows, int nCols, const int * pWalls, int n, int step)
{
    m_wallData = new Wall();
    m_wallData->init(nRows, nCols, pWalls, n, step);
    
    Wall::VertexPool vertices;
    Wall::FaceMap faces;
    m_wallData->generateVertex(vertices, faces);
    
    m_wallMesh = new my3d::Mesh();
    
    //构造顶点的索引数据
    Wall::IndexPool indices;
    size_t start = 0;
    for(auto it : faces)
    {
        size_t n = it.second.size();
        if(n == 0) continue;
        for(size_t i = 0; i<n; ++i)
        {
            indices.push_back(it.second[i]);
        }
        
        my3d::SubMesh * pMesh = new my3d::SubMesh();
        pMesh->setMaterial(m_materials[it.first]);
        pMesh->setPrimitive(my3d::PrimitiveType::TriangleList, start, n);
        m_wallMesh->addSubMeshes(pMesh);
        
        start += n;
    }
    
    m_wallMesh->setVertexBuffer(
        new my3d::VertexBufferEx<my3d::VertexXYZNUV>(my3d::BufferUsage::Static, vertices.size(), &vertices[0]));
    m_wallMesh->setIndexBuffer(
       new my3d::IndexBufferEx<my3d::uint16>(my3d::BufferUsage::Static, indices.size(), &indices[0]));
    m_wallMesh->setVertexDecl(my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZNUV::getType()));
    
}
