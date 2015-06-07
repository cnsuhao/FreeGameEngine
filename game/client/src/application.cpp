//
//  application.cpp
//  client
//
//  Created by zhoubao on 14-6-11.
//
//

#include "model/pch.hpp"
#include "application.h"

#include "util/log_tool.h"
#include "util/event_def.h"
#include "util/watch_time.h"
#include "util/watcher.h"
#include "util/random.h"
#include "util/timer.h"
#include "network/HttpClient.h"

#include "gui/gui_platform.h"
#include "graphics/graphics.h"
#include "core/world.h"
#include "core/mesh_node.h"
#include "core/font_node.h"

#include "script/script.h"
#include "script/root_scene.h"

#include <sstream>
#include <iomanip>

#include "cstdmf/pch.hpp"
#include "cstdmf/cstdmf.hpp"

#include "resmgr/pch.hpp"
#include "moo/render_context.hpp"
#include "romp/script_math.hpp"
#include "model/model.hpp"
#include "particle/particle_system_manager.hpp"


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

namespace Moo
{
    RenderContext* g_RC;
}

Application::Application()
    : myguiPlatform_(nullptr)
    , mygui_(nullptr)
{
    BW_GUARD;

    ORA_STACK_TRACE;
    pStdmf_ = new CStdMf();
    pBwresource_ = new BWResource();
    pRenderContext_ = new Moo::RenderContext();

    Moo::g_RC = pRenderContext_;

#ifndef _RELEASE
    debugWatch_ = nullptr;
    watchInterval_ = 10;
#endif
}

Application::~Application()
{
    BW_GUARD;
    delete pRenderContext_;
    delete pBwresource_;
    delete pStdmf_;

    ORA_STACK_TRACE;
}

bool Application::onSetup()
{
    BW_GUARD;
    ORA_STACK_TRACE;
    
    ORA_LOG_INFO("AppDocDir: %s", BWUtil::getAppDocDir().c_str());
    ORA_LOG_INFO("AppHomeDir: %s", BWUtil::getAppHomeDir().c_str());
    ORA_LOG_INFO("AppTmpDir: %s", BWUtil::getAppTmpDir().c_str());
    ORA_LOG_INFO("AppResDir: %s", BWUtil::getAppResPath().c_str());

#ifdef WIN32
    SDL_ICON_RESOURCE = IDI_GLFW_ICON;
#elif defined(ANDROID)
    if(!onClientSetup()) return false;
#endif

    if (!BWResource::init("", false))
    {
        ORA_LOG_ERROR("BWResource::init failed.");
        return false;
    }

    BWResource::addPath(BWUtil::getAppDocDir(), 0);
    return true;
}

bool Application::onStart()
{
    BW_GUARD;
    ORA_STACK_TRACE;

    myguiPlatform_ = new MyGUI::OrangePlatform();
    myguiPlatform_->initialise(BWUtil::getAppDocDir() + "mygui.log");
    
    mygui_ = new MyGUI::Gui();
    mygui_->initialise();

    MyGUI::IntSize designSize;
    designSize.width = ora::g_sysConfig->readInt("app/designWidth");
    designSize.height = ora::g_sysConfig->readInt("app/designHeight");
    mygui_->setDesignSize(designSize);
    
    ora::HttpClient::initInstance();
    if (!ora::HttpClient::instance()->init())
    {
        ORA_LOG_ERROR("Failed to init http!");
        return false;
    }
	
    MyGUI::PointerManager::getInstance().setVisible(false);

    subscribeEvent(ora::ET::FrameEnd);

#ifndef _RELEASE
    loadDebugForm();
#endif

    pRenderContext_->init();
    if (!pRenderContext_->createDevice())
        return false;

    ParticleSystemManager::init();

	ora::FontNodeMgr::initInstance();

    ora::ScriptMgr::initInstance();
    if (!ora::ScriptMgr::instance()->init())
    {
        ORA_LOG_ERROR("Failed to init script!");
        return false;
    }
    return true;
}

void Application::onStop()
{
    BW_GUARD;

    // release any reference to script object.
    ora::Timer::instance()->finiInstance();

    if (ora::HttpClient::hasInstance())
    {
        ora::HttpClient::instance()->fini();
        ora::HttpClient::finiInstance();
    }
    
	if (ora::FontNodeMgr::hasInstance())
	{
        ora::FontNodeMgr::finiInstance();
	}

    if (mygui_)
    {
        mygui_->shutdown();
        delete mygui_;
    }
    
    if (myguiPlatform_)
    {
        myguiPlatform_->shutdown();
        delete myguiPlatform_;
    }

    if (ora::ScriptMgr::hasInstance())
    {
        ora::ScriptMgr::instance()->fini();
        ora::ScriptMgr::finiInstance();
    }

    Model::fini();
    ParticleSystemManager::fini();
    if (pRenderContext_ != nullptr)
    {
        pRenderContext_->fini();
    }

    ORA_LOG_DEBUG("game stop.");
}

void Application::onEvent(IEventSender * sender, uint32 eventType, ora::VariantVector & args)
{
    if (eventType == ora::ET::FrameEnd)
    {
        float elapse = ora::World::instance()->getElapse();
        ProviderStore::tick(elapse);
        ora::ScriptMgr::instance()->tick(elapse);

#ifndef _RELEASE
        uint32 frame = ora::World::instance()->getTotalFrame();
        if ((watchInterval_ > 0) && (frame % watchInterval_ == 0) )
        {
            updateDebugInfo();
        }
#endif
    }
}

#ifndef _RELEASE
bool Application::loadDebugForm()
{
    MyGUI::LayoutManager::getInstancePtr()->loadLayout("layout/debug_watch.layout");
    debugWatch_ = MyGUI::Gui::getInstancePtr()->findWidgetByPathT("debugWatch");
    if (debugWatch_)
    {
        MyGUI::Widget *btnDebug = MyGUI::Gui::getInstancePtr()->findWidgetByPathT("btnDebugWatch");
        if (btnDebug != nullptr)
        {
            btnDebug->eventMouseButtonClick += MyGUI::newDelegate(this, &Application::onBtnDebugWatch);
        }

        MyGUI::EditBox *edtFrame = (MyGUI::EditBox*) debugWatch_->findWidgetByPath("edtFrame");
        if (edtFrame != nullptr)
        {
            std::ostringstream ss;
            ss << watchInterval_;
            edtFrame->setCaption(ss.str());
            edtFrame->eventEditTextChange += MyGUI::newDelegate(this, &Application::onEdtTextChange);
        }

        return true;
    }
    return false;
}

void Application::onBtnDebugWatch(MyGUI::Widget *sender)
{
    BW_GUARD;

    RootScene * scene = (RootScene*) ora::World::instance()->getRoot();
    scene->testRendertarget();
    
    //ASSERT_0();

    debugWatch_->setVisible(!debugWatch_->getVisible());
}

void Application::onEdtTextChange(MyGUI::EditBox *sender)
{
    watchInterval_ = atoi(sender->getCaption().asUTF8_c_str());
}

void Application::updateDebugInfo()
{
    do
    {
        ORA_BREAK_NOT_IF(debugWatch_ && debugWatch_->getVisible());
        
        MyGUI::TextBox * lblText = (MyGUI::TextBox *)debugWatch_->findWidgetByPath("lblText");
        ORA_BREAK_NOT_IF(lblText);
        
        std::ostringstream ss;
        ss << "name\t|count\t|average\t|min\t|max\t|total" << std::endl
        << std::fixed << std::setprecision(7);
        
        const ora::WatchTime::Pool & pool = ora::WatchTime::instance()->getPool();
        for (auto pair : pool)
        {
            ss << std::setw(20) << std::left << pair.first << "\t"
            << pair.second.getCounter() << "\t"
            << pair.second.getAverage() << "\t"
            << pair.second.getMinElapse() << "\t"
            << pair.second.getMaxElapse() << "\t"
            << pair.second.getAccumulate() << "\t"
            << std::endl;
        }
        
        ora::Watcher::root()->print(ss);
        
        lblText->setCaption(ss.str());
        
    }while(0);
    
    ora::WatchTime::instance()->clear();
}
#endif //_RELEASE

// ¶¨Òåmainº¯Êý
DEFINE_APPLICATION_MAIN(Application);
