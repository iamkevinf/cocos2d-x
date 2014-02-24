#ifndef H_EFFECT_AUTO_CONST_IMP_H
#define H_EFFECT_AUTO_CONST_IMP_H

#include "EffectConstant.h"

namespace my3d
{
    
    typedef void (*EffectApplyFun)(EffectConstant *);
    
    class EffectConstantProxy : public EffectAutoConstant
    {
        EffectApplyFun m_fun;
    public:
        EffectConstantProxy(EffectApplyFun fun);
        ~EffectConstantProxy();
        
        virtual void apply(EffectConstant *pConst) override;
    };
    
    void effectApplyWorld(EffectConstant *pConst);
    void effectApplyViewProj(EffectConstant *pConst);
    void effectApplyWorldViewProj(EffectConstant *pConst);
    void effectApplyAmbient(EffectConstant *pConst);
    void effectApplyOmitLight(EffectConstant *pConst);
    void effectApplyDirLight(EffectConstant *pConst);
    void effectApplySpotLight(EffectConstant *pConst);

}//end namespace my3d


#endif //H_EFFECT_AUTO_CONST_IMP_H