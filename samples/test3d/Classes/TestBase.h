//
//  TestBase.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestBase__
#define __test3d__TestBase__

#include "cocos3d.h"
#include "my3d/my3d.h"

#define CREATE_TEST_3D_NODE(CLASS)  \
static CLASS * create()             \
{                                   \
    CLASS *p = new CLASS();         \
    if(!p->initTest3D())            \
    {                               \
        delete p;                   \
        return nullptr;             \
    }                               \
    p->autorelease();               \
    return p;                       \
}

class TestBaseNode : public cocos2d::C3DNode
{
public:
    TestBaseNode();
    ~TestBaseNode();
    
    CREATE_TEST_3D_NODE(TestBaseNode);
    
    virtual bool initTest3D();
    virtual void draw() override;
};

#endif /* defined(__test3d__TestBase__) */
