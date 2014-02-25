//
//  EffectManager.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__EffectManager__
#define __cocos3d_libs__EffectManager__

#include "Effect.h"
#include "mytool/singleton.h"
#include <map>

namespace my3d
{

    class EffectMgr : public Singleton<EffectMgr>
    {
    public:
        
        EffectMgr();
        ~EffectMgr();
        
        EffectPtr get(const std::string & resouce, bool createIfMiss=true);
        void del(const std::string & resouce);
        void del(Effect *pEffect);
        
    private:
        
        std::map<std::string, Effect*> m_effects;
    };

}

#endif /* defined(__cocos3d_libs__EffectManager__) */
