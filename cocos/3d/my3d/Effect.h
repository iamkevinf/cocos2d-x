//
//  Effect.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__Effect__
#define __cocos3d_libs__Effect__

#include "CCObject.h"
#include "../EnumDef.h"
#include "mytool/smartptr.h"

#include <cassert>

namespace cocos2d
{
    class ElementNode;
}

namespace my3d
{
    class EffectConstant;
    class Shader;
    
    typedef SmartPtr<Shader> ShaderPtr;
    
    class Effect : public cocos2d::Object
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
        
    private:
        
        bool loadEffect(cocos2d::ElementNode * pNode);
        void parseConstants();
        
    private:
        std::string m_resouce;
        cocos2d::ProgramHandle m_program;
        SmartPtr<Shader> m_vertexShader;
        SmartPtr<Shader> m_pixelShader;
        std::map<std::string, EffectConstant*> m_constants;
    };
    
    typedef SmartPtr<Effect> EffectPtr;
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__Effect__) */
