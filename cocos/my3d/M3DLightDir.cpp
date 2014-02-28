#include "M3DLightDir.h"

namespace my3d
{

    DirLight::DirLight()
    {
    }


    DirLight::~DirLight()
    {
    }
    
    void DirLight::getEffectData(DirLightInfo & info)
    {
        info.direction.set(m_direction.x, m_direction.y, m_direction.z, 1.0f);
        info.color = m_color;
    }

}//end namespace my3d