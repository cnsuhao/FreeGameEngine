#ifndef H_EFFECT_AUTO_CONST_IMP_H
#define H_EFFECT_AUTO_CONST_IMP_H

#include "effect_constant.h"

namespace ora
{
    /** call this function before use Effect.*/
    void registerDefaultEffectConst();
    
    
    typedef void (*EffectApplyFun)(EffectConstant *);

    class EffectConstantProxy : public EffectAutoConstant
    {
        EffectApplyFun fun_;
    public:
        EffectConstantProxy(EffectApplyFun fun);
        ~EffectConstantProxy();

        virtual void apply(EffectConstant *pConst) override;
    };

    void effectApplyWorld(EffectConstant *pConst);
    void effectApplyView(EffectConstant *pConst);
    void effectApplyProj(EffectConstant *pConst);
    void effectApplyViewProj(EffectConstant *pConst);
    void effectApplyWorldViewProj(EffectConstant *pConst);
    void effectApplyAmbient(EffectConstant *pConst);
    void effectApplyOmitLight(EffectConstant *pConst);
    void effectApplyDirLight(EffectConstant *pConst);
    void effectApplySpotLight(EffectConstant *pConst);
    
}//end namespace ora


#endif //H_EFFECT_AUTO_CONST_IMP_H