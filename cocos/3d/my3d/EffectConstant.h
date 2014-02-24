//
//  EffectConstant.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__EffectConstant__
#define __cocos3d_libs__EffectConstant__

#include "M3DBase.h"

namespace cocos2d
{
    class C3DTexture;
    class C3DSampler;
    class C3DSamplerCube;
}

namespace my3d
{
    class Effect;

    namespace EffectConstType
    {
        const std::string World = "u_matWorld";
        const std::string View = "u_matView";
        const std::string Proj = "u_matProj";
        const std::string ViewProj = "u_matViewProj";
        const std::string WorldViewProj = "u_matWorldViewProj";
        const std::string AmbientColor = "u_ambientColor";
        const std::string OmitLight = "u_omitLight";
        const std::string DirLight = "u_dirLight";
        const std::string SpotLight = "u_spotLight";
        const std::string Texture = "u_texture";
        const std::string Sampler = "u_sampler";
    }
    
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

    class EffectAutoConstant
    {
    public:

        virtual void apply(EffectConstant *pConst) = 0;

        static EffectAutoConstant * get(const std::string & name);
        static void set(const std::string & name, EffectAutoConstant *autoConst);
        static void fini();

    protected:
        EffectAutoConstant();
        virtual ~EffectAutoConstant();

        static std::map<std::string, EffectAutoConstant*> s_autoConstMap;
    };
}

#endif /* defined(__cocos3d_libs__EffectConstant__) */
