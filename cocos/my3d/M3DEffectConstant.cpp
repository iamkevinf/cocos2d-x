﻿//
//  EffectConstant.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DEffectConstant.h"
#include "M3DColor.h"

using namespace cocos2d;

namespace my3d
{
    
    EffectConstant::EffectConstant()
        : m_location(-1)
        , m_type(0)
        , m_index(0)
        , m_pEffect(nullptr)
    {
    }
    
    EffectConstant::~EffectConstant()
    {
        for(auto it : m_children)
        {
            delete it.second;
        }
    }
    
    Effect* EffectConstant::getEffect() const
    {
        return m_pEffect;
    }
    
    const std::string& EffectConstant::getName() const
    {
        return m_name;
    }
    
    const GLenum EffectConstant::getType() const
    {
        return m_type;
    }
    
    EffectConstant *EffectConstant::getChild(const std::string & name, bool createIfMiss)
    {
        auto it = m_children.find(name);
        if(it != m_children.end()) return it->second;
        
        if(createIfMiss)
        {
            EffectConstant *p = new EffectConstant();
            m_children.insert(std::make_pair(name, p));
            return p;
        }
        
        return nullptr;
    }
    
    EffectConstant *EffectConstant::getChildren(const std::string & name, bool createIfMiss)
    {
        CCAssert(!name.empty(), "EffectConstant::getChild");
        
        size_t pos = name.find('.');
        if(pos == name.npos)
        {
            return getChild(name, createIfMiss);
        }
        
        EffectConstant *pChild = getChild(name.substr(0, pos), createIfMiss);
        if(!pChild) return nullptr;
        
        return pChild->getChildren(name.substr(pos+1), createIfMiss);
    }
    
    
    void EffectConstant::bindValue(float value)
    {
        glUniform1f(m_location, value);
    }
    
    void EffectConstant::bindValue(const float* values, unsigned int count)
    {
        glUniform1fv(m_location, count, values);
    }
    
    void EffectConstant::bindValue(int value)
    {
        glUniform1i(m_location, value);
    }
    
    void EffectConstant::bindValue(const int* values, unsigned int count)
    {
        glUniform1iv(m_location, count, values);
    }
    
    void EffectConstant::bindValue(const Matrix& value)
    {
        glUniformMatrix4fv(m_location, 1, GL_FALSE, value.m);
    }
    
    void EffectConstant::bindValue(const Matrix* values, unsigned int count)
    {
        glUniformMatrix4fv(m_location, count, GL_FALSE, (GLfloat*)values);
    }
    
    void EffectConstant::bindValue(const Vector2& value)
    {
        glUniform2f(m_location, value.x, value.y);
    }
    
    void EffectConstant::bindValue(const Vector2* values, unsigned int count)
    {
        glUniform2fv(m_location, count, (GLfloat*)values);
    }
    
    void EffectConstant::bindValue(const Vector3& value)
    {
        glUniform3f(m_location, value.x, value.y, value.z);
    }
    
    void EffectConstant::bindValue(const Vector3* values, unsigned int count)
    {
        glUniform3fv(m_location, count, (GLfloat*)values);
    }
    
    void EffectConstant::bindValue(const Vector4& value)
    {
        glUniform4f(m_location, value.x, value.y, value.z, value.w);
    }
    
    void EffectConstant::bindValue(const Vector4* values, unsigned int count)
    {
        glUniform4fv(m_location, count, (GLfloat*)values);
    }
    
    void EffectConstant::bindValue(const Color & color)
    {
        GL_ASSERT( glUniform4f(m_location, color.r, color.g, color.b, color.a) );
    }
    
    void EffectConstant::bindValue(const MaterialColor & color)
    {
        //GL_ASSERT( glUniform4fv(m_location, 4, (GLfloat*)&color) );
        
        EffectConstant *pConst = getChild("ambient");
        assert(pConst);
        pConst->bindValue(color.ambient);
        
        pConst = getChild("diffuse");
        assert(pConst);
        pConst->bindValue(color.diffuse);
        
        pConst = getChild("specular");
        assert(pConst);
        pConst->bindValue(color.specular);
        
        pConst = getChild("emissive");
        assert(pConst);
        pConst->bindValue(color.emissive);
    }
    
    void EffectConstant::bindValue(TexturePtr texture)
    {
        assert(m_type == GL_SAMPLER_2D);
        
        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + m_index));
        
        //binds the texture
        glBindTexture(GL_TEXTURE_2D, texture? texture->getHandle() : 0);
        
        glUniform1i(m_location, m_index);
    }

    //////////////////////////////////////////////////////////////////

    /*static*/ std::map<std::string, EffectAutoConstant*> EffectAutoConstant::s_autoConstMap;

    /*static*/ EffectAutoConstant * EffectAutoConstant::get(const std::string & name)
    {
        auto it = s_autoConstMap.find(name);
        if (it != s_autoConstMap.end()) return it->second;

        return nullptr;
    }

    /*static*/ void EffectAutoConstant::set(const std::string & name, EffectAutoConstant * autoConst)
    {
        s_autoConstMap.insert(std::make_pair(name, autoConst));
    }

    /*static*/ void EffectAutoConstant::fini()
    {
        for (auto it = s_autoConstMap.begin(); it != s_autoConstMap.end(); ++it)
        {
            delete it->second;
        }
        s_autoConstMap.clear();
    }

    
    EffectAutoConstant::EffectAutoConstant()
    {

    }

    EffectAutoConstant::~EffectAutoConstant()
    {

    }

}