//
//  Effect.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "Effect.h"
#include "EffectMgr.h"
#include "Shader.h"

namespace my3d
{
    
    
    //////////////////////////////////////////////////////////
    Effect::Effect(const std::string & resouce)
    : m_program(0)
    , m_resouce(resouce)
    {
        
        m_vertexShader = new Shader(ShaderType::Vertex);
        m_pixelShader = new Shader(ShaderType::Pixel);
    }
    
    Effect::~Effect()
    {
        EffectMgr::instance()->del(this);
    }
    
    bool Effect::isValid() const
    {
        return m_program != 0;
    }
    
    void Effect::bindAttribute(cocos2d::VertexAttribute index, const std::string & name)
    {
        glBindAttribLocation(m_program, index, name.c_str());
    }
    
}//end namespace my3d