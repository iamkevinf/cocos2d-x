//
//  TestMeshFromFile.h
//  test3d
//
//  Created by zhoubao on 14-3-3.
//
//

#ifndef __test3d__TestMeshFromFile__
#define __test3d__TestMeshFromFile__

#include "TestBase.h"

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
    
    enum GridSize
    {
        GridUp = 0,
        GridLeft = 1,
        GridRight = 2,
        GridDown = 3,
    };

    struct Wall
    {
        int index;
        int grid[2];
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
};

class Wall : public cocos2d::ISmartObject
{
public:
    typedef std::vector<my3d::VertexXYZNUV> VertexPool;
    typedef std::vector<my3d::uint16> IndexPool;
    typedef std::map<int, IndexPool> FaceMap;

    Wall();
    ~Wall();
    
    //pWalls：3个整数为一组（墙索引，材质1，材质2）
    void init(int nRows, int nCols, const int * pWalls, int n);
    
    void generateVertex(VertexPool & vertices, FaceMap & faces);
    
    //水平的墙
    void generateVertexA(VertexPool & vertices, FaceMap & faces, WallMap::WallSide side);
    //竖直的墙
    void generateVertexB(VertexPool & vertices, FaceMap & faces, WallMap::WallSide side);
    //墙顶部
    void generateVertexC(VertexPool & vertices, FaceMap & faces);
    void generateVertexD(VertexPool & vertices, FaceMap & faces);
    
    bool hasWall(int iWall) const;
    
private:
    void built(int nRows, int nCols);
    void setData(const int *pWalls, int n);
    
    int m_nRows;
    int m_nCols;
    WallMap m_wallMap;
    std::map<int, WallCell> m_walls;//墙索引->墙
    std::vector<bool> m_wallMarks;//标记此位置是否有墙
    float m_gridSize;
    float m_y;
    float m_wallHeight;
    float m_wallThick;
    float m_wallThickHalf;
    int m_defaultMaterial;
};
typedef SmartPtr<Wall> WallPtr;

class TestMeshFileNode : public TestBaseNode
{
    int m_nRows;
    int m_nCols;
    std::vector<my3d::MaterialPtr> m_materials;
    my3d::MeshPtr m_ground;
    WallPtr m_wallData;
    my3d::MeshPtr m_wallMesh;
public:

    TestMeshFileNode();
    ~TestMeshFileNode();
    
    virtual bool initTest3D() override;
    virtual void draw() override;
    
    bool loadMaterial();
    
    void generateGround(int nRows, int nCols, const int *rawData);
    
    void generatePrimitive(int nRows, int nCols, const int *rawData, std::map<int, std::vector<int>> & group);
    void generateVertices(int nRows, int nCols, my3d::VertexXYZNUV * pVertices);
    void generateIndices(int nRows, int nCols, my3d::uint16 * pIndices);
    
    void generateWall(int nRows, int nCols, const int * pWalls, int n);
    
    CREATE_TEST_3D_NODE(TestMeshFileNode)
    
    
};

#endif /* defined(__test3d__TestMeshFromFile__) */
