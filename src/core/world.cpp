//
//  world_mgr.cpp
//  my3d_libs
//
//  Created by zhoubao on 14-3-31.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "world.h"
#include "graphics/render_device.h"

#include "util/watch_time.h"
#include "util/time_tool.h"
#include "util/timer.h"
#include "util/log_tool.h"
#include "util/event_dispatch.h"
#include "util/event_def.h"

#include <thread>

IMPLEMENT_SINGLETON(ora::World);

namespace ora
{
    static const float MinElapse = 1.0f / 60.0f;

    World::World()
        : pause_(0)
        , elapse_(0.0f)
        , timeScale_(1.0f)
        , fps_(0.0f)
        , totalFrame_(0)
		, lineMode_(0)
    {
        retain(); // sigleton object.
    }

    World::~World()
    {
    }

    void World::setRoot(SceneNodePtr root, bool cleanup)
    {
        if(root_ == root) return;
        if(root_)
        {
            root_->leaveWorld();
            if(cleanup)  root_->destroy();
        }

        root_ = root;
        if(root_)  root_->enterWorld();
    }

    bool World::init()
    {
        return true;
    }

    void World::fini()
    {
        setRoot(nullptr);
    }

    void World::update()
    {
        updateTime();
		updateTime_ms();

        sendEvent(ET::FrameBeg, NullArgument);
        if (!pause_)
        {
            doUpdate();
        }
		doRender();
        sendEvent(ET::FrameEnd, NullArgument);

        if (elapse_ < MinElapse)
        {
            float tsleep = (MinElapse - elapse_) * 1000.0f;
            std::this_thread::sleep_for(std::chrono::milliseconds(int(tsleep)));
        }
    }
    
	void World::updateTime_ms()
	{
        uint32 curTime = getTickTime_ms();
        
        static uint32 lastUpdateTime = curTime;
        elapse_ms_ = curTime - lastUpdateTime;
        lastUpdateTime = curTime;
	}

    void World::updateTime()
    {
        float curTime = getTickTime();
        
        static float lastUpdateTime = curTime;
        elapse_ = std::min(0.1f, curTime - lastUpdateTime);
        elapse_ = std::max(0.000001f, elapse_ * timeScale_);

        lastUpdateTime = curTime;

        static float lastFpsUpdateTime = curTime;
        static uint32 lastFpsUpdateFrame = 0;
        
        ++totalFrame_;
        
        if(curTime - lastFpsUpdateTime >= 1.0f)
        {
            fps_ = (totalFrame_ - lastFpsUpdateFrame) / (curTime - lastFpsUpdateTime);
            
            lastFpsUpdateTime = curTime;
            lastFpsUpdateFrame = totalFrame_;
        }
    }

	void World::doUpdate()
	{
        WATCH_TIME(world_tick);
        VariantVector args;
        build_arguments(args, elapse_, elapse_ms_);
        sendEvent(ET::TickBeg, args);
        
        Timer::instance()->tick();

        updateCamera();

        if (root_)
            root_->tick(elapse_);
        
        sendEvent(ET::TickEnd, args);
	}

	void World::doRender()
	{
#if defined(_DEBUG) && defined(_WIN32)
		GLenum mode = getLineMode() ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
#endif
		WATCH_TIME(world_draw);
        updateCamera();

        renderDev()->clear(ClearState::All, ora::Color(0xff000000), 1, 0);
		if (renderDev()->beginDraw())
		{
            sendEvent(ET::RenderBeg, NullArgument);
            
            if(root_) root_->draw();
            

            sendEvent(ET::RenderEnd, NullArgument);
            
			renderDev()->endDraw();
		}
	}

    
    void World::setCamera(CameraPtr pCamera)
    {
        camera_ = pCamera;

        updateCamera();
    }

    void World::updateCamera()
    {
        if( camera_ )
        {
            camera_->tick(elapse_);
            renderDev()->setView(camera_->getView());
        }
        else
        {
            renderDev()->setView(Matrix::identity);
        }
    }

    
    Vector3 World::screenPosToWorld(const Vector2 & pos) const
    {
        const Viewport & vp = renderDev()->getViewPort();
        float dx = pos.x / vp.width_;
        float dy = pos.y / vp.height_;

        const ProjCamera & camera = renderDev()->getCamera();

        float zNear = camera.getNear();
        float nearHeight = zNear * tan(camera.getFov() * 0.5f) * 2.0f;
        float nearWidth = camera.getAspect() * nearHeight;

        Vector3 ray((dx - 0.5f) * nearWidth, (0.5f - dy) * nearHeight, zNear);
        ray.normalise();
        renderDev()->getInvView().applyVector(ray, ray);
        ray.normalise();

        return ray;
    }

    Vector3 World::worldPosToProj(const Vector3 & pos) const
    {
        Vector4 vec4(pos, 1.0f);
        renderDev()->getViewProjection().applyPoint(vec4, vec4);

        vec4 *= (1.0f / vec4.w);
        return Vector3(vec4.x, vec4.y, vec4.z);
    }

    Vector2 World::worldPosToScreen(const Vector3 & pos) const
    {
        Vector3 p = worldPosToProj(pos);
        const Viewport & vp = renderDev()->getViewPort();

        return Vector2(
            (p.x + 1.0f) * 0.5f * vp.width_,
            (p.y + 1.0f) * 0.5f * vp.height_);
    }

    Vector2 World::getViewSize() const
    {
        const Viewport & vp = renderDev()->getViewPort();
        return Vector2(float(vp.width_), float(vp.height_));
    }

}//end namespace ora
