//
//  TestMeshDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestMeshDraw__
#define __test3d__TestMeshDraw__

#include "TestBase.h"

class TestMeshDrawNode : public TestBaseNode
{
    my3d::MeshPtr m_mesh;
public:
    TestMeshDrawNode();
    ~TestMeshDrawNode();
    
    CREATE_TEST_3D_NODE(TestMeshDrawNode)
    
    virtual bool initTest3D() override;
    virtual void draw() override;
};

#endif /* defined(__test3d__TestMeshDraw__) */
