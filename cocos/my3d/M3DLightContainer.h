//
//  M3DLightContainer.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-27.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__M3DLightContainer__
#define __cocos3d_libs__M3DLightContainer__

#include "M3DLightDir.h"

namespace my3d
{
    typedef std::vector<DirLightPtr> DirLightVector;
    
    class LightContainer : public cocos2d::ISmartObject
    {
    public:
        LightContainer();
        ~LightContainer();
        
        void setAmbientColor(const Color & color);
        const Color & getAmbientColor() const { return m_ambient;}
        
        void addDirLight(DirLightPtr light);
        void clearDirLight();
        DirLightVector & getDirLights(){ return m_dirLights; }
        
    private:
        Color m_ambient;
        DirLightVector m_dirLights;
    };
    
    typedef SmartPtr<LightContainer> LightContainerPtr;
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__M3DLightContainer__) */
