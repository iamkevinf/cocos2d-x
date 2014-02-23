#include "M3DInit.h"
#include "M3DRenderDevice.h"
#include "EffectAutoConstImp.h"

namespace my3d
{
    static RenderDevice * s_pRenderDev = nullptr;

    RenderDevice * renderDev()
    {
        assert(s_pRenderDev != nullptr && "please call 'init' first!");
        return s_pRenderDev;
    }

    bool init()
    {
        assert(s_pRenderDev == nullptr);
        s_pRenderDev = new RenderDevice();

#define REG_EFFECT_CONST_FACTORY(TYPE, FACTORY) EffectAutoConstant::set(TYPE, new FACTORY())

        REG_EFFECT_CONST_FACTORY(EffectConstType::World, EffectWorldConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::ViewProj, EffectViewProjConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::WorldViewProj, EffectWorldViewProjConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::AmbientColor, EffectAmbientConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::OmitLight, EffectOmitLightConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::DirLight, EffectDirLightConst);
        REG_EFFECT_CONST_FACTORY(EffectConstType::SpotLight, EffectSpotLightConst);

#undef REG_EFFECT_CONST_FACTORY

        return true;
    }

    void fini()
    {
        if (s_pRenderDev != nullptr)
        {
            delete s_pRenderDev;
            s_pRenderDev = nullptr;
        }

        EffectAutoConstant::fini();
    }

}