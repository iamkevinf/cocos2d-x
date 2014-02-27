#ifndef H_M3D_LIGHT_DIR_H
#define H_M3D_LIGHT_DIR_H

#include "M3DBase.h"
#include "M3DColor.h"
#include "mytool/smartptr.h"

namespace my3d
{

    class DirLight : public cocos2d::ISmartObject
    {
    public:
        DirLight();
        ~DirLight();

        void setColor(const Color & cr){ m_color = cr; }
        const Color & getColor() const { return m_color; }

        void setDirection(const cocos2d::Vector3 & dir){ m_direction = dir; }
        const cocos2d::Vector3 & getDirection() const { return m_direction; }

        void getEffectData(cocos2d::Vector4 data[2]);
    private:
        cocos2d::Vector3 m_direction;
        Color   m_color;
    };
    
    typedef SmartPtr<DirLight> DirLightPtr;

}//end namespace my3d

#endif //H_M3D_LIGHT_DIR_H