//
//  engine.cpp
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#include "engine.h"

#include "util/log_tool.h"
#include "util/event_def.h"
#include "util/object_linker.h"
#include "util/timer.h"
#include "util/section_factory.h"
#include "util/watch_time.h"
#include "util/watcher.h"
#include "util/time_tool.h"

#include "platform/platform.h"
#include "graphics/render_device.h"

#include "core.h"
#include "world.h"

#include <SDL/include/SDL.h>


namespace ora
{
    int g_ref_counter = 0;
    
    extern int g_effect_counter;
    extern int g_texture_counter;
    extern int g_vb_counter;
    extern int g_ib_counter;

#ifdef ANDROID
    extern void registerAndroidLogListener();
#endif

    SectionPtr g_sysConfig;
    
    static RenderDevice * s_pRenderDev = nullptr;
    
    RenderDevice * renderDev()
    {
        ASSERT_2(s_pRenderDev != nullptr, "please call 'init' first!");
        return s_pRenderDev;
    }
    
    bool renderDevValid()
    {
        return s_pRenderDev != nullptr;
    }

    
    IMPLEMENT_SINGLETON(Engine);
    Engine::Engine()
		: exit_(false)
    {
        ORA_STACK_TRACE;
    }
    
    Engine::~Engine()
    {
        ORA_STACK_TRACE;
    }
    
    bool Engine::preInit()
    {
        ORA_STACK_TRACE;
        
        logEnvironment();

        //initialize singleton instance
        GlobalSender::initInstance();
#ifndef _RELEASE
        WatchTime::initInstance();
        WatcherMgr::initInstance();
#endif
        return true;
    }
    
    bool Engine::init()
    {
        ORA_STACK_TRACE;

        //initialize singleton instance
        SectionFactory::initInstance();
        Timer::initInstance();

        //initialize the config system
        g_sysConfig = SectionFactory::loadDS("config/system.cfg");
        if (!g_sysConfig)
        {
            ORA_LOG_ERROR("Failed to load system config!");
            return false;
        }
        
        //initialize log system
        logfile_ = new LogFile();
        if(!logfile_->init("game.log"))
        {
            ORA_LOG_WARN("Failed to init LogFile!");
        }
        
        
        //initialize render system
        s_pRenderDev = new RenderDevice();
        if (!s_pRenderDev->init())
            return false;
        
        
        //initialize world manager
        World::initInstance();
        
        if(!World::instance()->init())
            return false;
        
        ADD_WATCHER("counter/effect", g_effect_counter);
        ADD_WATCHER("counter/texture", g_texture_counter);
        ADD_WATCHER("counter/buffer_vb", g_vb_counter);
        ADD_WATCHER("counter/buffer_ib", g_ib_counter);
        ADD_WATCHER("counter/ref", g_ref_counter);
        ADD_WATCHER("time/ticktime", getTickTime);

        subscribeEvent(ET::Exit);
        return true;
    };
    
    void Engine::mainLoop()
    {
        ORA_STACK_TRACE;
        
        SDL_Event event;
        VariantVector argList;
        while(!exit_)
        {
            if(SDL_PollEvent(&event))
            {
                //ORA_LOG_DEBUG("SDL_PollEvent %u", event.type);

                switch (event.type)
                {
                    case SDL_KEYDOWN:
                    case SDL_KEYUP:
                        build_arguments(argList, event.key);
                        sendEvent(ET::Keyboard, argList);
                        break;
                        
#if defined(IOS) || defined(ANDROID)
                    case SDL_FINGERDOWN:
                    case SDL_FINGERMOTION:
                    case SDL_FINGERUP:
                        build_arguments(argList, event.tfinger);
                        sendEvent(ET::Touch, argList);
                        break;
#else
                    case SDL_MOUSEMOTION:
                        build_arguments(argList, event.motion);
                        sendEvent(ET::MouseMove, argList);
                        break;
                        
                    case SDL_MOUSEBUTTONDOWN:
                    case SDL_MOUSEBUTTONUP:
                        build_arguments(argList, event.button);
                        sendEvent(ET::MouseButton, argList);
                        break;
                        
                    case SDL_MOUSEWHEEL:
                        build_arguments(argList, event.wheel);
                        sendEvent(ET::MouseWheel, argList);
                        break;
#endif
                    case SDL_TEXTEDITING:
                        build_arguments(argList, event.edit);
                        sendEvent(ET::TextEditing, argList);
                        break;
                        
                    case SDL_TEXTINPUT:
                        build_arguments(argList, event.text);
                        sendEvent(ET::TextInput, argList);
                        break;
                   
                    case SDL_WINDOWEVENT:
                        build_arguments(argList, event.window);
                        sendEvent(ET::Window, argList);
                        break;
                        
                    default:
                        break;
                }
                
                if (event.type == SDL_QUIT ||
                    event.type == SDL_APP_TERMINATING)
                {
					ORA_LOG_DEBUG("Receive quit event.");
                    break;
                }
            }
            else
            {
                World::instance()->update();
            }
        }
        
        sendEvent(ET::Destroy, NullArgument);

        if (World::hasInstance())
        {
            World::instance()->fini();
            World::finiInstance();
        }
    }
    
    void Engine::onEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        if(eventType == ET::Exit)
        {
            quit();
        }
    }
    
    void Engine::fini()
    {
        ORA_STACK_TRACE;

        if (s_pRenderDev != nullptr)
        {
            s_pRenderDev->fini();
            delete s_pRenderDev;
            s_pRenderDev = nullptr;
        }
        
        if (logfile_)
        {
            logfile_->fini();
            logfile_ = nullptr;
        }

        g_sysConfig = nullptr;
        
        //release singleton instance
        SectionFactory::finiInstance();
        Timer::finiInstance();

#ifndef _RELEASE
        WatchTime::finiInstance();
        WatcherMgr::finiInstance();
#endif //#ifndef _RELEASE
        
#if 0   //以下内容，需要常驻内存，不需要释放
        
        //GlobalSender::initInstance();
        
#endif
    }
    
} // end namespace ora

