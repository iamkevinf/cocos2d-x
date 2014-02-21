//
//  Effect.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "Effect.h"

namespace my3d
{
    
    Effect::Effect()
    : m_program(0)
    {
        
    }
    
    Effect::~Effect()
    {
        
    }
    
    void Effect::bindAttribute(cocos2d::VertexAttribute index, const std::string & name)
    {
        glBindAttribLocation(m_program, index, name.c_str());
    }
    
}//end namespace my3d