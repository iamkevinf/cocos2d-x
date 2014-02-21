//
//  EffectConstant.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__EffectConstant__
#define __cocos3d_libs__EffectConstant__

#include "Base.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix.h"

namespace cocos2d
{
    class C3DTexture;
    class C3DSampler;
    class C3DSamplerCube;
}

namespace my3d
{
    class Effect;
    
    /**
     * Represents a uniform variable within an effect.
     */
    class EffectConstant
    {
        friend class Effect;
        
    public:
        
        /**
         * Returns the name of this uniform.
         *
         * @return The name of the uniform.
         */
        const std::string& getName() const;
        
        /**
         * Returns the OpenGL uniform type.
         *
         * @return The OpenGL uniform type.
         */
        const GLenum getType() const;
        
        /**
         * Returns the effect for this uniform.
         *
         * @return The uniform's effect.
         */
        Effect* getEffect() const;
        
        
        void bindValue(float value);
        
        void bindValue(const float* values, unsigned int count = 1);
        
        void bindValue(int value);
        
        void bindValue(const int* values, unsigned int count = 1);
        
        void bindValue(const cocos2d::Matrix& value);
        
        void bindValue(const cocos2d::Matrix* values, unsigned int count = 1);
        
        void bindValue(const cocos2d::Vector2& value);
        
        void bindValue(const cocos2d::Vector2* values, unsigned int count = 1);
        
        void bindValue(const cocos2d::Vector3& value);
        
        void bindValue(const cocos2d::Vector3* values, unsigned int count = 1);
        
        void bindValue(const cocos2d::Vector4& value);
        
        void bindValue(const cocos2d::Vector4* values, unsigned int count = 1);
        
        void bindValue(const cocos2d::C3DSampler* sampler);
        
        void bindValue(const cocos2d::C3DTexture* texture);
        
    private:
        
        /**
         * Constructor.
         */
        EffectConstant();
        
        /**
         * Destructor.
         */
        ~EffectConstant();
        
        std::string m_name;
        GLint       m_location;
        GLenum      m_type;
        unsigned int m_index;
        Effect*     m_pEffect;
    };
}

#endif /* defined(__cocos3d_libs__EffectConstant__) */
