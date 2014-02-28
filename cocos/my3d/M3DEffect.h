﻿//
//  Effect.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__Effect__
#define __cocos3d_libs__Effect__

#include "M3DBase.h"
#include "mytool/smartptr.h"

namespace cocos2d
{
    class ElementNode;
}

namespace my3d
{
    class EffectConstant;
    class EffectAutoConstant;
    class Shader;
    
    typedef SmartPtr<Shader> ShaderPtr;
    
    class Effect : public cocos2d::ISmartObject
    {
        CC_DISALLOW_COPY_AND_ASSIGN(Effect);
    public:
        
        explicit Effect(const std::string & resouce);
        virtual ~Effect();
        
        bool isValid() const;
        
        void bindAttribute(cocos2d::VertexAttribute index, const std::string & name);
        EffectConstant * getConstant(const std::string & name);
        
        bool begin();
        void end();

        static Effect * getActiveEffect();
        
    private:
        
        bool loadEffect(cocos2d::ElementNode * pNode);
        void parseConstants();
        void parseAttributes();
        
        void applyAutoConst();
        void applyAttributes();
        
    private:
        std::string m_resouce;
        cocos2d::ProgramHandle m_program;
        SmartPtr<Shader> m_vertexShader;
        SmartPtr<Shader> m_pixelShader;
        std::map<std::string, EffectConstant*> m_constants;
        std::map<std::string, GLint> m_attributes;
        std::vector<std::pair<EffectAutoConstant*, EffectConstant*>> m_autoConsts;

        static Effect * s_pActiveEffect;
    };
    
    typedef SmartPtr<Effect> EffectPtr;
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__Effect__) */
