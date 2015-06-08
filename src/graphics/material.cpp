#include "material.h"
#include "effect_constant.h"
#include "render_device.h"
#include "effect_mgr.h"
#include "texture_mgr.h"

#include "util/data_stream.h"
#include "util/log_tool.h"
#include "util/file_tool.h"
#include "util/section_factory.h"

namespace ora
{
    TextureWrap string2wrap(const std::string & wrap)
    {
        if(wrap == "clamp") return TextureWrap::Clamp;
        if(wrap == "mirror") return TextureWrap::Mirror;
        if(wrap == "repeat") return TextureWrap::Repeat;

        return TextureWrap::Clamp;
    }
    
    BlendFun string2blend(const std::string & blend)
    {
        if(blend == "zero") return BlendFun::Zero;
        if(blend == "one") return BlendFun::One;

        if(blend == "srcColor") return BlendFun::SrcColor;
        if(blend == "invSrcColor") return BlendFun::InvSrcColor;

        if(blend == "dstColor") return BlendFun::DstColor;
        if(blend == "invDstColor") return BlendFun::InvDstColor;

        if(blend == "srcAlpha") return BlendFun::SrcAlpha;
        if(blend == "invSrcAlpha") return BlendFun::InvSrcAlpha ;

        if(blend == "dstAlpha") return BlendFun::DstAlpha;
        if(blend == "invDstAlpha") return BlendFun::InvDstAlpha;

        if(blend == "constantColor") return BlendFun::ConstantColor;
        if(blend == "invConstantColor") return BlendFun::InvConstantColor;

        if(blend == "constantAlpha") return BlendFun::ConstantAlpha;
        if(blend == "inConstantAlpha") return BlendFun::InvConstantAlpha;
        
        return BlendFun::Zero;
    }

    namespace DefaultRS
    {
        const bool                  BlendEnabled = true;
        const BlendFun              SrcBlend = BlendFun::SrcAlpha;
        const BlendFun              DstBlend = BlendFun::InvSrcAlpha;

        const bool                  AlphaTestEnabled = false;
        const int                   AlphaReference = 0;
        
        const bool                  CullFaceEnabled = true;
        const CullFace              CullFaceMode = CullFace::Back;
        const bool                  FrontFaceCW = true;

        const bool                  DepthTestEnabled = true;
        const bool                  DepthWriteEnabled = true;
        const CompareFun            DepthTestFun = CompareFun::Less;
    }


    Material::Material()
        : blendEnabled_(DefaultRS::BlendEnabled)
        , srcBlend_(DefaultRS::SrcBlend)
        , dstBlend_(DefaultRS::DstBlend)
        , alphaTestEnabled_(DefaultRS::AlphaTestEnabled)
        , alphaReference_(DefaultRS::AlphaReference)
        , cullFaceEnabled_(DefaultRS::CullFaceEnabled)
        , cullFaceMode_(DefaultRS::CullFaceMode)
        , frontFaceCW_(DefaultRS::FrontFaceCW)
        , depthTestEnabled_(DefaultRS::DepthTestEnabled)
        , depthWriteEnabled_(DefaultRS::DepthWriteEnabled)
        , depthTestFun_(DefaultRS::DepthTestFun)
    {
    }


    Material::~Material()
    {
    }


    bool Material::load(SectionPtr pStream, const std::string & texturePath)
    {
        mtlColor_.ambient.set(0.7f, 0.7f, 0.7f, 1.0f);
        mtlColor_.diffuse.set(0.9f, 0.9f, 0.9f, 1.0f);
        mtlColor_.specular = mtlColor_.diffuse;
        mtlColor_.emissive.set(0, 0, 0, 1);

        //pStream->read(&mtlColor_);
        resouce_ = pStream->readString("identifier");

        std::string shaderFile = pStream->readString("fx");
        shaderFile = "shaders/" + shaderFile + ".glsl";
        effect_ = EffectMgr::instance()->get(shaderFile);

        size_t nState = pStream->readInt("state", 1);
        textureStages_.resize(nState);

        bool isCompressed = pStream->readBool("isCompressed", false);
        for(size_t i = 0; i < nState; ++i)
        {
            TextureStage & st = textureStages_[i];

			std::string textureName = "texture";
			if ( i > 0 )
			{
				std::ostringstream oss;
				oss << "texture" << i+1;
				textureName = oss.str();
			}

            std::string textureFile = pStream->readString(textureName);
            if(!textureFile.empty())
            {
                TexturePtr tex;

                //try the given path first.
                std::string name = texturePath + getFileName(textureFile);
                if(!textureFile.empty() && FileSystemMgr::instance()->isFileExist(name))
                {

                    if ( isCompressed )
                        tex = TextureMgr::instance()->get(name);
                    else
                        tex = TextureMgr::instance()->get(name);
                }
                
                //try default path.
                if(!tex)
                {
                    if ( isCompressed )
                        tex = TextureMgr::instance()->get(textureFile);
                    else
                        tex = TextureMgr::instance()->get(textureFile);
                }

                st.setTexture(tex);
            }

            std::string param = pStream->readString("uwrap");
            st.setUWrap(string2wrap(param));

            param = pStream->readString("vwrap");
            st.setVWrap(string2wrap(param));
        }
        
        std::string tmp;
        blendEnabled_ = pStream->readBool("blendEnabled", DefaultRS::BlendEnabled);
        tmp = pStream->readString("srcBlend");
        if(!tmp.empty()) srcBlend_ = string2blend(tmp);
        tmp = pStream->readString("dstBlend");
        if(!tmp.empty()) dstBlend_ = string2blend(tmp);
        
        alphaTestEnabled_ = pStream->readBool("alphaTestEnabled", DefaultRS::AlphaTestEnabled);
        alphaReference_ = pStream->readInt("alphaReference", DefaultRS::AlphaReference);
        cullFaceEnabled_ = pStream->readBool("cullFaceEnabled", DefaultRS::CullFaceEnabled);
        //cullFaceMode_ = pStream->readBool("cullFaceMode", DefaultRS::CullFaceMode);
        //frontFaceCW_ = pStream->readBool("frontFaceCW", DefaultRS::FrontFaceCW);
        depthTestEnabled_ = pStream->readBool("depthTestEnabled", DefaultRS::DepthTestEnabled);
        depthWriteEnabled_ = pStream->readBool("depthWriteEnabled", DefaultRS::DepthWriteEnabled);
        //depthTestFun_ = pStream->readBool("blendEnabled", DefaultRS::DepthTestFun);

        return true;
    }

    bool Material::loadFromFile(const std::string & resource, const std::string & key)
    {
        resouce_ = resource;

        SectionPtr root = SectionFactory::instance()->loadDS(resource);
        if(!root) return false;
        
        SectionPtr ptr = root->read(key);
        if(!ptr) return false;
        
        return load(ptr);
    }

    MaterialPtr Material::clone()
    {
        return new Material(*this);
    }

    bool Material::begin()
    {
        if(!effect_ || !effect_->begin()) return false;

        EffectConstant *pConst;

        for (size_t i = 0; i < textureStages_.size(); ++i)
        {
            TextureStage & st = textureStages_[i];

            std::stringstream ss;
            ss << EffectConstType::Texture << i;
            pConst = effect_->getConstant(ss.str());
            if(pConst)
            {
                st.applyParam();
                pConst->bindValue(st.getTexture());
            }
        }

        pConst = effect_->getConstant(EffectConstType::Material);
        if(pConst) pConst->bindValue(mtlColor_);

        //blend
        RenderDevice * pDevice = renderDev();
        if(DefaultRS::BlendEnabled != blendEnabled_)
            pDevice->setRenderState(RenderState::Blend, blendEnabled_);

        if(DefaultRS::SrcBlend != srcBlend_ || 
            DefaultRS::DstBlend != dstBlend_)
            pDevice->setBlendFun(srcBlend_, dstBlend_);

        //cull face
        if(DefaultRS::CullFaceEnabled != cullFaceEnabled_)
            pDevice->setRenderState(RenderState::CullFace, cullFaceEnabled_);

        if(DefaultRS::CullFaceMode != cullFaceMode_)
            pDevice->setCullFace(cullFaceMode_);

        if(DefaultRS::FrontFaceCW != frontFaceCW_)
            pDevice->setFrontFace(frontFaceCW_);

        //depth test
        if(DefaultRS::DepthTestEnabled != depthTestEnabled_)
            pDevice->setRenderState(RenderState::DepthTest, depthTestEnabled_);

        if(DefaultRS::DepthWriteEnabled != depthWriteEnabled_)
            pDevice->setDepthWritable(depthWriteEnabled_);

        if(DefaultRS::DepthTestFun != depthTestFun_)
            pDevice->setDepthFun(depthTestFun_);

        return true;
    }

    void Material::end()
    {
        effect_->end();

        RenderDevice * pDevice = renderDev();

        //blend
        if(DefaultRS::BlendEnabled != blendEnabled_)
            pDevice->setRenderState(RenderState::Blend, DefaultRS::BlendEnabled);

        if(DefaultRS::SrcBlend != srcBlend_ || 
            DefaultRS::DstBlend != dstBlend_)
            pDevice->setBlendFun(DefaultRS::SrcBlend, DefaultRS::DstBlend);

        //cull face
        if(DefaultRS::CullFaceEnabled != cullFaceEnabled_)
            pDevice->setRenderState(RenderState::CullFace, DefaultRS::CullFaceEnabled);

        if(DefaultRS::CullFaceMode != cullFaceMode_)
            pDevice->setCullFace(DefaultRS::CullFaceMode);

        if(DefaultRS::FrontFaceCW != frontFaceCW_)
            pDevice->setFrontFace(DefaultRS::FrontFaceCW);

        //depth test
        if(DefaultRS::DepthTestEnabled != depthTestEnabled_)
            pDevice->setRenderState(RenderState::DepthTest, DefaultRS::DepthTestEnabled);

        if(DefaultRS::DepthWriteEnabled != depthWriteEnabled_)
            pDevice->setDepthWritable(DefaultRS::DepthWriteEnabled);

        if(DefaultRS::DepthTestFun != depthTestFun_)
            pDevice->setDepthFun(DefaultRS::DepthTestFun);

    }

    void Material::setTextureStage(int i, const TextureStage & stage)
    {
        textureStages_[i] = stage;
    }

    void Material::addTextureStage(const TextureStage & stage)
    {
        textureStages_.push_back(stage);
    }

    void Material::clearTextureStage()
    {
        textureStages_.clear();
    }
    
    TextureStage & Material::getTextureStage(size_t i)
    {
        return textureStages_[i];
    }

    size_t Material::getNumTextureStage() const
    {
        return textureStages_.size();
    }

    const std::string & Material::getResouce() const
    {
        return resouce_;
    }

    void Material::setEffect(EffectPtr eff)
    {
        effect_ = eff;
    }

    EffectPtr Material::getEffect() const
    {
        return effect_;
    }

    void Material::setMaterialColor(const MaterialColor & color)
    {
        mtlColor_ = color;
    }

    void Material::setBlendEnable(bool enable)
    {
        blendEnabled_ = enable;
    }

    bool Material::getBlendEnable() const
    {
        return blendEnabled_;
    }

    void Material::setBlendFun(BlendFun src, BlendFun dst)
    {
        srcBlend_ = src;
        dstBlend_ = dst;
    }

    BlendFun Material::getSrcBlend() const
    {
        return srcBlend_;
    }

    BlendFun Material::getDstBlend() const
    {
        return dstBlend_;
    }

    
    void Material::setAlphaTestEnable(bool enable)
    {
        alphaTestEnabled_ = enable;
    }

    void Material::setAlphaReference(int reference)
    {
        alphaReference_ = reference;
    }

    void Material::setCullFaceEnable(bool enable)
    {
        cullFaceEnabled_ = enable;
    }

    bool Material::getCullFaceEnable() const
    {
        return cullFaceEnabled_;
    }


    void Material::setCullFaceMode(CullFace mode)
    {
        cullFaceMode_ = mode;
    }

    CullFace Material::getCullFaceMode() const
    {
        return cullFaceMode_;
    }

    void Material::setDepthTestEanble(bool enable)
    {
        depthTestEnabled_ = enable;
    }

    bool Material::getDepthTestEnable() const
    {
        return depthTestEnabled_;
    }

    void Material::setDepthWriteEnable(bool enable)
    {
        depthWriteEnabled_ = enable;
    }

    bool Material::getDepthWriteEnable() const
    {
        return depthWriteEnabled_;
    }

    void Material::setDepthTestFun(CompareFun fun)
    {
        depthTestFun_ = fun;
    }

    CompareFun Material::getDepthTestFun() const
    {
        return depthTestFun_;
    }

}//end namespace ora