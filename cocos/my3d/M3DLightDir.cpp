#include "M3DLightDir.h"

namespace my3d
{

    DirLight::DirLight()
    {
    }


    DirLight::~DirLight()
    {
    }
    
    void DirLight::getEffectData(cocos2d::Vector4 data[2])
    {
        data[0].set(m_direction.x, m_direction.y, m_direction.z, 1.0f);
        data[1].set(m_color.r, m_color.g, m_color.g, m_color.a);
    }

}//end namespace my3d