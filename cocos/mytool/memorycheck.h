//
//  memorycheck.h
//  cocos2d_libs
//
//  Created by zhoubao on 14-2-24.
//
//

#ifndef __cocos2d_libs__memorycheck__
#define __cocos2d_libs__memorycheck__

#include "cocos2d.h"

NS_CC_BEGIN

//构造函数中无法得知对象的真实类型，所以需要延迟获取类型
class ITypeProxy
{
public:
    ITypeProxy()
    {}
    
    virtual ~ITypeProxy()
    {}
    
    virtual std::string getTypeName() = 0;
    virtual bool equal(ITypeProxy *p) const = 0;
};

template<typename T>
class ImpTypeProxy : public ITypeProxy
{
    T * m_ptr;
public:
    ImpTypeProxy(T * p)
        : m_ptr(p)
    {}
    
    virtual ~ImpTypeProxy()
    {}
    
    virtual std::string getTypeName() override
    {
        return typeid(*m_ptr).name();
    }
    
    virtual bool equal(ITypeProxy *p) const override
    {
        return m_ptr == ((ImpTypeProxy*)p)->m_ptr;
    }
};

template<typename T>
ITypeProxy * makeTypeProxy(T *p)
{
    return new ImpTypeProxy<T>(p);
}

//统计实例个数，以校验那些类型的对象出现内存泄漏
class InstanceCounter
{
    
public:
    
    static InstanceCounter * getInstance();
    static void deleteInstance();
    
    template<typename T>
    void addObj(T *p)
    {
        m_objAddCache.push_back(new ImpTypeProxy<T>(p));
        
        if(m_bUseCollector) m_objCollector.insert((cocos2d::Object*)p);
    }
    
    template<typename T>
    void delObj(T *p)
    {
        ITypeProxy * proxy = new ImpTypeProxy<T>(p);
        auto it = std::find_if(m_objAddCache.begin(), m_objAddCache.end(),
                               [proxy](ITypeProxy * a){ return a->equal(proxy);} );
        if(it != m_objAddCache.end())
        {
            m_objAddCache.erase(it);
            delete proxy;
        }
        else
        {
            m_objDelCache.push_back(proxy);
        }
        
        if(m_bUseCollector) m_objCollector.erase((cocos2d::Object*)p);
    }
    
    void beginSchedule();
    
    void dumpAll();
    
    void setCollectorEnable(bool e);
    
    void addCounter(const std::string & name);
    void delCounter(const std::string & name);
    
private:
    
    InstanceCounter();
    ~InstanceCounter();
    
    void update();
    
    
    std::vector<ITypeProxy*> m_objAddCache;
    std::vector<ITypeProxy*> m_objDelCache;
    std::map<std::string, int> m_objCatalog;
    
    bool m_bUseCollector;
    std::set<cocos2d::Object*> m_objCollector;
    
    friend class ScheduleProxy;
    class ScheduleProxy * m_pScheduleProxy;
    
    static InstanceCounter * s_instance;
};


#if COCOS2D_DEBUG > 0
#define CHECK_OBJECT_CONS(p) cocos2d::InstanceCounter::getInstance()->addObj(p)
#define CHECK_OBJECT_DEST(p) cocos2d::InstanceCounter::getInstance()->delObj(p)
#else
#define CHECK_OBJECT_CONS(p)
#define CHECK_OBJECT_DEST(p)
#endif

NS_CC_END

#endif /* defined(__cocos2d_libs__memorycheck__) */
