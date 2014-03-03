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

class TestMeshFileNode : public TestBaseNode
{
    int m_nRows;
    int m_nCols;
    std::vector<my3d::MaterialPtr> m_materials;
    my3d::MeshPtr m_ground;
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
    
    CREATE_TEST_3D_NODE(TestMeshFileNode)
    
    
};

#endif /* defined(__test3d__TestMeshFromFile__) */
