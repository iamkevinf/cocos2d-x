//
//  M3DTextureMgr.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-24.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__M3DTextureMgr__
#define __cocos3d_libs__M3DTextureMgr__

#include "C3DTexture.h"
#include "mytool/singleton.h"
#include "mytool/smartptr.h"

namespace my3d
{
    typedef cocos2d::C3DTexture Texture;
    typedef SmartPtr<cocos2d::C3DTexture> TexturePtr;
    
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
