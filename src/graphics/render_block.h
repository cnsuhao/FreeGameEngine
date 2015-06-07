#ifndef H_RENDER_BLOCK_H
#define H_RENDER_BLOCK_H

#include "render_state.h"

namespace ora
{
    class RenderBlock : public IReferenceCount
    {
    public:
        RenderBlock();
        ~RenderBlock();

        void init(const RenderBlock* pDefault);

        void bind();
        void unbind();

    private:
        void enableRS(RenderState rs, bool enalbe);

        bool                blendEabled_;
        BlendFun            srcBlend_;
        BlendFun            dstBlend_;
        
        bool                cullEable_;
        CullFace            cullFace_;
        bool                frontFaceCW_; //front face is in clockwise.

        bool                depthTestEabled_;
        CompareFun          depthTestFun_;

        bool                stencilEnabled_;
        uint32              stencilMask_;
        uint32              stencilReference_;
        CompareFun          stencilFun_;
        StencilOp           stencilOPSFail_;
        StencilOp           stencilOPDFail_;
        StencilOp           stencilOPDPass_;

        uint32              colorWriteMask_;

        const RenderBlock*  pDefault_;
    };

}// end namespace ora

#endif //H_RENDER_BLOCK_H