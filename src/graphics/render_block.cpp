#include "render_block.h"

namespace ora
{
    RenderBlock::RenderBlock()
    {
    }

    RenderBlock::~RenderBlock()
    {
    }

    void RenderBlock::init(const RenderBlock* pDefault)
    {
        pDefault_ = pDefault;

        //copy the render state from default block

        blendEabled_ = pDefault_->blendEabled_;
        cullEable_ = pDefault_->cullEable_;
        depthTestEabled_ = pDefault_->depthTestEabled_;
        stencilEnabled_ = pDefault_->stencilEnabled_;
    }

    void RenderBlock::enableRS(RenderState rs, bool enable)
    {
        enable ? glEnable(GLenum(rs)) : glDisable(GLenum(rs));
    }

    void RenderBlock::bind()
    {
        if(pDefault_->blendEabled_ != blendEabled_)
            enableRS(RenderState::Blend, blendEabled_);

        if(pDefault_->cullEable_ != cullEable_)
            enableRS(RenderState::CullFace, cullEable_);

        if(pDefault_->depthTestEabled_ != depthTestEabled_)
            enableRS(RenderState::DepthTest, depthTestEabled_);

        if(pDefault_->stencilEnabled_ != stencilEnabled_)
            enableRS(RenderState::StencilTest, stencilEnabled_);
    }

    void RenderBlock::unbind()
    {
        if(pDefault_->blendEabled_ != blendEabled_)
            enableRS(RenderState::Blend, pDefault_->blendEabled_);

        if(pDefault_->cullEable_ != cullEable_)
            enableRS(RenderState::CullFace, pDefault_->cullEable_);

        if(pDefault_->depthTestEabled_ != depthTestEabled_)
            enableRS(RenderState::DepthTest, pDefault_->depthTestEabled_);

        if(pDefault_->stencilEnabled_ != stencilEnabled_)
            enableRS(RenderState::StencilTest, pDefault_->stencilEnabled_);
    }

}// end namespace ora
