#ifndef H_M3D_MATERIAL_H
#define H_M3D_MATERIAL_H

#include "M3DColor.h"
#include "M3DTexture.h"
#include "M3DEffect.h"

namespace my3d
{

    class Material : public cocos2d::ISmartObject
    {
    public:
        Material();
        ~Material();
        
        bool load(const std::string & resouce);
        
        bool begin();
        void end();
        
        const std::string & getResouce() const { return m_resouce; }
        void setEffect(EffectPtr eff){ m_effect = eff; }
        
        void setMaterialColor(const MaterialColor & color){ m_mtlColor = color; }

        void setTextureStage(int i, const TextureStage & stage);
        void addTextureStage(const TextureStage & stage);
        void clearTextureStage();
        
        void setLightEnable(bool enable){ m_bLight = enable; }
        bool getLightEnable() const { return m_bLight;}
        
    private:
        std::string m_resouce;
        MaterialColor m_mtlColor;
        
        bool m_bLight;
        EffectPtr m_effect;
        
        std::vector<TextureStage> m_texStage;
    };
    
    typedef SmartPtr<Material> MaterialPtr;

}//end namespace my3d

#endif //H_M3D_MATERIAL_H