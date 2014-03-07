//
//  Homeland.cpp
//  test3d
//
//  Created by zhoubao on 14-3-7.
//
//

#include "Homeland.h"


/*正方形顶点排列
 0 2
 1 3
 */
const int SqureIndices[] = {
    0, 1, 2,
    2, 1, 3
};
const int NSqureIndices = 2 * 3;

/*窗户顶点排列
 0     6
 2 4
 3 5
 1     7
 */
const int WindowIndices[] = {
    0, 1, 3,
    0, 3, 2,
    0, 2, 4,
    0, 4, 6,
    1, 5, 3,
    1, 7, 5,
    6, 4, 5,
    6, 5, 7,
};
const int NWindowIndices = 8 * 3;

/*门的顶点排列
 0     6
 2 4
 1 3 5 7
 */
const int DoorIndices[] = {
    0, 1, 2,
    0, 2, 6,
    1, 3, 2,
    6, 4, 7,
    6, 2, 4,
    7, 4, 5,
};
const int NDoorIndices = 6 * 3;

//窗口裁剪区域
const float WindowRect[] = {
    0.2, 0.2,
    0.2, 1.2,
    0.6, 0.2,
    0.6, 1.2,
};

//门裁剪区域
const float DoorRect[] = {
    0.2, 0.2,
    0.2, 2.0,
    0.6, 0.2,
    0.6, 2.0,
};

//边的方向
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
    if(delta > 1) return EdgeDir::Down;
    if(delta == 1) return EdgeDir::Right;
    if(delta == -1) return EdgeDir::Left;
    return EdgeDir::Up;
}

inline int getEdgeMerge(int start, int end)
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

void dir2normalV(my3d::Vector3 & normal, int dir)
{
    switch(dir)
    {
        case EdgeDir::Up:
            normal.set(1, 0, 0);
            break;
            
        case EdgeDir::Down:
            normal.set(-1, 0, 0);
            break;
            
        case EdgeDir::Left:
            normal.set(0, 0, -1);
            break;
            
        case EdgeDir::Right:
            normal.set(0, 0, 1);
            break;
    }
}

void dir2normalH(my3d::Vector3 & normal, int dir)
{
    switch(dir)
    {
        case EdgeDir::Up:
            normal.set(0, 0, -1);
            break;
            
        case EdgeDir::Down:
            normal.set(0, 0, 1);
            break;
            
        case EdgeDir::Left:
            normal.set(-1, 0, 0);
            break;
            
        case EdgeDir::Right:
            normal.set(1, 0, 0);
            break;
    }
}


//临时方法，判断一个id是否是门
bool isDoor(int iWindow)
{
    return iWindow >= 1000;
}

//////////////////////////////////////////////////////////////////////////////////

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
, m_wallThick(0.2f)
, m_defaultMaterial(0)
{
    m_wallThickHalf = m_wallThick * 0.5f;
}

Wall::~Wall()
{
    
}

//pWalls：3个整数为一组（墙索引，材质1，材质2）
void Wall::init(int nRows, int nCols, const int * pWalls, int n, int step)
{
    built(nRows, nCols);
    setData(pWalls, n, step);
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

void Wall::setData(const int *pWalls, int n, int step)
{
    for (int i = 0; i < n; ++i)
    {
        int idx = i * step;
        WallCell cell;
        cell.index = pWalls[idx];
        cell.material[0] = pWalls[idx + 1];
        cell.material[1] = pWalls[idx + 2];
        cell.window = pWalls[idx + 3];
        
        m_walls.insert(std::make_pair(cell.index, cell));
        m_wallMarks[cell.index] = true;
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


void Wall::generateVertex(VertexPool & vertices, FaceMap & faces)
{
    VertexPool tempVertices;
    FaceMap tempFaces;
    
    const int nColVertices = m_nCols + 1;
    
    m_edges.clear();
    m_edgeInfo.clear();
    
    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        m_edges.insert(std::make_pair(w.start, w.end));
        m_edges.insert(std::make_pair(w.end, w.start));
        
        int dir = getEdgeDir(w.start, w.end);
        int iMtl = dir & (EdgeDir::Right | EdgeDir::Down) ? 0 : 1;
        m_edgeInfo[getEdgeMerge(w.start, w.end)] = WallEdgeInfo(it.first, it.second.material[iMtl], it.second.window, -1, -1);
        m_edgeInfo[getEdgeMerge(w.end, w.start)] = WallEdgeInfo(it.first, it.second.material[(iMtl+1)%2], it.second.window, -1, -1);
    }
    
    //每个交叉点生成的顶点
    std::map<int, std::vector<int>> crossVertices;
    
    int iStart = -1;
    int iEnd = -1;
    int iLastVertex = -1;
    int nRepeat = 0;
    while(!m_edges.empty())
    {
        WallEdgeMap::iterator itEdge;
        if(iEnd < 0)//随机选择一个边
        {
            itEdge = m_edges.begin();
            
            iStart = itEdge->first;
            iEnd = itEdge->second;
            iLastVertex = -1;
            nRepeat = 0;
        }
        else
        {
            auto equal = m_edges.equal_range(iStart);
            itEdge = equal.first;
            for(; itEdge != equal.second; ++itEdge)
            {
                if(itEdge->first == iStart && itEdge->second == iEnd)
                    break;
            }
            
            CCAssert(itEdge != equal.second, "Invalid edge!");
        }
        
        int dir = getEdgeDir(iStart, iEnd);
        
        auto equal = m_edges.equal_range(iEnd);
        int existMask = 0;
        for(auto it = equal.first; it != equal.second; ++it)
        {
            existMask |= getEdgeDir(it->first, it->second);
        }
        
        WallEdgeInfo & edgeInfo = m_edgeInfo[getEdgeMerge(iStart, iEnd)];
        if(iLastVertex >= 0)
        {
            edgeInfo.vStart = iLastVertex;
            
            //围墙的面
            if(nRepeat == 0)
            {
                my3d::VertexXYZNUV vertex = vertices[edgeInfo.vStart];
                dir2normalV(vertex.normal, dir);
                vertex.uv.set(0, 0);
                tempVertices.push_back(vertex);
                
                vertex.position.y = m_y;
                vertex.uv.set(0, 1);
                tempVertices.push_back(vertex);
                
                CCLOG("wall vertex %d->%d start: copy(%d) index(%d, %d)",
                      iStart, iEnd, iLastVertex, tempVertices.size() - 2, tempVertices.size() - 1);
            }
        }
        
        if(edgeInfo.vEnd < 0)
        {
            my3d::VertexXYZNUV vEnd;
            vEnd.position.set((iEnd % nColVertices) * m_gridSize, m_y + m_wallHeight, (iEnd / nColVertices) * m_gridSize);
            vEnd.normal.set(0, 1, 0);
            vEnd.uv.set(0, 0);
            
            
            //逆时针判断下一步要走的方向
            int nextDir = dir == EdgeDir::Max ? EdgeDir::Min : dir << 1;
            int i = 0;
            for(; i<4; ++i)
            {
                if(nextDir & existMask) break;
                
                nextDir >>= 1;
                if(nextDir < EdgeDir::Min) nextDir = EdgeDir::Max;
            }
            
            const float dirs[] = {1, 0, -1, 0};
            float sign = dir & (EdgeDir::Up | EdgeDir::Right) ? 1 : -1;
            if(dir & (EdgeDir::Up | EdgeDir::Down))
            {
                vEnd.position.x += sign * m_wallThickHalf;
                vEnd.position.z += sign * dirs[i] * m_wallThickHalf;
            }
            else
            {
                vEnd.position.z += sign * m_wallThickHalf;
                vEnd.position.x -= sign * dirs[i] * m_wallThickHalf;
            }
            
            edgeInfo.vEnd = vertices.size();
            vertices.push_back(vEnd);
            iLastVertex = edgeInfo.vEnd;
            crossVertices[iEnd].push_back(iLastVertex);
            
            CCLOG("%d->%d pos(%f, %f, %f) index(%d) edge(%d, %d)",
                  iStart, iEnd, vEnd.position.x, vEnd.position.y, vEnd.position.z, iLastVertex, edgeInfo.vStart, edgeInfo.vEnd);
            
            
            //决定下一次需要选取的边
            int iNextEnd = getNextVertex(iEnd, nextDir);
            
            //反向的边
            if(iNextEnd == iStart)
            {
                //镜像出对边的起点
                my3d::VertexXYZNUV nextV = vEnd;
                nextV.normal.set(0, 1, 0);
                
                int axis = dir & (EdgeDir::Up | EdgeDir::Down) ? 0 : 2;
                float sign = dir & (EdgeDir::Up | EdgeDir::Right) ? -1 : 1;
                nextV.position.m[axis] += sign * m_wallThick;
                
                iLastVertex = vertices.size();
                vertices.push_back(nextV);
                CCLOG("%d->%d pos(%f, %f, %f) index(%d)",
                      iEnd, iNextEnd, nextV.position.x, nextV.position.y, nextV.position.z, iLastVertex);
                
#if 1
                //意味着这是一面孤立的墙，需要绘制侧面的边
                int index = vertices.size();
                
                my3d::VertexXYZNUV prevV = vEnd;
                dir2normalH(prevV.normal, dir);
                vertices.push_back(prevV);
                
                prevV.position.y = m_y;
                vertices.push_back(prevV);
                
                nextV.normal = prevV.normal;
                vertices.push_back(nextV);
                
                nextV.position.y = m_y;
                vertices.push_back(nextV);
                
                IndexPool & pool = faces[m_defaultMaterial];
                for(int k = 0; k<NSqureIndices; ++k)
                    pool.push_back(index + SqureIndices[k]);
#endif
            }
            
            iStart = iEnd;
            iEnd = iNextEnd;
            
            //顶点是否公用
            nRepeat = (dir == nextDir && edgeInfo.vStart >=0 ? nRepeat + 1 : 0);
        }
        else
        {
            CCLOG("%d->%d edge(%d, %d)", iStart, iEnd, edgeInfo.vStart, edgeInfo.vEnd);
            
            iEnd = -1;
        }
        
        if(edgeInfo.vStart >= 0)
        {
            m_edges.erase(itEdge);
            
            //生成围墙的面
            
            int idx = tempVertices.size() - 2; //公用两个顶点
            
            const my3d::VertexXYZNUV prevV = tempVertices[idx];
            my3d::VertexXYZNUV nextV = vertices[edgeInfo.vEnd];
            dir2normalV(nextV.normal, dir);
            nextV.uv.set(nRepeat + 1, 0);
            
            const int *pIndices;
            int nIndices;
            
            if(edgeInfo.iWindow < 0)
            {
                pIndices = SqureIndices;
                nIndices = NSqureIndices;
            }
            else
            {
                float *pClipRect;
                if(!isDoor(edgeInfo.iWindow))
                {
                    pIndices = WindowIndices;
                    nIndices = NWindowIndices;
                    pClipRect = (float*)WindowRect;
                }
                else
                {
                    pIndices = DoorIndices;
                    nIndices = NDoorIndices;
                    pClipRect = (float*)DoorRect;
                }
                
                //加入窗口的4个顶点
                int axis = dir & (EdgeDir::Left | EdgeDir::Right) ? 0 : 2;
                float realWidth = fabs(nextV.position.m[axis] - prevV.position.m[axis]);
                float offset = (realWidth - m_gridSize) * 0.5f;
                float sign = dir & (EdgeDir::Right | EdgeDir::Down) ? 1 : -1;
                int bias = sign > 0 ? 0 : 2;
                for(int k = 0; k < 4; ++k)
                {
                    int x = (k + bias) % 4 * 2;
                    my3d::VertexXYZNUV tempV = sign > 0 ? prevV : nextV;
                    
                    tempV.position.m[axis] += (offset + pClipRect[x]);
                    tempV.position.y = m_y + m_wallHeight - pClipRect[x + 1];
                    
                    tempV.uv.x += sign * pClipRect[x] / m_gridSize;
                    tempV.uv.y += pClipRect[x + 1] / m_wallHeight;
                    
                    tempVertices.push_back(tempV);
                }
            }
            
            //后两个顶点
            tempVertices.push_back(nextV);
            
            nextV.position.y = m_y;
            nextV.uv.set(nRepeat + 1, 1);
            tempVertices.push_back(nextV);
            
            CCLOG("wall vertex %d->%d end: copy(%d) index(%d, %d)",
                  iStart, iEnd, edgeInfo.vEnd, tempVertices.size() - 2, tempVertices.size() - 1);
            
            IndexPool & pool = tempFaces[edgeInfo.iMaterial];
            for(int k = 0; k<nIndices; ++k)
            {
                pool.push_back(idx + pIndices[k]);
            }
        }
    }
    
    //从顶点生成面
    
    IndexPool & pool = faces[m_defaultMaterial];
    
#define SAFE_ADD_VERTEX(INDEX) \
assert(INDEX >= 0); pool.push_back(INDEX)
    
    for (auto it: m_walls)
    {
        const WallMap::Wall & w = m_wallMap.getWall(it.first);
        
        const WallEdgeInfo & e1 = m_edgeInfo[getEdgeMerge(w.start, w.end)];
        const WallEdgeInfo & e2 = m_edgeInfo[getEdgeMerge(w.end, w.start)];
        //std::swap(e2.first, e2.second);
        
        SAFE_ADD_VERTEX(e1.vEnd);
        SAFE_ADD_VERTEX(e1.vStart);
        SAFE_ADD_VERTEX(e2.vEnd);
        
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e1.vEnd, e1.vStart, e2.vEnd);
        
        SAFE_ADD_VERTEX(e2.vEnd);
        SAFE_ADD_VERTEX(e2.vStart);
        SAFE_ADD_VERTEX(e1.vEnd);
        CCLOG("wall(%d) vert(%d, %d) face(%d %d %d)", it.first, w.start, w.end,  e2.vEnd, e2.vStart, e1.vEnd);
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


