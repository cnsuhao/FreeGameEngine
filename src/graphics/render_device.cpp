
#include "render_device.h"

#include "render_config.h"
#include "render_device_platform.h"
#include "render_helper.h"
#include "render_resource.h"

#include "vertex_buffer.h"
#include "vertex_declaration.h"

#include "texture_mgr.h"
#include "mesh_mgr.h"
#include "effect_mgr.h"

#include "effect_auto_const_imp.h"
#include "shader_parser.h"

#include "uibatch.h"

#include "util/assert_tool.h"
#include "util/log_tool.h"
#include "util/data_stream.h"
#include "util/watch_time.h"
#include "util/event_def.h"

#include "core/core.h"

namespace ora
{
    namespace DirtyFlag
    {
        const uint32 InvView = 1 << 0;
        const uint32 ViewProj = 1 << 1;
        const uint32 WorldViewProj = 1 << 2;
    }


    RenderDevice::RenderDevice()
        : dirty_(0)
        , vertexBuffer_(nullptr)
        , indexBuffer_(nullptr)
        , currentFrame_(0)
        , viewport_(0, 0, 640, 480)
    {
        matWorlds_.push_back(Matrix::identity);
        matInvView_ = matView_ = Matrix::identity;
        matWorldViewProj_ = matViewProj_ = matProj_ = Matrix::identity;

        devImp_ = new RenderDevicePlatform();
    }

    RenderDevice::~RenderDevice()
    {
    }

    bool RenderDevice::init()
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_NOPARACHUTE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        int multiSample = g_sysConfig->readBool("app/multiSample");
        if (multiSample > 1)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSample);
        }
        else
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
        }

        //SDL_SetHint(SDL_HINT_ORIENTATIONS, orientations_.CString());

        std::string title = g_sysConfig->readString("app/title", "orange");
#ifndef _RELEASE
        do
        {
            std::ostringstream ss;
            ss << "(" << getVersionString() << " " << getCompileString() << ")";
            title += ss.str();
        } while (0);
#endif

        int x = 0, y = 0, w = 0, h = 0;

        uint32 flag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(0, &mode);

#if defined(IOS) || defined(ANDROID)
        flag |= SDL_WINDOW_FULLSCREEN;
        w = mode.w;
        h = mode.h;
#else
        flag |= SDL_WINDOW_RESIZABLE;

        w = g_sysConfig->readInt("app/width", 640);
        h = g_sysConfig->readInt("app/height", 480);

        x = (mode.w - w) >> 1;
        y = (mode.h - h) >> 1;
#endif
        devImp_->window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flag);
        if (!devImp_->window_)
        {
            ORA_LOG_ERROR("Failed to create window: %s", SDL_GetError());
            return false;
        }

        viewport_.x_ = viewport_.y_ = 0;
        SDL_GetWindowSize(devImp_->window_, &viewport_.width_, &viewport_.height_);
        ORA_LOG_INFO("Window created. w=%d, h=%d", viewport_.width_, viewport_.height_);

        bool vsync = g_sysConfig->readBool("app/vsync", true);
        SDL_GL_SetSwapInterval(vsync ? 1 : 0);


        devImp_->render_ = SDL_CreateRenderer(devImp_->window_, -1, 0);
        if (!devImp_->render_)
        {
            ORA_LOG_ERROR("Failed to create context: %s", SDL_GetError());
            return false;
        }


        // If OpenGL extensions not yet initialized, initialize now
#if defined(WIN32) && !defined(GL_ES_VERSION_2_0)

        ORA_LOG_DEBUG("call glewInit.");
        
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            ORA_LOG_ERROR("Cannot initialize OpenGL");
            return false;
        }

        if (!GLEW_VERSION_2_0)
        {
            ORA_LOG_ERROR("OpenGL 2.0 is required");
            return false;
        }

        if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
        {
            ORA_LOG_ERROR("EXT_framebuffer_object and EXT_packed_depth_stencil OpenGL extensions are required");
            return false;
        }

        glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
        glEnable(GL_POINT_SPRITE);

#endif
        //setup default render state
        setRenderState(RenderState::DepthTest, true);
        setRenderState(RenderState::ScissorTest, false);
        setRenderState(RenderState::StencilTest, false);
        setRenderState(RenderState::CullFace, true);
        setCullFace(CullFace::Back);
        setFrontFace(true);
        
        setRenderState(RenderState::Blend, true);
        setBlendFun(BlendFun::SrcAlpha, BlendFun::InvSrcAlpha);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
#if 1 //test
        GLint maxTexSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
        ORA_LOG_DEBUG("GL_ES max texture size is %d", maxTexSize);
      
        GLfloat pointSizeRange[2];
        glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pointSizeRange);
        ORA_LOG_DEBUG("GL_ES point size range [%f, %f]", pointSizeRange[0], pointSizeRange[1]);
#endif

        //initilalize resource singleton
        RenderConfig::initInstance();
        TextureMgr::initInstance();
#if defined(USE_PVRTC) || defined(USE_ETC)
        CompressedTextureMgr::initInstance();
#endif
        EffectMgr::initInstance();
        MeshMgr::initInstance();
        UIBatch::initInstance();
        VertexDeclMgr::initInstance();
        ShaderParser::initInstance();
        RenderHelper::initInstance();
        ResourceMonitor::initInstance();


        ResourceMonitor::instance()->deviceCreate();
        registerDefaultEffectConst();
        
        VertexDeclMgr::instance()->init();
        
        if (!UIBatch::instance()->init())
            return false;
        
        subscribeEvent(ET::Window);

        return true;
    }


    void RenderDevice::fini()
    {
        if(ResourceMonitor::hasInstance())
            ResourceMonitor::instance()->deviceClose();
        ResourceMonitor::finiInstance();

        RenderHelper::finiInstance();
        UIBatch::finiInstance();

        MeshMgr::finiInstance();
        EffectMgr::finiInstance();
        ShaderParser::finiInstance();
        VertexDeclMgr::finiInstance();
        TextureMgr::finiInstance();
#if defined(IOS)
        CompressedTextureMgr::finiInstance();
#endif
        RenderConfig::finiInstance();
        
        EffectAutoConstant::fini();
        
        delete devImp_;
        devImp_ = nullptr;

        SDL_Quit();
    }

    void RenderDevice::preMultiply( const Matrix& m )
    {
	    matWorlds_.back().preMultiply( m );
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::postMultiply( const Matrix& m )
    {
	    matWorlds_.back().postMultiply( m );
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::pushWorld()
    {
        matWorlds_.push_back(matWorlds_.back());
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::pushWorld(const Matrix & world)
    {
        matWorlds_.push_back(world);
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::popWorld()
    {
        matWorlds_.pop_back();
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::resetWorld()
    {
        matWorlds_.clear();
        pushWorld(Matrix::identity);
    }

    const Matrix & RenderDevice::getWorld() const
    {
        return matWorlds_.back();
    }

    void RenderDevice::setWorld(const Matrix & world)
    {
        matWorlds_.back() = world;
        dirty_ |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::setView(const Matrix & view)
    {
        matView_ = view;
        dirty_ |= DirtyFlag::InvView | DirtyFlag::ViewProj | DirtyFlag::WorldViewProj;
    }

    const Matrix & RenderDevice::getView() const
    {
        return matView_;
    }

    void RenderDevice::setProjection(const Matrix & proj)
    {
        matProj_ = proj;
        dirty_ |= DirtyFlag::ViewProj | DirtyFlag::WorldViewProj;
    }

    const Matrix & RenderDevice::getProjection() const
    {
        return matProj_;
    }

    const Matrix & RenderDevice::getInvView() const
    {
        if (dirty_ & DirtyFlag::InvView)
        {
            matInvView_.invert(matView_);
            dirty_ &= ~DirtyFlag::InvView;
        }
        return matInvView_;
    }

    const Matrix & RenderDevice::getViewProjection()
    {
        if (dirty_ & DirtyFlag::ViewProj)
        {
            matViewProj_ = matView_;
            matViewProj_.postMultiply(matProj_);

            dirty_ &= ~DirtyFlag::ViewProj;
        }
        return matViewProj_;
    }

    const Matrix & RenderDevice::getWorldViewProjection() const
    {
        if (dirty_ & DirtyFlag::WorldViewProj)
        {
            matWorldViewProj_ = matWorlds_.back();
            matWorldViewProj_.postMultiply(matView_);
            matWorldViewProj_.postMultiply(matProj_);

            dirty_ &= ~DirtyFlag::WorldViewProj;
        }
        return matWorldViewProj_;
    }

    void RenderDevice::setRenderState(RenderState state, bool enable)
    {
        if (enable) glEnable(GLenum(state));
        else glDisable(GLenum(state));
    }

    void RenderDevice::setBlendFun(BlendFun src, BlendFun dst)
    {
        glBlendFunc(GLenum(src), GLenum(dst));
    }

    void RenderDevice::setCullFace(CullFace mode)
    {
        glCullFace(GLenum(mode));
    }

    void RenderDevice::setFrontFace(bool isCW)
    {
        glFrontFace(isCW ? GL_CW : GL_CCW);
    }

    void RenderDevice::setDepthWritable(bool enable)
    {
        glDepthMask(GLboolean(enable));
    }

    void RenderDevice::setDepthFun(CompareFun fun)
    {
        glDepthFunc(GLenum(fun));
    }

    void RenderDevice::setScissor(int32 x, int32 y, uint32 width, uint32 height)
    {
        glScissor(x, y, width, height);
    }

    void RenderDevice::setStencilMask(uint32 mask)
    {
        glStencilMask(mask);
    }

    void RenderDevice::setStencilFun(CompareFun fun, int32 ref, uint32 mask)
    {
        glStencilFunc(GLenum(fun), ref, mask);
    }

    void RenderDevice::setStencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass)
    {
        glStencilOp(
            GLenum(stencilFail),
            GLenum(depthFail),
            GLenum(depthPass)
        );
    }

    void RenderDevice::setStencilOpSeparate(CullFace face, StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass)
    {
        glStencilOpSeparate(
            GLenum(face),
            GLenum(stencilFail),
            GLenum(depthFail),
            GLenum(depthPass)
        );
    }

    void RenderDevice::setColorMask(uint32 color)
    {
        GLboolean r = color & ColorMask::R ? GL_TRUE : GL_FALSE;
        GLboolean g = color & ColorMask::G ? GL_TRUE : GL_FALSE;
        GLboolean b = color & ColorMask::B ? GL_TRUE : GL_FALSE;
        GLboolean a = color & ColorMask::A ? GL_TRUE : GL_FALSE;
        glColorMask(r, g, b, a);
    }

    void RenderDevice::setShadeMode()
    {
        //glShadeModel();
    }

    void RenderDevice::drawPrimitive(PrimitiveType pt, uint32 start, uint32 count)
    {
#ifndef _RELEASE
        ASSERT_2(vertexBuffer_, "Please bind the index and vertex buffer first!");
        WATCH_TIME(drawcall);

        if(pt == PrimitiveType::TriangleList)
        {
            WATCH_NUMBER(drawTriangles, count / 3.0f);
        }
        else if(pt == PrimitiveType::TriangleStrip)
        {
            WATCH_NUMBER(drawTriangles, (count - 1) / 2.0f);
        }
        else if(pt == PrimitiveType::TriangleFan)
        {
            WATCH_NUMBER(drawTriangles, count - 2.0f);
        }
#endif
        
        GL_ASSERT( glDrawArrays(GLenum(pt), start, count) );
    }

    void RenderDevice::drawIndexedPrimitive(PrimitiveType pt, uint32 start,  uint32 count)
    {
#ifndef _RELEASE
        ASSERT_2(indexBuffer_ && vertexBuffer_, "Please bind the index and vertex buffer first!");
        WATCH_TIME(drawcall);

        if(pt == PrimitiveType::TriangleList)
        {
            WATCH_NUMBER(drawTriangles, count / 3.0f);
        }
        else if(pt == PrimitiveType::TriangleStrip)
        {
            WATCH_NUMBER(drawTriangles, (count - 1) / 2.0f);
        }
        else if(pt == PrimitiveType::TriangleFan)
        {
            WATCH_NUMBER(drawTriangles, count - 2.0f);
        }
#endif

        IndexType itype = indexBuffer_->getIndexType();
        size_t isize = indexBuffer_->stride();
        GL_ASSERT( glDrawElements(GLenum(pt), count, GLenum(itype), (GLvoid*)(start * isize)) );
    }

    VertexBuffer * RenderDevice::getVertexBuffer()
    {
        return vertexBuffer_;
    }

    void RenderDevice::setVertexBuffer(VertexBuffer *p)
    {
        vertexBuffer_ = p;
    }

    void RenderDevice::unsetVertexBuffer(VertexBuffer *p)
    {
        if(vertexBuffer_ == p) vertexBuffer_ = nullptr;
    }

    IndexBuffer * RenderDevice::getIndexBuffer()
    {
        return indexBuffer_;
    }

    void RenderDevice::setIndexBuffer(IndexBuffer *p)
    {
        indexBuffer_ = p;
    }

    void RenderDevice::unsetIndexBuffer(IndexBuffer *p)
    {
        if(indexBuffer_ == p) indexBuffer_ = nullptr;
    }

    void RenderDevice::setLightContainer(LightContainerPtr container)
    {
        lightContainer_ = container;
    }

    LightContainerPtr RenderDevice::getLightContainer()
    {
        return lightContainer_;
    }

    void RenderDevice::setViewport(const Viewport & viewport)
    {
        if (viewport_ == viewport)
            return;

        viewport_.x_ = viewport.x_;
        viewport_.y_ = viewport.y_;
        viewport_.width_ = viewport.width_;
        viewport_.height_ = viewport.height_;

        glViewport(viewport_.x_, viewport_.y_,
            viewport_.width_, viewport_.height_);
    }


    void RenderDevice::clear(uint32 flags, const Color& color, float depth, uint32 stencil)
    {
#ifdef GL_ES_VERSION_2_0
        flags &= ~ClearState::Stencil;
#endif

        uint32 glFlags = 0;
        if (flags & ClearState::Color)
        {
            glFlags |= GL_COLOR_BUFFER_BIT;
            glClearColor(color.r, color.g, color.b, color.a);
        }
        if (flags & ClearState::Depth)
        {
            glFlags |= GL_DEPTH_BUFFER_BIT;
            glClearDepth(depth);
        }
        if (flags & ClearState::Stencil)
        {
            glFlags |= GL_STENCIL_BUFFER_BIT;
            glClearStencil(stencil);
        }

        glClear(glFlags);
    }

    bool RenderDevice::beginDraw()
    {
        updateProjection();
        return true;
    }

    void RenderDevice::endDraw()
    {
        SDL_GL_SwapWindow(devImp_->window_);

        nextFrame();
    }

    void RenderDevice::onEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        if (eventType == ET::Window)
        {
            SDL_WindowEvent eve;
            if (parse_arguments(args, &eve) && eve.event == SDL_WINDOWEVENT_RESIZED)
            {
                Viewport port(0, 0, eve.data1, eve.data2);
                setViewport(port);
                ORA_LOG_INFO("window resize to (%d, %d)", port.width_, port.height_);
            }
        }
    }

    uint32 RenderDevice::frameTimestamp( ) const
    {
        return currentFrame_;
    }

	bool RenderDevice::frameDrawn( uint32& frame ) const
    {
        bool ret = frame == currentFrame_;
        frame = currentFrame_;
        return ret;
    }

	void RenderDevice::nextFrame( )
    {
        ++currentFrame_;
    }

    bool RenderDevice::mixedVertexProcessing() const
    {
        return false;
    }

    float RenderDevice::zoomFactor() const 
    {
        return 1.0f;
    }

    int RenderDevice::maxVertexShaderConst() const
    {
        GLint n;
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &n);
        return n;
    }

    void RenderDevice::setCamera(const ProjCamera & camera)
    {
        camera_ = camera;
    }

    void RenderDevice::updateProjection()
    {
        setProjection(camera_.getMatrix());
    }

    bool RenderDevice::pushRenderTarget(RenderTargetPtr rt)
    {
        RenderStackItem item;
        item.view_ = matView_;
        item.projection_ = matProj_;
        item.viewport_ = viewport_;
        item.camera_ = camera_;
        item.renderTarget_ = rt;
        
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &item.FBO_);
        
        // apply the new FBO
        glBindFramebuffer(GL_FRAMEBUFFER, item.renderTarget_->getFBO());
        
        renderStack_.push_back(item);
        return true;
    }
    
    bool RenderDevice::popRenderTarget()
    {
        if(renderStack_.empty())
        {
            ORA_LOG_ERROR("renderStack is empty.");
            return false;
        }
        
        RenderStackItem item = renderStack_.back();
        renderStack_.pop_back();
        
        setView(item.view_);
        setProjection(item.projection_);
        setViewport(item.viewport_);
        setCamera(item.camera_);
        
        // restore the old FBO
        glBindFramebuffer(GL_FRAMEBUFFER, item.FBO_);
        
        return true;
    }
   
}//end namespace ora
