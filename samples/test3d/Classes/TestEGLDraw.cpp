//
//  TestEGL.cpp
//  test3d
//
//  Created by zhoubao on 14-2-21.
//
//

#include "TestEGLDraw.h"

namespace
{

    GLuint LoadShader(GLenum type, const char *shaderSrc)
    {
        GLuint shader;
        GLint compiled;
        
        // Create the shader object
        shader = glCreateShader(type);
        if(shader == 0)
            return 0;
        
        // Load the shader source
        glShaderSource(shader, 1, &shaderSrc, NULL);
        
        // Compile the shader
        glCompileShader(shader);
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        
        if(!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if(infoLen > 1)
            {
                char* infoLog = (char*)malloc(sizeof(char) * infoLen);
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                CCLOGERROR("Error compiling shader:\n%s\n", infoLog);
                free(infoLog);
            }
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }
}

TestEGLDrawNode::TestEGLDrawNode()
: m_programObject(0)
{
    
}

TestEGLDrawNode::~TestEGLDrawNode()
{
    
}

bool TestEGLDrawNode::initTest3D()
{
    const char *vShaderStr =
    "attribute vec4 vPosition; \n"
    "void main()\n"
    "{\n"
    " gl_Position = vPosition; \n"
    "}\n";
    
    const char *fShaderStr =
    "void main()\n"
    "{\n"
    " gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
    "} \n";
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;
    
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
    
    // Create the program object
    programObject = glCreateProgram();
    if(programObject == 0)
        return 0;
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    // Bind vPosition to attribute 0
    glBindAttribLocation(programObject, 0, "vPosition");
    
    // Link the program
    glLinkProgram(programObject);
    
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            CCLOGERROR("linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }
    
    m_programObject = programObject;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return true;
}

/*virtual*/ void TestEGLDrawNode::draw()
{
    GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
    
    glUseProgram(m_programObject);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}