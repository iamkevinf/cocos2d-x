#include "M3DEffectAutoConstImp.h"
#include "M3DRenderDevice.h"
#include "M3DEffect.h"

namespace my3d
{
    EffectConstantProxy::EffectConstantProxy(EffectApplyFun fun)
        : m_fun(fun)
    {
        assert(fun && "EffectConstantFun - fun must not be null!");
    }
    
    EffectConstantProxy::~EffectConstantProxy()
    {
        
    }
    
    void EffectConstantProxy::apply(EffectConstant *pConst)
    {
        (*m_fun)(pConst);
    }
    
    //////////////////////////////////////////////////////////////////

    void effectApplyWorld(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getWorld());
    }


    void effectApplyViewProj(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getViewProjection());
    }

    void effectApplyWorldViewProj(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getWorldViewProjection());
    }

    void effectApplyAmbient(EffectConstant *pConst)
    {
        Color color;
        
        LightContainerPtr lights =  renderDev()->getLightContainer();
        if(lights) color = lights->getAmbientColor();
        
        pConst->bindValue(color);
    }

    void effectApplyOmitLight(EffectConstant *pConst)
    {

    }

    void effectApplyDirLight(EffectConstant *pConst)
    {
        LightContainerPtr lights =  renderDev()->getLightContainer();
        if(!lights)
        {
            pConst->bindValue(false);
            return;
        }
        
        DirLightVector & dirLights = lights->getDirLights();
        if(dirLights.size() > 0)
        {
            DirLightPtr p = dirLights.front();
            
            EffectConstant *pc = pConst->getEffect()->getConstant(EffectConstType::DirLightDir);
            if(pc) pc->bindValue(p->getDirection());
            
            pc = pConst->getEffect()->getConstant(EffectConstType::DirLightColor);
            if(pc) pc->bindValue(p->getColor());
            
            pConst->bindValue(true);
        }
        else
        {
            pConst->bindValue(false);
        }
    }

    void effectApplySpotLight(EffectConstant *pConst)
    {

    }

   
}//end namespace my3d