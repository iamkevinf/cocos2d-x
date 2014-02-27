//
//  TestBase.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestBase.h"

TestBaseNode::TestBaseNode()
{
    
}

TestBaseNode::~TestBaseNode()
{
    
}

bool TestBaseNode::initTest3D()
{
    return true;
}

void TestBaseNode::draw()
{
    cocos2d::C3DNode::draw();
}