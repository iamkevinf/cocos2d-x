//
//  memorycheck.cpp
//  cocos2d_libs
//
//  Created by zhoubao on 14-2-24.
//
//

#include "memorycheck.h"
#include "cocos2d.h"

namespace
{
    std::map<std::string, int> g_memoryCounter;
    
}

void incObjectCounter(const std::string & name)
{
    ++g_memoryCounter[name];
}

void decObjectCounter(const std::string & name)
{
    --g_memoryCounter[name];
}

void dumpAllObjectCounter()
{
    for(auto it : g_memoryCounter)
    {
        if(it.second != 0)
        {
            CCLOG("object count: '%s'- %d", it.first.c_str(), it.second);
        }
    }
}
