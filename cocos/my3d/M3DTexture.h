#ifndef H_M3D_TEXTURE_H
#define H_M3D_TEXTURE_H

#include "../3d/C3DTexture.h"
#include "M3DRenderState.h"
#include "mytool/smartptr.h"

namespace my3d
{
    typedef cocos2d::C3DTexture Texture;
    typedef SmartPtr<cocos2d::C3DTexture> TexturePtr;
    
    const int MAX_TEXTURE_STAGE = 8;
    
    class TextureStage
    {
    public:
        TextureStage();
        ~TextureStage();
        
        void setMinFilter(TextureFilter filter){ m_minFilter = filter; }
        TextureFilter getMinFilter() const { return m_minFilter; }
        
        void setMagFilter(TextureFilter filter){ m_magFilter = filter; }
        TextureFilter getMagFilter() const { return m_magFilter; }
        
        void setUWrap(TextureWrap wrap){ m_uWrap = wrap; }
        TextureWrap getUWrap() const { return m_uWrap; }
        
        void setVWrap(TextureWrap wrap){ m_vWrap = wrap; }
        TextureWrap getVWrap() const { return m_vWrap; }
        
        TexturePtr getTexture(){ return m_texture; }
        void setTexture(TexturePtr tex){ m_texture = tex; }
        
    private:
        
        TextureWrap m_uWrap;
        TextureWrap m_vWrap;
        TextureFilter m_minFilter;
        TextureFilter m_magFilter;
        TexturePtr  m_texture;
    };
    
}

#endif //H_M3D_TEXTURE_H