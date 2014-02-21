//
//  smartptr.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef dydg_b_smartptr_h
#define dydg_b_smartptr_h


template<typename T>
class SmartPtr
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T * const_pointer;
    typedef const T & const_reference;
    typedef SmartPtr<T> this_type;
    
    SmartPtr()
    : m_ptr(NULL)
    {}
    
    SmartPtr(pointer p)
    : m_ptr(p)
    {
        if(m_ptr != NULL) m_ptr->retain();
    }
    
    SmartPtr(const this_type & o)
    : m_ptr(o.m_ptr)
    {
        if(m_ptr != NULL) m_ptr->retain();
    }
    
    template<typename U>
    SmartPtr(const SmartPtr<U> & o)
    {
        m_ptr = dynamic_cast<pointer>(o.get());
        if(m_ptr != NULL) m_ptr->retain();
    }
    
    ~SmartPtr()
    {
        if(m_ptr != NULL) m_ptr->release();
    }
    
    pointer get() const
    {
        return m_ptr;
    }
    
    template<typename U>
    U* cast() const
    {
        return (U*) m_ptr;
    }
    
public:
    
    operator bool () const
    {
        return m_ptr != NULL;
    }
    
    pointer operator -> () const
    {
        CCAssert(m_ptr != NULL, "SmartPtr::operator ->");
        return m_ptr;
    }
    
    reference operator * () const
    {
        CCAssert(m_ptr != NULL, "SmartPtr::operator *");
        return *m_ptr;
    }
    
    const this_type & operator = (pointer p)
    {
        if(m_ptr != p)
        {
            if(m_ptr != NULL) m_ptr->release();
            m_ptr = p;
            if(m_ptr != NULL) m_ptr->retain();
        }
        return *this;
    }
    
    const this_type & operator = (const this_type & o)
    {
        return *this = o.m_ptr;
    }
    
    template<typename U>
    const this_type & operator = (const SmartPtr<U> & o)
    {
        return *this = dynamic_cast<pointer>(o.get());
    }
    
private:
    pointer m_ptr;
};


template<typename T, typename U>
bool operator == (const SmartPtr<T> & l, const U * r)
{
    return l.get() == r;
}

template<typename T, typename U>
bool operator == (const T *l, const SmartPtr<U> & r)
{
    return l == r.get();
}

template<typename T, typename U>
bool operator == (const SmartPtr<T> & l, const SmartPtr<U> & r)
{
    return l.get() == r.get();
}

template<typename T, typename U>
bool operator != (const SmartPtr<T> & l, const U * r)
{
    return l.get() != r;
}

template<typename T, typename U>
bool operator != (const T *l, const SmartPtr<U> & r)
{
    return l != r.get();
}

template<typename T, typename U>
bool operator != (const SmartPtr<T> & l, const SmartPtr<U> & r)
{
    return l.get() != r.get();
}


void testSmartPtr();


#endif
