#ifndef H_M3D_TEXTURE_H
#define H_M3D_TEXTURE_H

#include "../3d/C3DTexture.h"
#include "mytool/smartptr.h"

namespace my3d
{
    typedef cocos2d::C3DTexture Texture;
    typedef SmartPtr<cocos2d::C3DTexture> TexturePtr;
    
}

#endif //H_M3D_TEXTURE_H