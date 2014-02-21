//
//  EffectManager.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "EffectMgr.h"

namespace my3d
{
    
    
    EffectMgr::EffectMgr()
    {
        
    }
    EffectMgr::~EffectMgr()
    {
        
    }
    
    EffectPtr EffectMgr::get(const std::string & resouce, bool createIfMiss)
    {
        EffectPtr effect;
        
        auto it = m_effects.find(resouce);
        if(it != m_effects.end())
        {
            effect = it->second;
        }
        else if(createIfMiss)
        {
            effect = new Effect(resouce);
            if(!effect->isValid())
            {
                effect = nullptr;
            }
            
            m_effects[resouce] = effect.get();
        }
        
        return effect;
    }
    
    void EffectMgr::del(const std::string & resouce)
    {
        auto it = m_effects.find(resouce);
        if(it != m_effects.end())
        {
            m_effects.erase(it);
        }
    }
    
    void EffectMgr::del(Effect *pEffect)
    {
        assert(pEffect);
        
        for(auto it = m_effects.begin(); it != m_effects.end(); ++it)
        {
            if(pEffect == it->second)
            {
                m_effects.erase(it);
                return;
            }
        }
    }
    
}