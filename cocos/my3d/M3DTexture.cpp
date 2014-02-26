#include "M3DTexture.h"

namespace my3d
{
    TextureStage::TextureStage()
        : m_uWrap(TextureWrap::Repeat)
        , m_vWrap(TextureWrap::Repeat)
        , m_minFilter(TextureFilter::Near)
        , m_magFilter(TextureFilter::Near)
    {
        
    }
    
    TextureStage::~TextureStage()
    {
        
    }
    
}//end namespace my3d