//
//  M3DTextureMgr.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-24.
//  Copyright (c) 2014年zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__M3DTextureMgr__
#define __cocos3d_libs__M3DTextureMgr__


#include "mytool/singleton.h"
#include "M3DTexture.h"

namespace my3d
{
    class TextureMgr : public Singleton<TextureMgr>
    {
    public:
        TextureMgr();
        ~TextureMgr();
        
        TexturePtr get(const std::string & name, bool createIfMiss=true);
        void del(Texture * pTex);
        
    private:
        std::map<std::string, Texture*> m_textures;
    };
    
}//end namespace my3d


#endif /* defined(__cocos3d_libs__M3DTextureMgr__) */
