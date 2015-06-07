#ifndef H__MATERIAL_H
#define H__MATERIAL_H

#include "color.h"
#include "texture.h"
#include "effect.h"

#include "util/section.h"

namespace ora
{

    typedef SmartPtr<class Material> MaterialPtr;

    class Material : public IReferenceCount
    {
    public:
        Material();
        ~Material();

        bool load(SectionPtr pStream, const std::string & texturePath = "");
        bool loadFromFile(const std::string & resource, const std::string & key);

        MaterialPtr clone();

        bool begin();
        void end();

        const std::string & getResouce() const;

        void setEffect(EffectPtr eff);
        EffectPtr getEffect() const;

        void setMaterialColor(const MaterialColor & color);

        void setTextureStage(int i, const TextureStage & stage);
        void addTextureStage(const TextureStage & stage);
        TextureStage & getTextureStage(size_t i);
        size_t getNumTextureStage() const;
        void clearTextureStage();

        void setBlendEnable(bool enable);
        bool getBlendEnable() const;

        void setBlendFun(BlendFun src, BlendFun dst);
        BlendFun getSrcBlend() const;
        BlendFun getDstBlend() const;

        void setAlphaTestEnable(bool enable);
        void setAlphaReference(int reference);

        void setCullFaceEnable(bool enable);
        bool getCullFaceEnable() const;

        void setCullFaceMode(CullFace mode);
        CullFace getCullFaceMode() const;

        void setDepthTestEanble(bool enable);
        bool getDepthTestEnable() const;

        void setDepthWriteEnable(bool enable);
        bool getDepthWriteEnable() const;

        void setDepthTestFun(CompareFun fun);
        CompareFun getDepthTestFun() const;

    private:
        std::string         resouce_;

        MaterialColor       mtlColor_;

        EffectPtr           effect_;

        bool                blendEnabled_;
        BlendFun            srcBlend_;
        BlendFun            dstBlend_;

        bool                alphaTestEnabled_;
        int                 alphaReference_;
        
        bool                cullFaceEnabled_;
        CullFace            cullFaceMode_;
        bool                frontFaceCW_; //front face is in clockwise.

        bool                depthTestEnabled_;
        bool                depthWriteEnabled_;
        CompareFun          depthTestFun_;

        std::vector<TextureStage> textureStages_;
    };


}//end namespace ora

#endif //H__MATERIAL_H