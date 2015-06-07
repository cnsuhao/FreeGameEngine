//
//  effect_constant.h
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__EffectConstant__
#define __liborange__EffectConstant__

#include "base.h"
#include "texture.h"

#include <map>

namespace ora
{
    class Effect;
    class Color;
    struct MaterialColor;

    //自动常量类型
    namespace EffectConstType
    {
        const std::string World = "u_matWorld";
        const std::string View = "u_matView";
        const std::string Proj = "u_matProj";
        const std::string ViewProj = "u_matViewProj";
        const std::string WorldViewProj = "u_matWorldViewProj";
        const std::string AmbientColor = "u_ambient";
        const std::string OmitLight = "u_omitLight";
        const std::string DirLight = "u_dirLight";
        const std::string SpotLight = "u_spotLight";
        const std::string Texture = "u_texture";
        const std::string Texture0 = "u_texture0";
        const std::string Texture1 = "u_texture1";
        const std::string Texture2 = "u_texture2";
        const std::string Texture3 = "u_texture3";
        const std::string Texture4 = "u_texture4";
        const std::string Texture5 = "u_texture5";
        const std::string Texture6 = "u_texture6";
        const std::string Texture7 = "u_texture7";
        const std::string Material = "u_material";
        const std::string ActionAlpha = "u_actionAlpha";
        const std::string ColdDownAlpha = "u_coldDownAlpha";
        const std::string ProgressReference = "u_progressReference";
    }

    /**
     * Represents a uniform variable within an effect.
     */
    class EffectConstant
    {
        friend class Effect;

    public:

        const std::string& getName() const;

        const GLenum getType() const;

        Effect* getEffect() const;

        EffectConstant *getChild(const std::string & name, bool createIfMiss = false);
        EffectConstant *getChildren(const std::string & name, bool createIfMiss = false);

        void bindValue(float value);

        void bindValue(const float* values, uint32 count = 1);

        void bindValue(int value);

        void bindValue(const int* values, uint32 count = 1);

        void bindValue(const Matrix& value);

        void bindValue(const Matrix* values, uint32 count, bool transposed);

        void bindValue(const Vector2& value);

        void bindValue(const Vector2* values, uint32 count = 1);

        void bindValue(const Vector3& value);

        void bindValue(const Vector3* values, uint32 count = 1);

        void bindValue(const Vector4& value);

        void bindValue(const Vector4* values, uint32 count = 1);

        void bindValue(const Color & color);

        void bindValue(const MaterialColor & color);

        void bindValue(const TexturePtr texture);

    private:

        EffectConstant(const std::string & name);
        ~EffectConstant();

        std::string name_;
        GLint       location_;
        GLenum      type_;
        uint32      index_;
        Effect*     pEffect_;
        //如果是纹理，这里需要持有它的一个引用技术，防止纹理提前析构而引起崩溃
        TexturePtr  texture_;
        std::map<std::string, EffectConstant*> children_;
    };

    class EffectAutoConstant
    {
    public:

        virtual void apply(EffectConstant *pConst) = 0;

        static EffectAutoConstant * get(const std::string & name);
        static void set(const std::string & name, EffectAutoConstant *autoConst);
        static void fini();

    protected:
        EffectAutoConstant();
        virtual ~EffectAutoConstant();

        static std::map<std::string, EffectAutoConstant*> s_autoConstMap;
    };
}

#endif /* defined(__liborange__EffectConstant__) */
