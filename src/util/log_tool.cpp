//
//  log_tool.cpp
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#include "log_tool.h"
#include "string_tool.h"
#include "event_dispatch.h"
#include "event_def.h"
#include "cstdmf/string_utils.hpp"

#ifdef WIN32

#include <Windows.h>

#elif defined(ANDROID)

#include <android/log.h>
#include "cstdmf/dprintf.hpp"
#include "cstdmf/debug.hpp"
#include <mygui/MyGUIEngine/include/MyGUI_ILogListener.h>

namespace ora
{
    android_LogPriority logLvlOrangeToAndroid(int lvl)
    {
        if(lvl >= ORA_LOG_LVL_FATAL) return ANDROID_LOG_FATAL;
        if(lvl >= ORA_LOG_LVL_ERROR) return ANDROID_LOG_ERROR;
        if(lvl >= ORA_LOG_LVL_WARN) return ANDROID_LOG_WARN;
        if(lvl >= ORA_LOG_LVL_INFO) return ANDROID_LOG_INFO;
        if(lvl >= ORA_LOG_LVL_DEBUG) return ANDROID_LOG_DEBUG;
        return ANDROID_LOG_DEBUG;
    }

    android_LogPriority logLvlBWToAndroid(int lvl)
    {
        switch(lvl)
        {
        case MESSAGE_PRIORITY_DEBUG:
            return ANDROID_LOG_DEBUG;

        case MESSAGE_PRIORITY_INFO:
        case MESSAGE_PRIORITY_NOTICE:
        case MESSAGE_PRIORITY_SCRIPT:
            return ANDROID_LOG_INFO;

        case MESSAGE_PRIORITY_WARNING:
            return ANDROID_LOG_WARN;

        case MESSAGE_PRIORITY_ERROR:
            return ANDROID_LOG_ERROR;

        case MESSAGE_PRIORITY_CRITICAL:
        case MESSAGE_PRIORITY_ASSET:
            return ANDROID_LOG_FATAL;

        default:
            return ANDROID_LOG_DEFAULT;
        }
    }

    android_LogPriority logLvlMyGUIToAndroid(int lvl)
    {
        switch(lvl)
        {
        case MyGUI::LogLevel::Info:
            return ANDROID_LOG_DEBUG;

        case MyGUI::LogLevel::Warning:
            return ANDROID_LOG_WARN;

        case MyGUI::LogLevel::Error:
            return ANDROID_LOG_ERROR;

        case MyGUI::LogLevel::Critical:
            return ANDROID_LOG_FATAL;

        default:
            return ANDROID_LOG_DEFAULT;
        }
    }
}

void mygui_log_for_android(int lvl, const std::string& message, const char* file, int line)
{
    __android_log_write(ora::logLvlMyGUIToAndroid(lvl), "MyGUI", message.c_str());
}

#else

#include <iostream>

#endif

#include <cstdarg>

namespace ora
{
    namespace LogPrefix
    {
        const char * Trace = "Trace: ";
        const char * Debug = "DEBUG: ";
        const char * Info = "INFO : ";
        const char * Warn = "WARN : ";
        const char * Error = "ERROR: ";
        const char * Fatal = "FATAL: ";
        
    }

    const char * getLogPrefix(int lvl)
    {
        if (lvl >= ORA_LOG_LVL_FATAL) return LogPrefix::Fatal;
        if (lvl >= ORA_LOG_LVL_ERROR) return LogPrefix::Error;
        if (lvl >= ORA_LOG_LVL_WARN) return LogPrefix::Warn;
        if (lvl >= ORA_LOG_LVL_INFO) return LogPrefix::Info;
        if (lvl >= ORA_LOG_LVL_DEBUG) return LogPrefix::Debug;
        return LogPrefix::Trace;
    }

#if defined(ANDROID)
    struct AndroidCriticalMessageCallback : public CriticalMessageCallback
    {
        virtual void handleCritical( const char * msg )
        {
            __android_log_write(ANDROID_LOG_FATAL, "bigworld", msg);
        }
    };

    struct AndroidDebugMessageCallback : public DebugMessageCallback
    {
        virtual bool handleMessage(int componentPriority, int messagePriority, const char * format, va_list argPtr)
        {
            __android_log_vprint(logLvlBWToAndroid(messagePriority), "bigworld", format, argPtr);
            return true;
        }
    };

    void registerAndroidLogListener()
    {
        ORA_STACK_TRACE;

        static AndroidCriticalMessageCallback s_logCritical;
        static AndroidDebugMessageCallback s_logDebug;

        DebugFilter::instance().addMessageCallback(&s_logDebug);
        DebugFilter::instance().addCriticalCallback(&s_logCritical);
    }

#endif
    
    void ora_log(int lvl, const char * format, ...)
    {
        std::string msg;
        
        va_list va;
        va_start(va, format);
        
        stringFromVAList(msg, format, va);
        
        va_end(va);
        
        ora_log_directly(lvl, msg.c_str());
    }

    void ora_log_directly(int lvl, const char * msg)
    {
#ifdef WIN32
        std::string temp = getLogPrefix(lvl);
        temp += msg;
        temp += '\n';
        {
            std::wstring out;
            if(bw_utf8tow(temp, out))
                OutputDebugStringW(out.c_str());
            else
                OutputDebugStringA(temp.c_str());
        }
#elif defined(ANDROID)
        __android_log_write(logLvlOrangeToAndroid(lvl), "orange", msg);
#else
        std::cout << getLogPrefix(lvl) << msg << std::endl;
#endif

        if (GlobalSender::hasInstance())
        {
            VariantVector args;
            build_arguments(args, lvl, msg);
            GlobalSender::instance()->sendEvent(ET::Log, args);
        }

        if (lvl >= ORA_LOG_LVL_FATAL)
        {
            abort();
        }
    }
    
} // end namespace ora
