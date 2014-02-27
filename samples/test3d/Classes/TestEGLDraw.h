//
//  TestEGL.h
//  test3d
//
//  Created by zhoubao on 14-2-21.
//
//

#ifndef __test3d__TestEGL__
#define __test3d__TestEGL__

#include "TestBase.h"


class TestEGLDrawNode : public TestBaseNode
{
    GLint m_programObject;
    
public:
    TestEGLDrawNode();
    ~TestEGLDrawNode();
    
    CREATE_TEST_3D_NODE(TestEGLDrawNode)
    
    virtual bool initTest3D() override;
    virtual void draw() override;
};


#endif /* defined(__test3d__TestEGL__) */
