//
//  Shader.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "Shader.h"
#include "Stream.h"

namespace my3d
{
    Shader::Shader(ShaderType type)
        : m_valid(false)
        , m_type(type)
        , m_compiled(false)
    {
        createShader();
    }
    
    Shader::Shader(ShaderType type, const std::string & resouce, const std::string & defines)
        : m_valid(false)
        , m_type(type)
        , m_compiled(false)
        , m_resouce(resouce)
    {
        int length;
        char* source = cocos2d::StreamManager::readAll(resouce.c_str(), &length);
        if(source != nullptr)
        {
            std::string code(source, length);
            delete source;
            
            createShader();
            
            std::vector<std::string> codes;
            codes.push_back(defines);
            
#ifdef OPENGL_ES
            if (!codes.back().empty())
                codes.back() += "\n";
            codes.back() += OPENGL_ES_DEFINE;
#endif
            codes.push_back("\n");
            codes.push_back(code);
            
            setCode(codes);
        }
        else
        {
            CCLOGERROR("load shader '%s' failed!", resouce.c_str());
        }
    }
    
    Shader::~Shader()
    {
        deleteShader();
    }
    
    bool Shader::createShader()
    {
        m_shader = glCreateShader(m_type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        if(m_shader == 0)
        {
            CCLOGERROR("create shader failed!");
        }
        
        return m_shader != 0;
    }
    
    void Shader::deleteShader()
    {
        if(m_shader != 0)
        {
            glDeleteShader(m_shader);
        }
    }
    
    bool Shader::isValid() const
    {
        return m_shader != 0 && m_valid;
    }
    
    bool Shader::isCompiled() const
    {
        return m_compiled;
    }
    
    void Shader::setCode(const std::vector<std::string> & codes)
    {
        if(codes.empty()) return;
        
        assert(m_shader != 0);
        
        int n = int(codes.size());
        const GLchar **strings = new const GLchar*[n];
        GLint *lengths = new GLint[n];
        
        for(int i = 0; i<n; ++i)
        {
            strings[i] = codes[i].c_str();
            lengths[i] = GLint(codes[i].size());
        }
        
        glShaderSource(m_shader, n, strings, lengths);
        
        delete lengths;
        delete strings;
    }
    
    bool Shader::compile()
    {
        if(m_compiled) return false;
        
        glCompileShader(m_shader);
        m_compiled = true;
        
        GLint result;
        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &result);
        m_valid = (result == GL_TRUE);
        
        if(!m_valid)
        {
            glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &result);
            if(result > 0)
            {
                m_error.resize(result);
                glGetShaderInfoLog(m_shader, result, nullptr, &m_error[0]);
            }
        }
        
        return m_valid;
    }
    
}//end namespace my3d