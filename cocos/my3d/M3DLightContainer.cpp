//
//  M3DLightContainer.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-27.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DLightContainer.h"

namespace my3d
{
    LightContainer::LightContainer()
    {
        
    }
    LightContainer::~LightContainer()
    {
        
    }

    void LightContainer::setAmbientColor(const Color & color)
    {
        m_ambient = color;
    }

    void LightContainer::addDirLight(DirLightPtr light)
    {
        m_dirLights.push_back(light);
    }
    
    void LightContainer::clearDirLight()
    {
        m_dirLights.clear();
    }
    
    
}//end namespace my3d