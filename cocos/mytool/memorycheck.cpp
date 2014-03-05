//
//  memorycheck.cpp
//  cocos2d_libs
//
//  Created by zhoubao on 14-2-24.
//
//

#include "memorycheck.h"
#include "cocos2d.h"

NS_CC_BEGIN



/////////////////////////////////////////////////////////////////////////////


InstanceCounter * InstanceCounter::s_instance = nullptr;


/*static*/ InstanceCounter * InstanceCounter::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new InstanceCounter();
        CCLOG("InstanceCounter created.");
    }
    return s_instance;
}

/*static*/ void InstanceCounter::deleteInstance()
{
    if(s_instance != nullptr)
    {
        delete s_instance;
        s_instance = nullptr;
        CCLOG("InstanceCounter deleted.");
    }
}

InstanceCounter::InstanceCounter()
{
    //不要调用任何东西，容易出现递归调用
}

InstanceCounter::~InstanceCounter()
{
    CCLOG("InstanceCounter will exit soon.");
 
    dumpAll();
    
    for(ITypeProxy *p : m_objects)
    {
        delete p;
    }
    m_objects.clear();
}

void InstanceCounter::dumpAll()
{
    std::map<std::string, int> counter;
    
    for(auto it : m_objects)
    {
        ++counter[it->getTypeName()];
    }
    
    for(auto it : counter)
    {
        CCLOG("object: %-4d %s", it.second, it.first.c_str());
    }
}


NS_CC_END