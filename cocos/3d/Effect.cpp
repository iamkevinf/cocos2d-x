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
#include "ElementNode.h"
#include "EffectConstant.h"

namespace my3d
{
    static Effect * s_pEffectBegined = nullptr;
    
    //////////////////////////////////////////////////////////
    Effect::Effect(const std::string & resouce)
    : m_program(0)
    , m_resouce(resouce)
    {
        cocos2d::ElementNode* doc = cocos2d::ElementNode::create(resouce.c_str());
        if (!doc)
        {
            CCLOGERROR("loading effect: Could not load file: %s", resouce.c_str());
            return;
        }
        
        loadEffect(doc);
    }
    
    Effect::~Effect()
    {
        EffectMgr::instance()->del(this);
        
        for(auto it : m_constants)
        {
            delete it.second;
        }
        
        if(m_program != 0)
        {
            glDeleteProgram(m_program);
        }
    }
    
    bool Effect::isValid() const
    {
        return m_program != 0;
    }
    
    bool Effect::loadEffect(cocos2d::ElementNode * pNode)
    {
        assert(pNode != nullptr);
        
        std::string vshFile = pNode->getString("vsh");
        std::string pshFile = pNode->getString("psh");
        std::string defines = pNode->getString("defines");
        
        if(vshFile.empty() || pshFile.empty()) return false;
        
        size_t pos = m_resouce.find_last_of('/');
        if(pos != m_resouce.npos)
        {
            std::string path(m_resouce, 0, pos);
            path += "/";
            
            vshFile = path + vshFile;
            pshFile = path + pshFile;
        }
        
        ShaderPtr vshPtr = new Shader(ShaderType::Vertex, vshFile, defines);
        if(!vshPtr->compile())
        {
            return false;
        }
        
        ShaderPtr pshPtr = new Shader(ShaderType::Pixel, pshFile, defines);
        if(!pshPtr->compile())
        {
            return false;
        }
        
        m_vertexShader = vshPtr;
        m_pixelShader = pshPtr;
        
        m_program = glCreateProgram();
        if(m_program == 0) return false;
        
        glAttachShader(m_program, m_vertexShader->getShader());
        glAttachShader(m_program, m_pixelShader->getShader());
        
        glLinkProgram(m_program);
        
        int result;
        glGetProgramiv(m_program, GL_LINK_STATUS, &result);
        if(result != GL_TRUE)
        {
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &result);
            if(result > 0)
            {
                std::string error(result, '\0');
                glGetProgramInfoLog(m_program, result, nullptr, &error[0]);
                CCLOGERROR("link effect failed '%s'!", error.c_str());
            }
            else
            {
                CCLOGERROR("link effect failed!");
            }
            
            return false;
        }
        
        parseConstants();
        
        return true;
    }
    
    void Effect::parseConstants()
    {
        GLint nUniforms;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &nUniforms);
        
        if(nUniforms == 0) return;
        
        GLint nMaxNameLen;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nMaxNameLen);

        char * uniformName = new char[nMaxNameLen+1];
        unsigned int samplerIndex = 0;
        for(int i = 0; i < nUniforms; ++i)
        {
            GLsizei len;
            GLint num;
            GLenum type;
            glGetActiveUniform(m_program, i, nMaxNameLen, &len, &num, &type, uniformName);
            uniformName[len] = '\0';
            
            GLint location = glGetUniformLocation(m_program, uniformName);
            
            EffectConstant* uniform = new EffectConstant();
            uniform->m_pEffect = this;
            uniform->m_name = uniformName;
            uniform->m_location = location;
            uniform->m_type = type;
            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
                uniform->m_index = samplerIndex++;
            else
                uniform->m_index = 0;
            
            this->m_constants[uniformName] = uniform;
        }
    }
    
    void Effect::bindAttribute(cocos2d::VertexAttribute index, const std::string & name)
    {
        glBindAttribLocation(m_program, index, name.c_str());
    }
    
    EffectConstant * Effect::getConstant(const std::string & name)
    {
        auto it = m_constants.find(name);
        if(it != m_constants.end()) return it->second;
        
        return nullptr;
    }
    
    bool Effect::begin()
    {
        CCAssert(s_pEffectBegined == nullptr, "Effect::begin invalid!" );
        
        if(m_program == 0) return false;
        
        glUseProgram(m_program);
        s_pEffectBegined = this;
        return true;
    }
    
    void Effect::end()
    {
        CCAssert(s_pEffectBegined == this, "Effect::end invalied");
        
        s_pEffectBegined = nullptr;
        glUseProgram(0);
    }
    
}//end namespace my3d