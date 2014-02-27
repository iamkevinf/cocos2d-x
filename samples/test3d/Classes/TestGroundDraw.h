//
//  TestGroundDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestGroundDraw__
#define __test3d__TestGroundDraw__

#include "TestBase.h"

class TestGroundDrawNode : public TestBaseNode
{
    my3d::MeshPtr m_ground;
    
public:
    TestGroundDrawNode();
    ~TestGroundDrawNode();
    
    CREATE_TEST_3D_NODE(TestGroundDrawNode);
    
    bool initTest3D();
    
    virtual void draw() override;
};

#endif /* defined(__test3d__TestGroundDraw__) */
