//
//  Shader.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__Shader__
#define __cocos3d_libs__Shader__

#include "CCObject.h"
#include "../EnumDef.h"

namespace my3d
{
    enum class ShaderType
    {
        Vertex = 0,
        Pixel = 1,
    };
    
    class Shader : public cocos2d::Object
    {
        CC_DISALLOW_COPY_AND_ASSIGN(Shader);
    public:
        Shader(ShaderType type);
        Shader(ShaderType type, const std::string & resouce, const std::string & defines);
        virtual ~Shader();
        
        bool isValid() const;
        bool isCompiled() const;
        
        void setCode(const std::vector<std::string> & codes);
        bool compile();
        
        GLuint getShader() const { return m_shader; }
        
    private:
        bool createShader();
        void deleteShader();
        
        std::string m_resouce;
        ShaderType m_type;
        bool m_valid;
        bool m_compiled;
        GLuint m_shader;
        std::string m_error;
    };

}//end namespace my3d

#endif /* defined(__cocos3d_libs__Shader__) */
