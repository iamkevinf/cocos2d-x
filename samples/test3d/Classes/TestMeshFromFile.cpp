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
    generateVertexA(vertices, faces, WallMap::WallOutside);
    generateVertexA(vertices, faces, WallMap::WallInside);
    
    generateVertexB(vertices, faces, WallMap::WallOutside);
    generateVertexB(vertices, faces, WallMap::WallInside);
    
    generateVertexC(vertices, faces);
    generateVertexD(vertices, faces);
}

void Wall::generateVertexA(VertexPool & vertices, FaceMap & faces, WallMap::WallSide side)
{
    float delta = m_wallThickHalf;
    if (side == WallMap::WallOutside)
    {
        delta = -delta;
    }
    
    for(int r = 0; r < m_nRows; ++r)
    {
        int repeat = 0;
        for(int c = 0; c < m_nCols; ++c)
        {
            int iGrid = r * m_nCols + c;
            int iWall = m_wallMap.grid2wall(iGrid, WallMap::GridUp);
            if(!hasWall(iWall))
            {
                repeat = 0;
                continue;
            }
            
            /* vertex order
            0 2
            1 3
            */
            
            const WallCell & w = m_walls[iWall];
            IndexPool & pool = faces[w.material[side]];
            my3d::uint16 index;
            
            if(repeat == 0)
            {
                index = (my3d::uint16)vertices.size();
                
                my3d::VertexXYZNUV vert;
                vert.normal.set(0, 0, 1);
                
                vert.position.set(c * m_gridSize, m_y + m_wallHeight, r * m_gridSize + delta);
                vert.uv.set(0, 0);
                vertices.push_back(vert);
                
                vert.position.set(c * m_gridSize, m_y, r * m_gridSize + delta);
                vert.uv.set(0, 1);
                vertices.push_back(vert);
            }
            else//可以与前一面墙公用两个顶点
            {
                index = (my3d::uint16)vertices.size() - 2;
            }
            
            ++repeat;
            
            my3d::VertexXYZNUV vert;
            vert.normal.set(0, 0, 1);
            
            vert.position.set((c + 1) * m_gridSize, m_y + m_wallHeight, r * m_gridSize + delta);
            vert.uv.set(repeat, 0);
            vertices.push_back(vert);
            
            vert.position.set((c + 1) * m_gridSize, m_y, r * m_gridSize + delta);
            vert.uv.set(repeat, 1);
            vertices.push_back(vert);
            
            //两个三角形的6个索引
            pool.push_back(index);
            pool.push_back(index+1);
            pool.push_back(index+2);
            pool.push_back(index+2);
            pool.push_back(index+1);
            pool.push_back(index+3);
            
        }
    }
    
}

void Wall::generateVertexB(VertexPool & vertices, FaceMap & faces, WallMap::WallSide side)
{
    float delta = m_wallThickHalf;
    if (side == WallMap::WallOutside)
    {
        delta = -delta;
    }
    
    for(int c = 0; c < m_nCols; ++c)
    {
        int repeat = 0;
        
        for(int r = m_nRows - 1; r >= 0; --r)
        {
            int iGrid = r * m_nCols + c;
            int iWall = m_wallMap.grid2wall(iGrid, WallMap::GridLeft);
            if(!hasWall(iWall))
            {
                repeat = 0;
                continue;
            }
            
            /* vertex order
             0 2
             1 3
             */
            
            const WallCell & w = m_walls[iWall];
            IndexPool & pool = faces[w.material[side]];
            my3d::uint16 index;
            
            if(repeat == 0)
            {
                index = (my3d::uint16)vertices.size();
                
                my3d::VertexXYZNUV vert;
                vert.normal.set(0, 0, 1);
                
                vert.position.set(c * m_gridSize + delta, m_y + m_wallHeight, (r + 1) * m_gridSize);
                vert.uv.set(0, 0);
                vertices.push_back(vert);
                
                vert.position.set(c * m_gridSize + delta, m_y, (r + 1) * m_gridSize);
                vert.uv.set(0, 1);
                vertices.push_back(vert);
            }
            else//可以与前一面墙公用两个顶点
            {
                index = (my3d::uint16)vertices.size() - 2;
            }
            
            ++repeat;
            
            my3d::VertexXYZNUV vert;
            vert.normal.set(0, 0, 1);
            
            vert.position.set(c * m_gridSize + delta, m_y + m_wallHeight, r * m_gridSize);
            vert.uv.set(repeat, 0);
            vertices.push_back(vert);
            
            vert.position.set(c * m_gridSize + delta, m_y, r * m_gridSize);
            vert.uv.set(repeat, 1);
            vertices.push_back(vert);
            
            //两个三角形的6个索引
            pool.push_back(index);
            pool.push_back(index+1);
            pool.push_back(index+2);
            pool.push_back(index+2);
            pool.push_back(index+1);
            pool.push_back(index+3);
            
        }
    }
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
    const int Up    = 1 << 1;
    const int Right = 1 << 2;
    const int Down  = 1 << 3;
    const int Left  = 1 << 4;
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

void Wall::generateVertexC(VertexPool & vertices, FaceMap & faces)
{
    const int nColVertices = m_nCols + 1;
    
    //一面墙的两个边
    typedef std::multimap<int, int> WallEdgeMap;
    WallEdgeMap wallEdge;
    
    //记录每个边的两个顶点。key是一个合成的数值：start << 16 + end
    typedef std::pair<int, int> EdgeVertex;
    std::map<my3d::uint32, EdgeVertex> edgeVertices;
    
    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        wallEdge.insert(std::pair<int, int>(w.start, w.end));
        wallEdge.insert(std::pair<int, int>(w.end, w.start));
        
        edgeVertices[getEdgeMerge(w.start, w.end)] = EdgeVertex(-1, -1);
        edgeVertices[getEdgeMerge(w.end, w.start)] = EdgeVertex(-1, -1);
    }
    
#if 1
    CCLOG("num wall:%lu, num edge: %lu", m_walls.size(), wallEdge.size());
    for(auto it : wallEdge)
    {
        CCLOG("%d -> %d", it.first, it.second);
    }
#endif

    
    //每个交叉点生成的顶点
    std::map<int, std::vector<int>> crossVertices;
    
    int iStart = -1;
    int iEnd = -1;
    int iLastVertex = -1;
    while(!wallEdge.empty())
    {
        WallEdgeMap::iterator itEdge;
        if(iEnd < 0)
        {
            itEdge = wallEdge.begin();
            
            iStart = itEdge->first;
            iEnd = itEdge->second;
            iLastVertex = -1;
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
                iStart = -1;
                iEnd = -1;
                continue;
            }
            
            itEdge = it;
        }
        
        auto equal = wallEdge.equal_range(iEnd);
        int existMask = 0;
        for(auto it = equal.first; it != equal.second; ++it)
        {
            existMask |= getEdgeDir(it->first, it->second);
        }
        
        EdgeVertex & eVertex = edgeVertices[getEdgeMerge(iStart, iEnd)];
        if(iLastVertex >= 0)
        {
            eVertex.first = iLastVertex;
            if(eVertex.second >= 0)//走完一圈了
            {
                wallEdge.erase(itEdge);
                
                CCLOG("%d->%d edge(%d, %d)", iStart, iEnd, eVertex.first, eVertex.second);
                
                iEnd = -1;
                continue;
            }
        }
        
        int dir = getEdgeDir(iStart, iEnd);
        int nextDir = -1;
        
        my3d::VertexXYZNUV v;
        v.position.set((iEnd % nColVertices) * m_gridSize, m_y + m_wallHeight, (iEnd / nColVertices) * m_gridSize);
        v.normal.set(0, 1, 0);
        v.uv.set(0, 0);
        
        if(dir == EdgeDir::Down)//向下
        {
            v.position.x += m_wallThickHalf;
            
            if(existMask & EdgeDir::Right)
            {
                v.position.z -= m_wallThickHalf;
                nextDir = EdgeDir::Right;
            }
            else if(existMask & EdgeDir::Down)
            {
                nextDir = EdgeDir::Down;
            }
            else if(existMask & EdgeDir::Left)
            {
                v.position.z += m_wallThickHalf;
                nextDir = EdgeDir::Left;
            }
            else
            {
                nextDir = EdgeDir::Up;
            }
        }
        else if(dir == EdgeDir::Up)//向上
        {
            v.position.x -= m_wallThickHalf;
            
            if(existMask & EdgeDir::Left)
            {
                v.position.z += m_wallThickHalf;
                nextDir = EdgeDir::Left;
            }
            else if(existMask & EdgeDir::Up)
            {
                nextDir = EdgeDir::Up;
            }
            else if(existMask & EdgeDir::Right)
            {
                v.position.z -= m_wallThickHalf;
                nextDir = EdgeDir::Right;
            }
            else
            {
                nextDir = EdgeDir::Down;
            }
        }
        else if(dir == EdgeDir::Right)//向右
        {
            v.position.z -= m_wallThickHalf;
            
            if(existMask & EdgeDir::Up)
            {
                v.position.x -= m_wallThickHalf;
                nextDir = EdgeDir::Up;
            }
            else if(existMask & EdgeDir::Right)
            {
                nextDir = EdgeDir::Right;
            }
            else if(existMask & EdgeDir::Down)
            {
                v.position.x += m_wallThickHalf;
                nextDir = EdgeDir::Down;
            }
            else
            {
                nextDir = EdgeDir::Left;
            }
        }
        else if(dir == EdgeDir::Left)//向左
        {
            v.position.z += m_wallThickHalf;
            
            if(existMask & EdgeDir::Down)
            {
                v.position.x += m_wallThickHalf;
                nextDir = EdgeDir::Down;
            }
            else if(existMask & EdgeDir::Left)
            {
                nextDir = EdgeDir::Left;
            }
            else if(existMask & EdgeDir::Up)
            {
                v.position.x -= m_wallThickHalf;
                nextDir = EdgeDir::Up;
            }
            else
            {
                nextDir = EdgeDir::Right;
            }
        }
        
        eVertex.second = vertices.size();
        vertices.push_back(v);
        iLastVertex = eVertex.second;
        crossVertices[iEnd].push_back(iLastVertex);
        
        CCLOG("%d->%d pos(%f, %f, %f) index(%d) edge(%d, %d)",
              iStart, iEnd, v.position.x, v.position.y, v.position.z, iLastVertex, eVertex.first, eVertex.second);
        
        if(eVertex.first >= 0 && eVertex.second >= 0)
        {
            wallEdge.erase(itEdge);
        }
        
        
        //决定下一次需要选取的边
        int iNextEnd = iEnd;
        switch (nextDir)
        {
            case EdgeDir::Up:
                iNextEnd -= nColVertices;
                break;
                
            case EdgeDir::Down:
                iNextEnd += nColVertices;
                break;
                
            case EdgeDir::Left:
                iNextEnd -= 1;
                break;
                
            case EdgeDir::Right:
                iNextEnd += 1;
                break;
                
            default:
                iNextEnd = -1;
                break;
        }
        
        //反向的边
        if(iNextEnd == iStart)
        {
            my3d::VertexXYZNUV nextV = v;
            switch (nextDir)
            {
                case EdgeDir::Up:
                    nextV.position.x -= m_wallThick;
                    break;
                    
                case EdgeDir::Down:
                    nextV.position.x += m_wallThick;
                    break;
                    
                case EdgeDir::Left:
                    nextV.position.z += m_wallThick;
                    break;
                    
                case EdgeDir::Right:
                    nextV.position.z -= m_wallThick;
                    break;
                    
                default:
                    CCAssert(0, "invalid dir!");
                    break;
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
    }
    
    IndexPool & pool = faces[m_defaultMaterial];
    
#define SAFE_ADD_VERTEX(INDEX) \
assert(INDEX >= 0); pool.push_back(INDEX)

    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        
        EdgeVertex e1 = edgeVertices[getEdgeMerge(w.start, w.end)];
        EdgeVertex e2 = edgeVertices[getEdgeMerge(w.end, w.start)];
        std::swap(e2.first, e2.second);
        
        SAFE_ADD_VERTEX(e1.second);
        SAFE_ADD_VERTEX(e1.first);
        SAFE_ADD_VERTEX(e2.first);
        
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e1.second, e1.first, e2.first);
        
        SAFE_ADD_VERTEX(e2.first);
        SAFE_ADD_VERTEX(e2.second);
        SAFE_ADD_VERTEX(e1.second);
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e2.first, e2.second, e1.second);
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
}

void Wall::generateVertexD(VertexPool & vertices, FaceMap & faces)
{
    
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
        0, 0, 0,
        5, 0, 0,
        6, 0, 0,
        11, 0, 0,
        3, 2, 2,
        9, 2, 2,
        8, 2, 2,
        14, 2, 2,
        15, 2, 2,
        20, 2, 2,
        19, 2, 2,
    };
    generateWall(nRows, nCols, &wall[0], wall.size() / 3);
    
    
    cocos2d::C3DScene *pScene = cocos2d::C3DLayer::getMainLayer()->get3DScene();
    pScene->getActiveCamera()->setPosition(3, 4.0f, 8.0f);
    my3d::renderDev()->updateCamera();
    
    return true;
}

void TestMeshFileNode::draw()
{
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
