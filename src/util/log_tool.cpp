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
        
        const std::string & getLogPrefix(int lvl)
        {
            if(lvl >= ORA_LOG_LVL_FATAL) return Fatal;
            if(lvl >= ORA_LOG_LVL_ERROR) return Error;
            if(lvl >= ORA_LOG_LVL_WARN) return Warn;
            if(lvl >= ORA_LOG_LVL_INFO) return Info;
            if(lvl >= ORA_LOG_LVL_DEBUG) return Debug;
            return Trace;
        }
    }
    
    
    void ora_log(int lvl, const char * format, ...)
    {
        std::string msg;
        
        va_list va;
        va_start(va, format);
        
        stringFromVAList(msg, format, va);
        
        va_end(va);
        
        const std::string & prefix = LogPrefix::getLogPrefix(lvl);
        msg.insert(0, prefix);
        
#ifdef WIN32
        OutputDebugStringA((msg + "\r").c_str());
#elif defined(ANDROID)
        __android_log_print(logLvlOrangeToAndroid(lvl), "orange", "%s", msg.c_str());
#else
        std::cout << msg << std::endl;
#endif
        
        if(GlobalSender::hasInstance())
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