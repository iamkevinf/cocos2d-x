//
//  VertexDeclaration.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__VertexDeclaration__
#define __cocos3d_libs__VertexDeclaration__

#include "M3DBase.h"
#include "mytool/singleton.h"

namespace my3d
{
    class Effect;
    
    /**
     * Defines a set of usages for vertex elements.
     */
    enum class VertexUsage
    {
        POSITION = 1,
        NORMAL = 2,
        COLOR = 3,
        TANGENT = 4,
        BINORMAL = 5,
        BLENDWEIGHTS = 6,
        BLENDINDICES = 7,
        TEXCOORD0 = 8,
        TEXCOORD1 = 9,
        TEXCOORD2 = 10,
        TEXCOORD3 = 11,
        TEXCOORD4 = 12,
        TEXCOORD5 = 13,
        TEXCOORD6 = 14,
        TEXCOORD7 = 15
    };
    
    
    const std::string & vertexUsage2Attr(VertexUsage usage);
    
    
    /**
     * Defines a single element within a vertex format.
     *
     * All vertex elements are assumed to be of type float, but can
     * have a varying number of float values (1-4), which is represented
     * by the size attribute. Additionally, vertex elements are assumed
     * to be tightly packed.
     */
    class VertexElement
    {
    public:
        
        VertexUsage usage;//The vertex element usage semantic.
        
        int nComponent;//The number of values in the vertex element.
        
        VertexElement();
        
        VertexElement(VertexUsage usage, int nComponent);
        
        ~VertexElement();
        
        int size() const;
    };
    
    
    class VertexDeclaration : public cocos2d::ISmartObject
    {
    public:
        
        VertexDeclaration();
        virtual ~VertexDeclaration();
        
        void addElement(const VertexElement & e);
        void addElement(VertexUsage usage, int nComponent);
        const VertexElement & getElement(int i) const;
        size_t getVertexSize() const;
        
        void bind();
        void unbind();
        
    private:
        
        void bindEffectAttr(Effect * pEffect);
        
        
        size_t m_vertexSize;
        std::vector<VertexElement> m_elements;
        
        static VertexDeclaration * s_pActiveDecl;
        friend class Effect;
    };
    
    typedef SmartPtr<VertexDeclaration> VertexDeclarationPtr;
    
    class VertexDeclMgr : public Singleton<VertexDeclMgr>
    {
    public:
        VertexDeclMgr();
        ~VertexDeclMgr();
        
        void init();
        
        void add(const std::string & name, VertexDeclarationPtr decl);
        
        VertexDeclarationPtr get(const std::string & name);
        
    private:
        std::map<std::string, VertexDeclarationPtr> m_decls;
    };

}//end namespace my3d


#endif /* defined(__cocos3d_libs__VertexDeclaration__) */
