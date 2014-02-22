//
//  VertexDeclaration.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "VertexDeclaration.h"
#include "Effect.h"

#include <map>
#include <cassert>

namespace my3d
{
    namespace
    {
        const std::string EmptyString;
        std::map<VertexUsage, std::string> UsageMap;
    }
    
    const std::string & vertexUsage2Attr(VertexUsage usage)
    {
        if(UsageMap.empty())
        {
#define REGISTER_USAGE(USAGE, ATTRIBUTE) UsageMap[USAGE] = ATTRIBUTE
            
            REGISTER_USAGE(VertexUsage::POSITION, "a_position");
            REGISTER_USAGE(VertexUsage::NORMAL, "a_normal");
            REGISTER_USAGE(VertexUsage::COLOR, "a_color");
            REGISTER_USAGE(VertexUsage::TANGENT, "a_tangent");
            REGISTER_USAGE(VertexUsage::BINORMAL, "a_binormal");
            REGISTER_USAGE(VertexUsage::BLENDWEIGHTS, "a_blendWeights");
            REGISTER_USAGE(VertexUsage::BLENDINDICES, "a_blendIndices");
            REGISTER_USAGE(VertexUsage::TEXCOORD0, "a_texCoord0");
            REGISTER_USAGE(VertexUsage::TEXCOORD1, "a_texCoord1");
            REGISTER_USAGE(VertexUsage::TEXCOORD2, "a_texCoord2");
            REGISTER_USAGE(VertexUsage::TEXCOORD3, "a_texCoord3");
            REGISTER_USAGE(VertexUsage::TEXCOORD4, "a_texCoord4");
            REGISTER_USAGE(VertexUsage::TEXCOORD5, "a_texCoord5");
            REGISTER_USAGE(VertexUsage::TEXCOORD6, "a_texCoord6");
            REGISTER_USAGE(VertexUsage::TEXCOORD7, "a_texCoord7");
            
#undef REGISTER_USAGE
        }
        
        std::map<VertexUsage, std::string>::iterator it = UsageMap.find(usage);
        if(it != UsageMap.end()) return it->second;
        
        return EmptyString;
    }
    
    //////////////////////////////////////////////////////////////////
    VertexElement::VertexElement()
    {
        
    }
    
    VertexElement::VertexElement(VertexUsage usage_, int nComponent_)
    : usage(usage_)
    , nComponent(nComponent_)
    {
        
    }
    
    VertexElement::~VertexElement()
    {
        
    }
    
    int VertexElement::size() const
    {
        return sizeof(float) * nComponent;
    }
    
    //////////////////////////////////////////////////////////////////
    VertexDeclaration::VertexDeclaration()
        : m_vertexSize(0)
    {
        
    }
    VertexDeclaration::~VertexDeclaration()
    {
        
    }
    
    void VertexDeclaration::addElement(const VertexElement & e)
    {
        m_elements.push_back(e);
        m_vertexSize += e.size();
    }
    
    void VertexDeclaration::addElement(VertexUsage usage, int nComponent)
    {
        m_elements.push_back(VertexElement(usage, nComponent));
        m_vertexSize += m_elements.back().size();
    }
    
    const VertexElement & VertexDeclaration::getElement(int i) const
    {
        return m_elements[i];
    }
    
    size_t VertexDeclaration::getVertexSize() const
    {
        return m_vertexSize;
    }
    
    void VertexDeclaration::bind(Effect * pEffect)
    {
        if (pEffect == nullptr)
            pEffect = Effect::getActiveEffect();

        assert(pEffect != nullptr && "VertexDeclaration::bind failed! no active effect was found!");
        
        unsigned int offset = 0;
        for(GLuint i = 0; i < (GLuint)m_elements.size(); ++i)
        {
            VertexElement & e = m_elements[i];
            
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, e.nComponent, GL_FLOAT, GL_FALSE,
                                  GLsizei(m_vertexSize), reinterpret_cast<GLvoid*>(offset));
            
            pEffect->bindAttribute(i, vertexUsage2Attr(e.usage));
            
            offset += e.size();
        }
    }
    
    void VertexDeclaration::unbind()
    {
        for(GLuint i = 0; i < (GLuint)m_elements.size(); ++i)
        {
            glDisableVertexAttribArray(i);
        }
    }

}