//
//  M3DTextureMgr.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-24.
//  Copyright (c) 2014骞?zhoubao. All rights reserved.
//

#include "M3DTextureMgr.h"


namespace my3d
{
    TextureMgr::TextureMgr()
    {
        
    }
    
    TextureMgr::~TextureMgr()
    {
        
    }
    
    TexturePtr TextureMgr::get(const std::string & name, bool createIfMiss)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            return it->second;
        }
        
        if(createIfMiss)
        {
            TexturePtr tex = Texture::loadTexture(name);
            if(tex) tex->release();
            
            m_textures.insert(std::make_pair(name, tex.get()));
            return tex;
        }
        
        return nullptr;
    }
    
    void TextureMgr::del(Texture * pTex)
    {
        for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
        {
            if(it->second == pTex)
            {
                m_textures.erase(it);
                return;
            }
        }
    }
    
}//end namespace my3d