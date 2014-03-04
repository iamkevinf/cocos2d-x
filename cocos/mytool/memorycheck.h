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
    ITypeProxy(void *p)
    : m_ptr(p)
    {}
    
    virtual ~ITypeProxy()
    {}
    
    virtual std::string getTypeName() = 0;
    
    void * get() const { return m_ptr; }
    
protected:
    void *m_ptr;
    
    friend struct ProxyLessCompare;
};

struct ProxyLessCompare
{
    bool operator () (const ITypeProxy * a, const ITypeProxy * b) const
    {
        return a->m_ptr < b->m_ptr;
    }
};

template<typename T>
class ImpTypeProxy : public ITypeProxy
{
public:
    ImpTypeProxy(T * p)
        : ITypeProxy(p)
    {}
    
    virtual ~ImpTypeProxy()
    {}
    
    virtual std::string getTypeName() override
    {
        return typeid(*((T*)m_ptr)).name();
    }
};


//统计实例个数，以校验那些类型的对象出现内存泄漏
class InstanceCounter
{
    
public:
    static InstanceCounter * getInstance();
    static void deleteInstance();
    
    template<typename T>
    void addObj(T *p)
    {
        m_objects.insert(new ImpTypeProxy<T>(p));
    }
    
    template<typename T>
    void delObj(T *p)
    {
        ITypeProxy * proxy = new ImpTypeProxy<T>(p);
        
        auto it = m_objects.find(proxy);
        if(it != m_objects.end())
        {
            m_objects.erase(it);
        }
    }
    
    void dumpAll();
    
private:
    
    InstanceCounter();
    ~InstanceCounter();
    
    std::set<ITypeProxy*, ProxyLessCompare> m_objects;
    
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
