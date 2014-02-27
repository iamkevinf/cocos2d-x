//
//  TestLightDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestLightDraw__
#define __test3d__TestLightDraw__

#include "TestBase.h"

class TestLightDrawNode : public TestBaseNode
{
    my3d::MeshPtr m_mesh;
public:
    CREATE_TEST_3D_NODE(TestLightDrawNode)
    
    TestLightDrawNode();
    ~TestLightDrawNode();
    
    virtual bool initTest3D() override;
    virtual void draw() override;
};

#endif /* defined(__test3d__TestLightDraw__) */
