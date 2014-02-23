#ifndef H_EFFECT_AUTO_CONST_IMP_H
#define H_EFFECT_AUTO_CONST_IMP_H

#include "EffectConstant.h"

namespace my3d
{
    class EffectWorldConst : public EffectAutoConstant
    {
    public:
        EffectWorldConst();
        ~EffectWorldConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectViewProjConst : public EffectAutoConstant
    {
    public:
        EffectViewProjConst();
        ~EffectViewProjConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectWorldViewProjConst : public EffectAutoConstant
    {
    public:
        EffectWorldViewProjConst();
        ~EffectWorldViewProjConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectAmbientConst : public EffectAutoConstant
    {
    public:
        EffectAmbientConst();
        ~EffectAmbientConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectOmitLightConst : public EffectAutoConstant
    {
    public:
        EffectOmitLightConst();
        ~EffectOmitLightConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectDirLightConst : public EffectAutoConstant
    {
    public:
        EffectDirLightConst();
        ~EffectDirLightConst();

        virtual void apply(EffectConstant *pConst) override;

    };

    class EffectSpotLightConst : public EffectAutoConstant
    {
    public:
        EffectSpotLightConst();
        ~EffectSpotLightConst();

        virtual void apply(EffectConstant *pConst) override;

    };

}//end namespace my3d


#endif //H_EFFECT_AUTO_CONST_IMP_H