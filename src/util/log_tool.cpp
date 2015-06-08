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

#ifdef WIN32
#include <Windows.h>
#elif defined(ANDROID)
#include <android/log.h>

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
}

#else
#include <iostream>
#endif

#include <cstdarg>

namespace ora
{
    namespace LogPrefix
    {
        const std::string Trace = "Trace: ";
        const std::string Debug = "DEBUG: ";
        const std::string Info  = "INFO : ";
        const std::string Warn  = "WARN : ";
        const std::string Error = "ERROR: ";
        const std::string Fatal = "FATAL: ";
    }
    
    const std::string & getLogPrefix(int lvl)
    {
        if(lvl >= ORA_LOG_LVL_FATAL) return LogPrefix::Fatal;
        if(lvl >= ORA_LOG_LVL_ERROR) return LogPrefix::Error;
        if(lvl >= ORA_LOG_LVL_WARN) return LogPrefix::Warn;
        if(lvl >= ORA_LOG_LVL_INFO) return LogPrefix::Info;
        if(lvl >= ORA_LOG_LVL_DEBUG) return LogPrefix::Debug;
        return LogPrefix::Trace;
    }
    
    
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