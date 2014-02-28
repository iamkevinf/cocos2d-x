//
//  VertexDeclaration.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DVertexDeclaration.h"
#include "M3DEffect.h"
#include "M3DVertex.h"

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
    /*static*/ VertexDeclaration * VertexDeclaration::s_pActiveDecl = nullptr;
    
    VertexDeclaration::VertexDeclaration()
        : m_vertexSize(0)
    {
        
    }
    
    VertexDeclaration::~VertexDeclaration()
    {
        if(this == s_pActiveDecl) s_pActiveDecl = nullptr;
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
    
    const VertexElement & VertexDeclaration::getElement(size_t i) const
    {
        return m_elements[i];
    }
    
    size_t VertexDeclaration::getNumElement() const
    {
        return m_elements.size();
    }
    
    size_t VertexDeclaration::getVertexSize() const
    {
        return m_vertexSize;
    }
    
    void VertexDeclaration::bind()
    {
        s_pActiveDecl = this;
    }
    
    void VertexDeclaration::unbind()
    {
        assert(s_pActiveDecl == this && "VertexDeclaration::unbind - invalid operation!");
        s_pActiveDecl = nullptr;
    }
    
    //////////////////////////////////////////////////////////////////////////
    VertexDeclMgr::VertexDeclMgr()
    {
        
    }
    
    VertexDeclMgr::~VertexDeclMgr()
    {
        
    }
    
    void VertexDeclMgr::init()
    {
        VertexDeclarationPtr decl;
        
        decl = new VertexDeclaration();
        decl->addElement(VertexUsage::POSITION, 3);
        add(VertexXYZ::getType(), decl);
        
        decl = new VertexDeclaration();
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::COLOR, 4);
        add(VertexXYZColor::getType(), decl);
        
        decl = new VertexDeclaration();
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::NORMAL, 3);
        add(VertexXYZN::getType(), decl);
        
        decl = new VertexDeclaration();
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::TEXCOORD0, 2);
        add(VertexXYZUV::getType(), decl);
        
        decl = new VertexDeclaration();
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::NORMAL, 3);
        decl->addElement(VertexUsage::TEXCOORD0, 2);
        add(VertexXYZNUV::getType(), decl);
    }
    
    void VertexDeclMgr::add(const std::string & name, VertexDeclarationPtr decl)
    {
        auto it = m_decls.find(name);
        if (it != m_decls.end())
        {
            CCLOGERROR("the vertex declaration '%s' has been exist!", name.c_str());
            assert(0);
            return;
        }
        m_decls.insert(std::make_pair(name, decl));
    }
    
    VertexDeclarationPtr VertexDeclMgr::get(const std::string & name)
    {
        auto it = m_decls.find(name);
        if(it != m_decls.end()) return it->second;
        
        return nullptr;
    }

}