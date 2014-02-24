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


class ScheduleProxy : public Object
{
    DISALLOW_COPY_AND_ASSIGN(ScheduleProxy);
    
public:
    ScheduleProxy()
    {
        Director::getInstance()->getScheduler()->scheduleSelector(
             SEL_SCHEDULE(&ScheduleProxy::update), this, 1.0f, false);
    }
    
    ~ScheduleProxy()
    {
        
    }
    
    void upate(float)
    {
        InstanceCounter::getInstance()->update();
    }
    
};


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
    : m_bUseCollector(false)
    , m_pScheduleProxy(nullptr)
{
    //不要调用任何东西，容易出现递归调用
}

InstanceCounter::~InstanceCounter()
{
    CCLOG("InstanceCounter will exit soon.");
 
    if(m_pScheduleProxy != nullptr)
    {
        delete m_pScheduleProxy;
    }
    
    dumpAll();
}


void InstanceCounter::addCounter(const std::string & name)
{
    ++m_objCatalog[name];
}

void InstanceCounter::delCounter(const std::string & name)
{
    --m_objCatalog[name];
}

void InstanceCounter::beginSchedule()
{
    if(m_pScheduleProxy == nullptr)
    {
        m_pScheduleProxy = new ScheduleProxy();
    }
}

//此方法需要用一个定时器不断调用
void InstanceCounter::update()
{
    for(ITypeProxy * p : m_objAddCache)
    {
        addCounter(p->getTypeName());
        delete p;
    }
    m_objAddCache.clear();
    
    for(ITypeProxy * p : m_objDelCache)
    {
        delCounter(p->getTypeName());
        delete p;
    }
    m_objDelCache.clear();
}

void InstanceCounter::dumpAll()
{
    update();
    
    for(auto it : m_objCatalog)
    {
        if(it.second != 0)
        {
            CCLOG("object count: %-4d '%s'", it.second, it.first.c_str());
        }
    }
    
    if(m_bUseCollector)
    {
        for(auto it : m_objCollector)
        {
            CCLOG("object: 0x%x %s", it, typeid(*it).name());
        }
    }
}

void InstanceCounter::setCollectorEnable(bool e)
{
    m_bUseCollector = e;
}

NS_CC_END