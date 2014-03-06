//
//  TestMeshFromFile.cpp
//  test3d
//
//  Created by zhoubao on 14-3-3.
//
//

#include "TestMeshFromFile.h"

const WallMap::GridSide g_invGridSide[] = {WallMap::GridDown, WallMap::GridRight, WallMap::GridLeft, WallMap::GridUp};

/*static*/ int WallMap::getInvGridSide(int side)
{
    return g_invGridSide[side];
}

WallMap::WallMap()
: m_nRows(0)
, m_nCols(0)
{
    
}

WallMap::~WallMap()
{
    
}


void WallMap::reset(int nRows, int nCols)
{
    m_nRows = nRows;
    m_nCols = nCols;
    
    m_grids.resize(nRows * nCols);
    m_walls.resize(2 * nRows * nCols + nRows + nCols);
    
    int iWall = 0;
    for (Wall & it : m_walls)
    {
        it.index = iWall;
        it.grid[0] = it.grid[1] = -1;
        it.dir = -1;
    }
    
    //处理墙和格子的映射关系
    for(int r = 0; r < m_nRows; ++r)
    {
        for(int c = 0; c < m_nCols; ++c)
        {
            int iGrid = r * m_nCols + c;
            m_grids[iGrid].index = iGrid;
            
            int *pWall = m_grids[iGrid].wall;
            
            int uWall = 2 *r * m_nCols + r + c;
            int lWall = uWall + m_nCols;
            int rWall = lWall + 1;
            int dWall = rWall + m_nCols;
            
            pWall[GridUp] = uWall;
            pWall[GridLeft] = lWall;
            pWall[GridRight] = rWall;
            pWall[GridDown] = dWall;
            
            /*左&上为0，右&下为1
            ---0---
            |1 i 2|
            ---3---
            */
            
            int iVertex = r * (m_nCols + 1) + c;
            
            m_walls[uWall].grid[WallOutside] = iGrid;
            m_walls[uWall].dir = 0;
            m_walls[uWall].start = iVertex;
            m_walls[uWall].end = m_walls[uWall].start + 1;
            
            m_walls[lWall].grid[WallOutside] = iGrid;
            m_walls[lWall].dir = 1;
            m_walls[lWall].start = iVertex;
            m_walls[lWall].end = m_walls[lWall].start + (m_nCols + 1);
            
            m_walls[rWall].grid[WallInside] = iGrid;
            m_walls[rWall].dir = 1;
            m_walls[rWall].start = iVertex + 1;
            m_walls[rWall].end = m_walls[rWall].start + (m_nCols + 1);
            
            m_walls[dWall].grid[WallInside] = iGrid;
            m_walls[dWall].dir = 0;
            m_walls[dWall].start = iVertex + (m_nCols + 1);
            m_walls[dWall].end = m_walls[dWall].start + 1;
        }
    }
}

int WallMap::wall2grid(int iWall, int i) const
{
    return m_walls[iWall].grid[i];
}

int WallMap::grid2wall(int iGrid, int i) const
{
    return m_grids[iGrid].wall[i];
}

const WallMap::Grid & WallMap::getGrid(int iGrid) const
{
    return m_grids[iGrid];
}

const WallMap::Wall & WallMap::getWall(int iWall) const
{
    return m_walls[iWall];
}

//////////////////////////////////////////////////////////////
Wall::Wall()
: m_nRows(0)
, m_nCols(0)
, m_gridSize(1.0f)
, m_y(0.0f)
, m_wallHeight(2.0f)
, m_wallThick(0.1f)
, m_defaultMaterial(0)
{
    m_wallThickHalf = m_wallThick * 0.5f;
}

Wall::~Wall()
{
    
}

//pWalls：3个整数为一组（墙索引，材质1，材质2）
void Wall::init(int nRows, int nCols, const int * pWalls, int n)
{
    built(nRows, nCols);
    setData(pWalls, n);
}

void Wall::generateVertex(VertexPool & vertices, FaceMap & faces)
{
    generateVertexC(vertices, faces);
}


struct WallVertex
{
    WallVertex()
    {
        vertex[0][0] = -1;
        vertex[0][1] = -1;
        vertex[1][0] = -1;
        vertex[1][0] = -1;
    }
    
    int vertex[2][2];//两个面，每个面有两个顶点
};

namespace EdgeDir
{
    const int Up    = 1 << 0;
    const int Right = 1 << 1;
    const int Down  = 1 << 2;
    const int Left  = 1 << 3;
    
    const int Min = Up;
    const int Max = Left;
}

const my3d::uint32 EdgeShift = 16;
const my3d::uint32 EdgeMark = 0xffff;

int getEdgeDir(int start, int end)
{
    int delta = end - start;
    if(delta > 1) return EdgeDir::Down;//下
    if(delta == 1) return EdgeDir::Right;//右
    if(delta == -1) return EdgeDir::Left;//左
    return EdgeDir::Up;//上
}

int getEdgeMerge(int start, int end)
{
    return (start << EdgeShift) + end;
}

inline bool isEqual(float a, float b)
{
    return fabs(a - b) < 0.0001f;
}

inline int cmpFloat(float a, float b)
{
    if(isEqual(a, b)) return 0;
    if(a > b) return 1;
    return -1;
}

bool lessPositionXZ(const cocos2d::Vector3 & p1, const cocos2d::Vector3 & p2)
{
    int r = cmpFloat(p1.x, p2.x);
    if(r == 0) return cmpFloat(p1.z, p2.z) < 0;
    return r < 0;
}

void sortCrossVertex(std::vector<int> & indices, Wall::VertexPool & vertices)
{
    size_t n = indices.size();
    for(size_t i = 0; i < n-1; ++i)
    {
        size_t minI = i;
        for(size_t k = i + 1; k < n; ++k)
        {
            if(lessPositionXZ(vertices[indices[k]].position,
                              vertices[indices[minI]].position))
            {
                minI = k;
            }
        }
        
        if(i != minI) std::swap(indices[i], indices[minI]);
    }
}

int Wall::getNextVertex(int iVertex, int dir) const
{
    switch (dir)
    {
        case EdgeDir::Up:
            return iVertex - m_nCols - 1;
            
        case EdgeDir::Down:
            return iVertex + m_nCols + 1;
            
        case EdgeDir::Left:
            return iVertex - 1;
        
        case EdgeDir::Right:
            return iVertex + 1;
            break;
            
        default:
            return -1;
    }
}


struct WallEdge
{
    int wall;
    int material;
    int iStart;
    int iEnd;
    
    WallEdge()
    {}
    
    WallEdge(int wall_, int mtl, int start, int end)
    : wall(wall_), material(mtl), iStart(start), iEnd(end)
    {}
};

void dir2normal(my3d::Vector3 & normal, int dir)
{
    switch(dir)
    {
        case EdgeDir::Up:
            normal.set(-1, 0, 0);
            break;
            
        case EdgeDir::Down:
            normal.set(1, 0, 0);
            break;
            
        case EdgeDir::Left:
            normal.set(0, 0, 1);
            break;
            
        case EdgeDir::Right:
            normal.set(0, 0, -1);
            break;
    }
}

void Wall::generateVertexC(VertexPool & vertices, FaceMap & faces)
{
    VertexPool tempVertices;
    FaceMap tempFaces;
    
    const int nColVertices = m_nCols + 1;
    
    //一面墙的两个边
    typedef std::multimap<int, int> WallEdgeMap;
    WallEdgeMap wallEdge;
    
    //记录每个边的两个顶点。key是一个合成的数值：start << 16 + end
    std::map<my3d::uint32, WallEdge> edgeVertices;
    
    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        wallEdge.insert(std::pair<int, int>(w.start, w.end));
        wallEdge.insert(std::pair<int, int>(w.end, w.start));
        
        int dir = getEdgeDir(w.start, w.end);
        int iMtl = dir & (EdgeDir::Right | EdgeDir::Down) ? 0 : 1;
        edgeVertices[getEdgeMerge(w.start, w.end)] = WallEdge(it.first, it.second.material[iMtl], -1, -1);
        edgeVertices[getEdgeMerge(w.end, w.start)] = WallEdge(it.first, it.second.material[(iMtl+1)%2], -1, -1);
    }
    
#if 1
    CCLOG("num wall:%lu, num edge: %lu", m_walls.size(), wallEdge.size());
    for(auto it : wallEdge) CCLOG("%d -> %d", it.first, it.second);
#endif

    
    //每个交叉点生成的顶点
    std::map<int, std::vector<int>> crossVertices;
    
    int iStart = -1;
    int iEnd = -1;
    int iLastVertex = -1;
    int nRepeat = 0;
    int iWallVertex = -1;
    while(!wallEdge.empty())
    {
        WallEdgeMap::iterator itEdge;
        if(iEnd < 0)
        {
            itEdge = wallEdge.begin();
            
            iStart = itEdge->first;
            iEnd = itEdge->second;
            iLastVertex = -1;
            nRepeat = 0;
            iWallVertex = -1;
        }
        else
        {
            auto equal = wallEdge.equal_range(iStart);
            WallEdgeMap::iterator it = equal.first;
            for(; it != equal.second; ++it)
            {
                if(it->first == iStart && it->second == iEnd)
                    break;
            }
            
            if(it == equal.second)//出错了，不应该执行到这里
            {
                CCAssert(0, "Invalid edge!");
                
                iEnd = -1;
                continue;
            }
            
            itEdge = it;
        }
        
        int dir = getEdgeDir(iStart, iEnd);
        
        auto equal = wallEdge.equal_range(iEnd);
        int existMask = 0;
        for(auto it = equal.first; it != equal.second; ++it)
        {
            existMask |= getEdgeDir(it->first, it->second);
        }
        
        WallEdge & eVertex = edgeVertices[getEdgeMerge(iStart, iEnd)];
        if(iLastVertex >= 0)
        {
            eVertex.iStart = iLastVertex;
            
            //围墙的面
            if(nRepeat == 0)
            {
                iWallVertex = tempVertices.size();
                
                my3d::VertexXYZNUV vertex = vertices[iLastVertex];
                dir2normal(vertex.normal, dir);
                vertex.uv.set(0, 0);
                tempVertices.push_back(vertex);
                
                vertex.position.y = m_y;
                vertex.uv.set(0, 1);
                tempVertices.push_back(vertex);
            }
            
            if(eVertex.iEnd >= 0)//走完一圈了
            {
                IndexPool & pool = tempFaces[eVertex.material];
                
                my3d::VertexXYZNUV vertex = vertices[eVertex.iEnd];
                dir2normal(vertex.normal, dir);
                vertex.uv.set(nRepeat + 1, 0);
                tempVertices.push_back(vertex);
                
                vertex.position.y = m_y;
                vertex.uv.set(nRepeat + 1, 1);
                tempVertices.push_back(vertex);
                
                //两个三角形的6个索引
                pool.push_back(iWallVertex);
                pool.push_back(iWallVertex+1);
                pool.push_back(iWallVertex+2);
                pool.push_back(iWallVertex+2);
                pool.push_back(iWallVertex+1);
                pool.push_back(iWallVertex+3);
                
                
                wallEdge.erase(itEdge);
                
                CCLOG("%d->%d edge(%d, %d)", iStart, iEnd, eVertex.iStart, eVertex.iEnd);
                
                iEnd = -1;
                continue;
            }
            
        }
        
        
        my3d::VertexXYZNUV v;
        v.position.set((iEnd % nColVertices) * m_gridSize, m_y + m_wallHeight, (iEnd / nColVertices) * m_gridSize);
        v.normal.set(0, 1, 0);
        v.uv.set(0, 0);
        
        
        //顺时针判断下一步要走的方向
        int nextDir = dir == EdgeDir::Min ? EdgeDir::Max : dir >> 1;
        int i = 0;
        for(; i<4; ++i)
        {
            if(nextDir & existMask) break;
            
            nextDir <<= 1;
            if(nextDir > EdgeDir::Max) nextDir = EdgeDir::Min;
        }
        
        const float dirs[] = {-1, 0, 1, 0};
        float sign = dir & (EdgeDir::Up | EdgeDir::Right) ? -1 : 1;
        if(dir & (EdgeDir::Up | EdgeDir::Down))
        {
            v.position.x += sign * m_wallThickHalf;
            v.position.z += sign * dirs[i] * m_wallThickHalf;
        }
        else
        {
            v.position.z += sign * m_wallThickHalf;
            v.position.x -= sign * dirs[i] * m_wallThickHalf;
        }
        
        eVertex.iEnd = vertices.size();
        vertices.push_back(v);
        iLastVertex = eVertex.iEnd;
        crossVertices[iEnd].push_back(iLastVertex);
        
        CCLOG("%d->%d pos(%f, %f, %f) index(%d) edge(%d, %d)",
              iStart, iEnd, v.position.x, v.position.y, v.position.z, iLastVertex, eVertex.iStart, eVertex.iEnd);
        
        if(eVertex.iStart >= 0 && eVertex.iEnd >= 0)
        {
            wallEdge.erase(itEdge);
        }
        
        //围墙的面
        if(iWallVertex >= 0)
        {
            IndexPool & pool = tempFaces[eVertex.material];
            
            my3d::VertexXYZNUV vertex = v;
            dir2normal(vertex.normal, dir);
            vertex.uv.set(nRepeat + 1, 0);
            tempVertices.push_back(vertex);
            
            vertex.position.y = m_y;
            vertex.uv.set(nRepeat + 1, 1);
            tempVertices.push_back(vertex);
            
            //两个三角形的6个索引
            pool.push_back(iWallVertex);
            pool.push_back(iWallVertex+1);
            pool.push_back(iWallVertex+2);
            pool.push_back(iWallVertex+2);
            pool.push_back(iWallVertex+1);
            pool.push_back(iWallVertex+3);
            
            iWallVertex += 2;
        }
        
        //决定下一次需要选取的边
        int iNextEnd = getNextVertex(iEnd, nextDir);
        
        //反向的边
        if(iNextEnd == iStart)
        {
            //镜像出对边的起点
            my3d::VertexXYZNUV nextV = v;
            nextV.normal.set(0, 1, 0);
            float sign = nextDir & (EdgeDir::Up | EdgeDir::Right) ? -1 : 1;
            if(nextDir & (EdgeDir::Up | EdgeDir::Down))
            {
                nextV.position.x += sign * m_wallThick;
            }
            else
            {
                nextV.position.z += sign * m_wallThick;
            }
            
            iLastVertex = vertices.size();
            vertices.push_back(nextV);
            
            
            //意味着这是一面孤立的墙，需要绘制侧面的边
            int index = vertices.size();
            vertices.push_back(v);
            vertices.push_back(nextV);
            
            v.position.y = m_y;
            nextV.position.y = m_y;
            vertices.push_back(v);
            vertices.push_back(nextV);
            
            IndexPool & pool = faces[m_defaultMaterial];
            pool.push_back(index);
            pool.push_back(index+2);
            pool.push_back(index+1);
            
            pool.push_back(index+1);
            pool.push_back(index+2);
            pool.push_back(index+3);
        }
        
        iStart = iEnd;
        iEnd = iNextEnd;
        
        if(dir == nextDir)
        {
            ++nRepeat;
        }
        else nRepeat = 0;
    }
    
    //从顶点生成面
    
    IndexPool & pool = faces[m_defaultMaterial];
    
#define SAFE_ADD_VERTEX(INDEX) \
assert(INDEX >= 0); pool.push_back(INDEX)

    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        
        const WallEdge & e1 = edgeVertices[getEdgeMerge(w.start, w.end)];
        const WallEdge & e2 = edgeVertices[getEdgeMerge(w.end, w.start)];
        //std::swap(e2.first, e2.second);
        
        SAFE_ADD_VERTEX(e1.iEnd);
        SAFE_ADD_VERTEX(e1.iStart);
        SAFE_ADD_VERTEX(e2.iEnd);
        
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e1.iEnd, e1.iStart, e2.iEnd);
        
        SAFE_ADD_VERTEX(e2.iEnd);
        SAFE_ADD_VERTEX(e2.iStart);
        SAFE_ADD_VERTEX(e1.iEnd);
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e2.iEnd, e2.iStart, e1.iEnd);
    }
#undef SAFE_ADD_VERTEX

    //在墙相交处需要补上缺口
    for(auto it = crossVertices.begin(); it != crossVertices.end(); ++it)
    {
        std::vector<int> & vertex = it->second;
        if(it->second.size() == 3)
        {
            pool.push_back(vertex[0]);
            pool.push_back(vertex[1]);
            pool.push_back(vertex[2]);
        }
        else if(it->second.size() == 4)
        {
            sortCrossVertex(vertex, vertices);
            
            pool.push_back(vertex[0]);
            pool.push_back(vertex[1]);
            pool.push_back(vertex[2]);
            
            pool.push_back(vertex[2]);
            pool.push_back(vertex[1]);
            pool.push_back(vertex[3]);
        }
    }
    
    //合并顶点缓冲区
    int bias = vertices.size();
    vertices.insert(vertices.end(), tempVertices.begin(), tempVertices.end());
    for(auto it : tempFaces)
    {
        IndexPool & pool = faces[it.first];
        for (auto i : it.second)
        {
            pool.push_back(i + bias);
        }
    }
}



bool Wall::hasWall(int iWall) const
{
    if(iWall < 0 || iWall >= m_wallMarks.size()) return false;
    
    return m_wallMarks[iWall];
}

void Wall::built(int nRows, int nCols)
{
    m_nRows = nRows;
    m_nCols = nCols;
    m_wallMap.reset(nRows, nCols);
    m_wallMarks.resize(2 * nRows * nCols + nRows + nCols);
    for (auto it = m_wallMarks.begin(); it != m_wallMarks.end(); ++it)
    {
        (*it) = false;
    }
    
}

void Wall::setData(const int *pWalls, int n)
{
    for (int i = 0; i < n; ++i)
    {
        WallCell cell;
        cell.index = pWalls[i * 3];
        cell.material[0] = pWalls[i * 3 + 1];
        cell.material[1] = pWalls[i * 3 + 2];
        
        m_walls.insert(std::make_pair(cell.index, cell));
        m_wallMarks[cell.index] = true;
    }
}

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
    
    std::vector<int> wall = {
        0, 0, 2,
        5, 0, 2,
        6, 0, 2,
        11, 0, 2,
        3, 2, 0,
        9, 2, 0,
        8, 2, 0,
        14, 2, 0,
        15, 2, 0,
        20, 2, 0,
        19, 2, 0,
        28, 2, 0,
    };
    generateWall(nRows, nCols, &wall[0], wall.size() / 3);
    
    
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

void TestMeshFileNode::generateWall(int nRows, int nCols, const int * pWalls, int n)
{
    m_wallData = new Wall();
    m_wallData->init(nRows, nCols, pWalls, n);
    
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
