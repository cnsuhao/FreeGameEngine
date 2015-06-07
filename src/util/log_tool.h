//
//  log_tool.h
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#ifndef __liborange__log_tool__
#define __liborange__log_tool__

#define ORA_LOG_LVL_TRACE 1
#define ORA_LOG_LVL_DEBUG 10
#define ORA_LOG_LVL_INFO  20
#define ORA_LOG_LVL_WARN  30
#define ORA_LOG_LVL_ERROR 40
#define ORA_LOG_LVL_FATAL 50


#ifndef ORA_LOG_LVL
#   ifdef _RELEASE
#       define ORA_LOG_LVL ORA_LOG_LVL_INFO
#   else
#       define ORA_LOG_LVL ORA_LOG_LVL_TRACE
#   endif
#endif

namespace ora
{
    const char * getLogPrefix(int lvl);

    //fatal级别的log，会使程序终止
    void ora_log(int lvl, const char * format, ...);
    void ora_log_directly(int lvl, const char * msg);
    
} // end namespace ora

#if ORA_LOG_LVL_TRACE >= ORA_LOG_LVL
#   define ORA_LOG_TRACE(...) ::ora::ora_log(ORA_LOG_LVL_TRACE, __VA_ARGS__)
#   define ORA_STACK_TRACE ORA_LOG_TRACE("%s() LINE:%d FILE:%s ", __FUNCTION__, __LINE__, __FILE__)
#else
#   define ORA_LOG_TRACE(...)
#   define ORA_STACK_TRACE
#endif

#if ORA_LOG_LVL_DEBUG >= ORA_LOG_LVL
#   define ORA_LOG_DEBUG(...) ::ora::ora_log(ORA_LOG_LVL_DEBUG, __VA_ARGS__)
#else
#   define ORA_LOG_DEBUG(...)
#endif

#if ORA_LOG_LVL_INFO >= ORA_LOG_LVL
#   define ORA_LOG_INFO(...) ::ora::ora_log(ORA_LOG_LVL_INFO, __VA_ARGS__)
#else
#   define ORA_LOG_INFO(...)
#endif

#if ORA_LOG_LVL_WARN >= ORA_LOG_LVL
#   define ORA_LOG_WARN(...) ::ora::ora_log(ORA_LOG_LVL_WARN, __VA_ARGS__)
#else
#   define ORA_LOG_WARN(...)
#endif

#if ORA_LOG_LVL_ERROR >= ORA_LOG_LVL
#   define ORA_LOG_ERROR(...) ::ora::ora_log(ORA_LOG_LVL_ERROR, __VA_ARGS__)
#else
#   define ORA_LOG_ERROR(...)
#endif

#define ORA_LOG_FATAL(...) ::ora::ora_log(ORA_LOG_LVL_FATAL, __VA_ARGS__)

#endif /* defined(__liborange__log_tool__) */

