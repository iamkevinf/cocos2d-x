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
#include "EnumDef.h"

namespace my3d
{
    class Effect : public cocos2d::Object
    {
    public:
        
        Effect();
        virtual ~Effect();
        
        void bindAttribute(cocos2d::VertexAttribute index, const std::string & name);
        
    private:
        cocos2d::ProgramHandle m_program;
    };
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__Effect__) */
