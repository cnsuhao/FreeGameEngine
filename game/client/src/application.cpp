//
//  application.cpp
//  client
//
//  Created by zhoubao on 14-6-11.
//
//

#include "application.h"

#include "util/log_tool.h"
#include "util/event_def.h"
#include "util/watch_time.h"
#include "util/watcher.h"
#include "util/random.h"
#include "util/timer.h"

#include "graphics/graphics.h"
#include "core/world.h"
#include "core/mesh_node.h"
#include "core/font_node.h"
#include "script/script.h"

#include <sstream>
#include <iomanip>

#ifdef WIN32

#include "../proj.win32/resource.h"
extern "C" int SDL_ICON_RESOURCE;

#elif defined(ANDROID)

extern bool onClientSetup();

#endif

#define _WIDE_S(x) L ## x
#define WIDE_S(x) _WIDE_S(x)

const char * compileTimeString = __DATE__ " " __TIME__;
const wchar_t * compileTimeStringW = WIDE_S(__DATE__) L" " WIDE_S(__TIME__);


Application::Application()
{
}

Application::~Application()
{
    ORA_STACK_TRACE;
}

bool Application::onSetup()
{
    ORA_STACK_TRACE;
    
//    ORA_LOG_INFO("AppDocDir: %s", BWUtil::getAppDocDir().c_str());
//    ORA_LOG_INFO("AppHomeDir: %s", BWUtil::getAppHomeDir().c_str());
//    ORA_LOG_INFO("AppTmpDir: %s", BWUtil::getAppTmpDir().c_str());
//    ORA_LOG_INFO("AppResDir: %s", BWUtil::getAppResPath().c_str());

#ifdef WIN32
    SDL_ICON_RESOURCE = IDI_GLFW_ICON;
#elif defined(ANDROID)
    if(!onClientSetup()) return false;
#endif

    return true;
}

bool Application::onStart()
{
    ORA_STACK_TRACE;

	ora::FontNodeMgr::initInstance();
    ora::ScriptMgr::initInstance();
    
    if(!ora::ScriptMgr::instance()->init())
    {
        return false;
    }

    return true;
}

void Application::onStop()
{
    // release any reference to script object.
    ora::Timer::instance()->finiInstance();
    
    if(ora::ScriptMgr::hasInstance())
    {
        ora::ScriptMgr::instance()->fini();
        ora::ScriptMgr::finiInstance();
    }
    
	if (ora::FontNodeMgr::hasInstance())
	{
        ora::FontNodeMgr::finiInstance();
	}

    ORA_LOG_DEBUG("game stop.");
}

DEFINE_APPLICATION_MAIN(Application);
