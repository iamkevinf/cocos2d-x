#include "EffectAutoConstImp.h"
#include "M3DRenderDevice.h"

namespace my3d
{
    
    EffectWorldConst::EffectWorldConst()
    {
    }

    EffectWorldConst::~EffectWorldConst()
    {
    }

    void EffectWorldConst::apply(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getWorld());
    }

    //////////////////////////////////////////////////////////////////

    EffectViewProjConst::EffectViewProjConst()
    {

    }

    EffectViewProjConst::~EffectViewProjConst()
    {

    }

    void EffectViewProjConst::apply(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getViewProjection());
    }

    //////////////////////////////////////////////////////////////////
    
    EffectWorldViewProjConst::EffectWorldViewProjConst()
    {

    }

    EffectWorldViewProjConst::~EffectWorldViewProjConst()
    {

    }

    void EffectWorldViewProjConst::apply(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getWorldViewProjection());
    }

    //////////////////////////////////////////////////////////////////


    EffectAmbientConst::EffectAmbientConst()
    {
    }

    EffectAmbientConst::~EffectAmbientConst()
    {
    }

    void EffectAmbientConst::apply(EffectConstant *pConst)
    {
    }

    //////////////////////////////////////////////////////////////////

   
    EffectOmitLightConst::EffectOmitLightConst()
    {

    }
    EffectOmitLightConst::~EffectOmitLightConst()
    {

    }

    void EffectOmitLightConst::apply(EffectConstant *pConst)
    {

    }


    //////////////////////////////////////////////////////////////////

    
    EffectDirLightConst::EffectDirLightConst()
    {

    }
    EffectDirLightConst::~EffectDirLightConst()
    {

    }

    void EffectDirLightConst::apply(EffectConstant *pConst)
    {

    }

    
    //////////////////////////////////////////////////////////////////

    
    EffectSpotLightConst::EffectSpotLightConst()
    {
    }

    EffectSpotLightConst::~EffectSpotLightConst()
    {

    }

    void EffectSpotLightConst::apply(EffectConstant *pConst)
    {

    }

   
}//end namespace my3d