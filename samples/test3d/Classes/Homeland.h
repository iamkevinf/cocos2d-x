//
//  Homeland.h
//  test3d
//
//  Created by zhoubao on 14-3-7.
//
//

#ifndef __test3d__Homeland__
#define __test3d__Homeland__

#include "my3d/my3d.h"


class WallMap
{
public:
    WallMap();
    ~WallMap();
    
    //墙的面。左侧和上侧视为外部，右侧和下侧视为内部
    enum WallSide
    {
        WallOutside = 0,
        WallInside = 1,
    };
    
    enum GridSide
    {
        GridUp = 0,
        GridLeft = 1,
        GridRight = 2,
        GridDown = 3,
    };
    
    static int getInvGridSide(int side);
    
    struct Wall
    {
        int index;
        int grid[2];//占据两个格子
        int dir;//水平/竖直
        int start;//起点
        int end;//终点
    };
    
    struct Grid
    {
        int index;
        int wall[4];
    };
    
    void reset(int nRows, int nCols);
    
    int wall2grid(int iWall, int i) const;
    int grid2wall(int iGrid, int i) const;
    
    const Grid & getGrid(int iGrid) const;
    const Wall & getWall(int iWall) const;
    
private:
    int m_nRows;
    int m_nCols;
    std::vector<Grid> m_grids;
    std::vector<Wall> m_walls;
};

struct WallCell
{
    int index;//墙的索引
    int material[2];
    int window;
};

struct WallEdgeInfo
{
    int iWall;
    int iMaterial;
    int iWindow;
    int vStart;
    int vEnd;
    
    WallEdgeInfo()
    {}
    
    WallEdgeInfo(int wall, int mtl, int window, int start, int end)
    : iWall(wall), iMaterial(mtl), iWindow(window),  vStart(start), vEnd(end)
    {}
};

class Wall : public cocos2d::ISmartObject
{
public:
    typedef std::vector<my3d::VertexXYZNUV> VertexPool;
    typedef std::vector<my3d::uint16> IndexPool;
    typedef std::map<int, IndexPool> FaceMap;
    
    Wall();
    ~Wall();
    
    //pWalls：4个整数为一组（墙索引，材质1，材质2，窗口id）
    void init(int nRows, int nCols, const int * pWalls, int n, int step);
    
    void generateVertex(VertexPool & vertices, FaceMap & faces);
    
    bool hasWall(int iWall) const;
    int getNextVertex(int iVertex, int dir) const;
    
private:
    void built(int nRows, int nCols);
    void setData(const int *pWalls, int n, int step);
    
    int m_nRows;
    int m_nCols;
    
    float m_gridSize;
    float m_y;
    float m_wallHeight;
    float m_wallThick;
    float m_wallThickHalf;
    int m_defaultMaterial;
    
    WallMap m_wallMap;
    std::map<int, WallCell> m_walls;//墙索引->墙
    std::vector<bool> m_wallMarks;//标记此位置是否有墙
    
    //一面墙的两个边
    typedef std::multimap<int, int> WallEdgeMap;
    WallEdgeMap m_edges;
    
    //记录每个边的两个顶点。key是一个合成的数值：start << 16 + end
    std::map<my3d::uint32, WallEdgeInfo> m_edgeInfo;
};
typedef SmartPtr<Wall> WallPtr;


#endif /* defined(__test3d__Homeland__) */
