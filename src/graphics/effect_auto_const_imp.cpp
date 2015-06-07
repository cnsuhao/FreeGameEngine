#include "effect_auto_const_imp.h"
#include "render_device.h"
#include "effect.h"

Vector4 actionAlphaToken = Vector4(1.f, 1.f, 1.f, 1.f);

namespace ora
{
    
	float coldDownAlphaToken = 0.f;
	float progressReferenceToken = 1.f;
    
    EffectConstantProxy::EffectConstantProxy(EffectApplyFun fun)
        : fun_(fun)
    {
        ASSERT_2(fun, "EffectConstantFun - fun must not be null!");
    }

    EffectConstantProxy::~EffectConstantProxy()
    {

    }

    void EffectConstantProxy::apply(EffectConstant *pConst)
    {
        (*fun_)(pConst);
    }

    //////////////////////////////////////////////////////////////////

    void effectApplyWorld(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getWorld());
    }

    void effectApplyView(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getView());
    }
    
    void effectApplyProj(EffectConstant *pConst)
    {
        pConst->bindValue(renderDev()->getProjection());
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
        ASSERT_0();
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
                info.halfVector.normalise();

                EffectConstant *pc = nullptr;

                pc = pConst->getChild("color");
                if(pc) pc->bindValue(info.color);

                pc = pConst->getChild("halfVector");
                if(pc) pc->bindValue(info.halfVector);
            }
        }

        info.direction.w = lightEnable ? 1.0f : -1.0f; //-1 means disable

        EffectConstant *pc = pConst->getChild("direction");
        if(pc) pc->bindValue(info.direction);

    }

    void effectApplySpotLight(EffectConstant *pConst)
    {
        ASSERT_0();
    }
    
    void effectApplyAlpha(EffectConstant *pConst)
    {
        pConst->bindValue(actionAlphaToken);
    }
	
	void effectApplyColdDownAlpha(EffectConstant *pConst)
	{
		pConst->bindValue(coldDownAlphaToken);
	}

	void effectApplyProgress(EffectConstant *pConst)
	{
		pConst->bindValue(progressReferenceToken);
	}
    
    //////////////////////////////////////////////////////////////////
    void registerDefaultEffectConst()
    {
#define REG_EFFECT_CONST_FACTORY(TYPE, FACTORY) \
    EffectAutoConstant::set(TYPE, new EffectConstantProxy(FACTORY))
        
        REG_EFFECT_CONST_FACTORY(EffectConstType::World, effectApplyWorld);
        REG_EFFECT_CONST_FACTORY(EffectConstType::View, effectApplyView);
        REG_EFFECT_CONST_FACTORY(EffectConstType::Proj, effectApplyProj);
        REG_EFFECT_CONST_FACTORY(EffectConstType::ViewProj, effectApplyViewProj);
        REG_EFFECT_CONST_FACTORY(EffectConstType::WorldViewProj, effectApplyWorldViewProj);
        REG_EFFECT_CONST_FACTORY(EffectConstType::AmbientColor, effectApplyAmbient);
        REG_EFFECT_CONST_FACTORY(EffectConstType::OmitLight, effectApplyOmitLight);
        REG_EFFECT_CONST_FACTORY(EffectConstType::DirLight, effectApplyDirLight);
        REG_EFFECT_CONST_FACTORY(EffectConstType::SpotLight, effectApplySpotLight);
        REG_EFFECT_CONST_FACTORY(EffectConstType::ActionAlpha, effectApplyAlpha);
		REG_EFFECT_CONST_FACTORY(EffectConstType::ColdDownAlpha, effectApplyColdDownAlpha);
		REG_EFFECT_CONST_FACTORY(EffectConstType::ProgressReference, effectApplyProgress);
        
#undef REG_EFFECT_CONST_FACTORY
    }


}//end namespace ora