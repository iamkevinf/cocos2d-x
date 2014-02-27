//
//  M3DVertex.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__M3DVertex__
#define __cocos3d_libs__M3DVertex__

#include "M3DBase.h"
#include "M3DColor.h"

namespace my3d
{
#define DEF_VERTEX_TYPE(name) static const char * getType(){ return #name; }
    
    struct VertexXYZ
    {
        cocos2d::Vector3 position;
        
        DEF_VERTEX_TYPE(xyz)
    };
    
    struct VertexXYZColor
    {
        cocos2d::Vector3 position;
        Color color;
        
        DEF_VERTEX_TYPE(xyzc)
    };
    
    struct VertexXYZN
    {
        cocos2d::Vector3 position;
        cocos2d::Vector3 normal;
        
        DEF_VERTEX_TYPE(xyzn)
    };
    
    struct VertexXYZUV
    {
        cocos2d::Vector3 position;
        cocos2d::Vector2 uv;
        
        DEF_VERTEX_TYPE(xyzuv)
    };
    
    struct VertexXYZNUV
    {
        cocos2d::Vector3 position;
        cocos2d::Vector3 normal;
        cocos2d::Vector2 uv;
        
        DEF_VERTEX_TYPE(xyznuv)
    };
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__M3DVertex__) */
