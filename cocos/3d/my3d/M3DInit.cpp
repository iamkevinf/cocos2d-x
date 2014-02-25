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

#define REG_EFFECT_CONST_FACTORY(TYPE, FACTORY) EffectAutoConstant::set(TYPE, new EffectConstantProxy(FACTORY))

        REG_EFFECT_CONST_FACTORY(EffectConstType::World, effectApplyWorld);
        REG_EFFECT_CONST_FACTORY(EffectConstType::ViewProj, effectApplyViewProj);
        REG_EFFECT_CONST_FACTORY(EffectConstType::WorldViewProj, effectApplyWorldViewProj);
        REG_EFFECT_CONST_FACTORY(EffectConstType::AmbientColor, effectApplyAmbient);
        REG_EFFECT_CONST_FACTORY(EffectConstType::OmitLight, effectApplyOmitLight);
        REG_EFFECT_CONST_FACTORY(EffectConstType::DirLight, effectApplyDirLight);
        REG_EFFECT_CONST_FACTORY(EffectConstType::SpotLight, effectApplySpotLight);

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