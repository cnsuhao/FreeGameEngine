//
//  effect.h
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__Effect__
#define __liborange__Effect__

#include "base.h"
#include "vertex_declaration.h"
#include "render_resource.h"

namespace ora
{
    class EffectConstant;
    class EffectAutoConstant;

    typedef SmartPtr<class ShaderCode> ShaderCodePtr;

    class Effect : public IResource
    {
        MAKE_UNCOPYABLE(Effect);
    public:

        Effect(const std::string & resource="");
        virtual ~Effect();
        void onDeviceClose();

        bool isValid() const;
        bool load(const std::string & resouce);
        bool load(ShaderCodePtr code, const std::string & macros);

        EffectConstant * getConstant(const std::string & name);

        bool begin();
        void end();

        const std::string & getResouce() const{ return resouce_; }
    
        static Effect * getActiveEffect();

    private:
        void doLoading();

        bool linkProgram();
    
        void parseUniforms();
        void parseAttributes(ShaderCodePtr code);

        void bindUniforms();

        void bindAttributes();
        void unbindAttributes();

    private:
        std::string     resouce_;
        ProgramHandle   program_;
        EffectConstant* pConstRoot_;
        int             attributes_[VertexUsageMax];

        typedef std::vector<std::pair<EffectAutoConstant*, EffectConstant*>> Properties;
        Properties      autoConsts_;

        struct EffectLoadingInfo*    pLoadingInfo_;

        static Effect * s_pActiveEffect;
    };

    typedef SmartPtr<Effect> EffectPtr;

}//end namespace ora

#endif /* defined(__liborange__Effect__) */
