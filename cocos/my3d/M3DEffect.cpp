//
//  Effect.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DEffect.h"
#include "M3DEffectMgr.h"
#include "M3DShader.h"
#include "M3DEffectConstant.h"
#include "M3DVertexDeclaration.h"

#include "../3d/ElementNode.h"

const std::string OPENGL_ES_DEFINE = "#define OPENGL_ES";

namespace my3d
{
    /*static*/ Effect * Effect::s_pActiveEffect = nullptr;

    /*static*/ Effect * Effect::getActiveEffect()
    {
        return s_pActiveEffect;
    }
    
    //////////////////////////////////////////////////////////
    Effect::Effect(const std::string & resouce)
    : m_program(0)
    , m_resouce(resouce)
    , m_pConstRoot(nullptr)
    {
        cocos2d::ElementNode* doc = cocos2d::ElementNode::create(resouce.c_str());
        if (!doc)
        {
            CCLOGERROR("loading effect: Could not load file: %s", resouce.c_str());
            return;
        }
        
        m_pConstRoot = new EffectConstant("root");
        
        if(!loadEffect(doc))
        {
            CCLOGERROR("load effect failed!");
        }
    }
    
    Effect::~Effect()
    {
        EffectMgr::instance()->del(this);

        if (this == s_pActiveEffect)
        {
            s_pActiveEffect = nullptr;
        }
        
        if(m_pConstRoot != nullptr)
        {
            delete m_pConstRoot;
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
        
        defines = OPENGL_ES_DEFINE + defines;
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
        
        parseAttributes();
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
            
            EffectConstant* uniform = m_pConstRoot->getChildren(uniformName, true);
            uniform->m_pEffect = this;
            uniform->m_location = location;
            uniform->m_type = type;
            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
                uniform->m_index = samplerIndex++;
            else
                uniform->m_index = 0;
        }
        
        for(auto it : m_pConstRoot->m_children)
        {
            EffectAutoConstant *pAutoConst = EffectAutoConstant::get(it.first);
            if (pAutoConst)
            {
                m_autoConsts.push_back(std::make_pair(pAutoConst, it.second));
            }
        }
    }
    
    void Effect::bindAttribute(cocos2d::VertexAttribute index, const std::string & name)
    {
        GL_ASSERT(glBindAttribLocation(m_program, index, name.c_str()));
    }
    
    EffectConstant * Effect::getConstant(const std::string & name)
    {
        return m_pConstRoot->getChildren(name);
    }
    
    bool Effect::begin()
    {
        CCAssert(s_pActiveEffect == nullptr, "Effect::begin - invalid operation!");
        
        if(m_program == 0) return false;
        
        if(VertexDeclaration::getActiveDecl() == nullptr)
        {
            CCAssert(0, "Effect::begin - failed! please set vertex declaration first.");
            return false;
        }
        
        glUseProgram(m_program);
        s_pActiveEffect = this;
        
        applyAttributes();
        applyAutoConst();

        return true;
    }
    
    void Effect::end()
    {
        CCAssert(s_pActiveEffect == this, "Effect::end invalied");
        
        s_pActiveEffect = nullptr;
        glUseProgram(0);
    }
    
    void Effect::applyAutoConst()
    {
        for (auto it : m_autoConsts)
        {
            it.first->apply(it.second);
        }
    }
    
    void Effect::applyAttributes()
    {
        VertexDeclaration *decl = VertexDeclaration::getActiveDecl();
        
        GLsizei vertexSize = GLsizei(decl->getVertexSize());
        unsigned int offset = 0;
        for(size_t i = 0; i < decl->getNumElement(); ++i)
        {
            const VertexElement & e = decl->getElement(i);
            
            const std::string & attrName = vertexUsage2Attr(e.usage);
            auto it = m_attributes.find(attrName);
            if(it == m_attributes.end())
            {
                CCLOGERROR("The effect attribute '%s' was not found!", attrName.c_str());
                assert(0);
                continue;
            }
            
            GL_ASSERT( glVertexAttribPointer(it->second, e.nComponent, GL_FLOAT, GL_FALSE,
                vertexSize, (GLvoid*)offset) );
            GL_ASSERT( glEnableVertexAttribArray(it->second) );
            offset += e.size();
        }
    }
    
    void Effect::parseAttributes()
    {
        GLint activeAttributes;
        GL_ASSERT( glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
        if (activeAttributes <= 0) return;
        
        int length;
        GL_ASSERT( glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
        if (length <= 0) return;
        
        GLchar* attribName = new GLchar[length + 1];
        GLint attribSize;
        GLenum attribType;
        GLint attribLocation;
        for (int i = 0; i < activeAttributes; ++i)
        {
            // Query attribute info.
            GL_ASSERT( glGetActiveAttrib(m_program, i, length, nullptr, &attribSize, &attribType, attribName) );
            attribName[length] = '\0';
            
            // Query the pre-assigned attribute location.
            GL_ASSERT( attribLocation = glGetAttribLocation(m_program, attribName) );
            
            // Assign the vertex attribute mapping for the effect.
            m_attributes[attribName] = attribLocation;
        }
        SAFE_DELETE_ARRAY(attribName);
    }
    
}//end namespace my3d