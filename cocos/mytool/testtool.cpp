//
//  testtool.cpp
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#include "testtool.h"
#include "cocos2d.h"
#include <exception>

void doTestCase(bool exp, const char * desc, const char * file, int line)
{
    if(exp) CCLOG("test succed: %s", desc);
    
    else CCLOG("test failed: %s. file:%s, line:%d", desc, file, line);
}

void doAssert(bool exp, const char * desc, const char *file, int line)
{
    if(!exp) CCLOG("Assert failed: %s. file:%s, line:%d", desc, file, line);
    
    throw(std::runtime_error());
}