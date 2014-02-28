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
        bool lightEnable = false;
        DirLightInfo info;
        
        LightContainerPtr lights =  renderDev()->getLightContainer();
        if(lights)
        {
            DirLightVector & dirLights = lights->getDirLights();
            if(dirLights.size() > 0)
            {
                lightEnable = true;
                dirLights.front()->getEffectData(info);
                info.halfVector.set(1, 0, 1);
                info.halfVector.normalize();
                
                EffectConstant *pc = nullptr;
                
                pc = pConst->getChild("color");
                assert(pc);
                pc->bindValue(info.color);
                
                pc = pConst->getChild("halfVector");
                assert(pc);
                pc->bindValue(info.halfVector);
            }
        }
        
        info.direction.w = lightEnable ? 1.0f : -1.0f; //-1 means disable
        
        EffectConstant *pc = pConst->getChild("direction");
        assert(pc);
        pc->bindValue(info.direction);
        
    }

    void effectApplySpotLight(EffectConstant *pConst)
    {

    }

   
}//end namespace my3d