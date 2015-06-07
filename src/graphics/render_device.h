#ifndef H__RENDER_DEVICE_H
#define H__RENDER_DEVICE_H

#include "render_state.h"
#include "light_container.h"
#include "viewport.h"
#include "projection.h"
#include "util/object.h"
#include "render_target.h"

namespace ora
{
    class VertexBuffer;
    class IndexBuffer;
    class RenderDevicePlatform;


    namespace ClearState
    {
        const uint32 Color = 1 << 0;
        const uint32 Depth = 1 << 1;
        const uint32 Stencil = 1 << 2;
        const uint32 All = 0xffffffff;
    };

    struct RenderStackItem
    {        
        RenderTargetPtr renderTarget_;
        Viewport        viewport_;
        ProjCamera      camera_;
        Matrix          view_;
        Matrix          projection_;
        int             FBO_;
    };

    class RenderDevice : public Object
    {
    public:
        RenderDevice();
        ~RenderDevice();

        bool init();
        void fini();

        bool valid() const { return true; }

        void preMultiply( const Matrix& m );
        void postMultiply( const Matrix& m );
        void pushWorld();
        void pushWorld(const Matrix & world);
        void popWorld();
        void resetWorld();

        const Matrix & getWorld() const;
        void setWorld(const Matrix & world);

        void setView(const Matrix & view);
        const Matrix & getView() const;

        void setProjection(const Matrix & proj);
        const Matrix & getProjection() const;

        const Matrix & getInvView() const;
        const Matrix & getViewProjection();
        const Matrix & getWorldViewProjection() const;

        void setRenderState(RenderState state, bool enable);
        void setBlendFun(BlendFun src, BlendFun dst);
        void setCullFace(CullFace mode);
        void setFrontFace(bool isCW);

        void setDepthWritable(bool enable);
        void setDepthFun(CompareFun fun);

        void setScissor(int32 x, int32 y, uint32 width, uint32 height);

        void setStencilMask(uint32 mask);
        void setStencilFun(CompareFun fun, int32 ref, uint32 mask);
        void setStencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass);
        void setStencilOpSeparate(CullFace face, StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass);

        void setColorMask(uint32 color);
        void setShadeMode();

        void drawPrimitive(PrimitiveType pt, uint32 start, uint32 count);
        void drawIndexedPrimitive(PrimitiveType pt, uint32 start, uint32 count);

        VertexBuffer * getVertexBuffer();
        void setVertexBuffer(VertexBuffer *p);
        void unsetVertexBuffer(VertexBuffer *p);

        IndexBuffer * getIndexBuffer();
        void setIndexBuffer(IndexBuffer *p);
        void unsetIndexBuffer(IndexBuffer *p);

        void setLightContainer(LightContainerPtr container);
        LightContainerPtr getLightContainer();

        void setViewport(const Viewport & viewport);
        const Viewport & getViewPort(){ return viewport_; }

        void clear(uint32 flags, const Color& color, float depth, uint32 stencil);
        bool beginDraw();
        void endDraw();

        void onEvent(IEventSender *sender, uint32 eventType, VariantVector & args) override;

        uint32 frameTimestamp( ) const;
		bool frameDrawn( uint32& frame ) const;
		void nextFrame( );

        bool mixedVertexProcessing() const;
        float zoomFactor() const ;

        int maxVertexShaderConst() const;

        void setCamera(const ProjCamera & camera);
        ProjCamera & getCamera(){ return camera_; }
        const ProjCamera & getCamera() const { return camera_; }
        void updateProjection();
        
        bool pushRenderTarget(RenderTargetPtr rt);
        bool popRenderTarget();

    private:
        std::vector<Matrix>     matWorlds_;
        Matrix                  matView_;
        Matrix                  matProj_;
        mutable Matrix          matInvView_;
        mutable Matrix          matViewProj_;
        mutable Matrix          matWorldViewProj_;
        mutable uint32          dirty_;

        uint32                  currentFrame_;

        VertexBuffer*           vertexBuffer_;
        IndexBuffer*            indexBuffer_;
        Viewport                viewport_;
        ProjCamera              camera_;

        LightContainerPtr       lightContainer_;
        RenderDevicePlatform*   devImp_;
        
        std::vector<RenderStackItem> renderStack_;
    };

}//end namespace m3d


#endif //H__RENDER_DEVICE_H
