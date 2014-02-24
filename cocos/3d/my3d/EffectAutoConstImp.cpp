#include "EffectAutoConstImp.h"
#include "M3DRenderDevice.h"

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
    }

    void effectApplyOmitLight(EffectConstant *pConst)
    {

    }

    void effectApplyDirLight(EffectConstant *pConst)
    {

    }

    void effectApplySpotLight(EffectConstant *pConst)
    {

    }

   
}//end namespace my3d