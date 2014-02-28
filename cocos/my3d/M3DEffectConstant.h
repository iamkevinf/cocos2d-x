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
#include "M3DTexture.h"

namespace my3d
{
    class Effect;
    class Color;
    struct MaterialColor;

    namespace EffectConstType
    {
        const std::string World = "u_matWorld";
        const std::string View = "u_matView";
        const std::string Proj = "u_matProj";
        const std::string ViewProj = "u_matViewProj";
        const std::string WorldViewProj = "u_matWorldViewProj";
        const std::string AmbientColor = "u_ambient";
        const std::string OmitLight = "u_omitLight";
        const std::string DirLight = "u_dirLight";
        const std::string DirLightDir = "u_dirLightDir";
        const std::string DirLightColor = "u_dirLightColor";
        const std::string SpotLight = "u_spotLight";
        const std::string Texture = "u_texture";
        const std::string Texture0 = "u_texture0";
        const std::string Texture1 = "u_texture1";
        const std::string Texture2 = "u_texture2";
        const std::string Texture3 = "u_texture3";
        const std::string Texture4 = "u_texture4";
        const std::string Texture5 = "u_texture5";
        const std::string Texture6 = "u_texture6";
        const std::string Texture7 = "u_texture7";
        const std::string Material = "u_material";
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
        
        EffectConstant *getChild(const std::string & name, bool createIfMiss=false);
        EffectConstant *getChildren(const std::string & name, bool createIfMiss=false);
        
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
        
        void bindValue(const Color & color);
        
        void bindValue(const MaterialColor & color);
        
        void bindValue(const TexturePtr texture);
        
    private:
        
        EffectConstant();
        ~EffectConstant();
        
        std::string m_name;
        GLint       m_location;
        GLenum      m_type;
        uint32      m_index;
        Effect*     m_pEffect;
        std::map<std::string, EffectConstant*> m_children;
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
