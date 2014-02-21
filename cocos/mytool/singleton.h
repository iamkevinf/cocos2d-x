//
//  singleton.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef dydg_b_singleton_h
#define dydg_b_singleton_h


template<typename T>
class Singleton
{
public:
    Singleton()
    {}
    
    ~Singleton()
    {}
    
    static T * instance()
    {
        static T s_t;
        return &s_t;
    }
};



#endif
