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

void doTestCase(bool cond, const char * desc, const char * file, int line)
{
    if(cond) CCLOG("test succed: %s", desc);
    
    else CCLOG("test failed: %s. file:%s, line:%d", desc, file, line);
}

void doAssert(bool cond, const char * desc, const char *file, int line)
{
    if(!cond)
    {
        CCLOGERROR("Assert failed: %s. \nfile:%s, \nline:%d", desc, file, line);
    }
}