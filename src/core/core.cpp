#include "core.h"

#include "util/log_tool.h"

#ifdef __APPLE__
#   include <TargetConditionals.h>
#endif

// Application need define this variable.
// eg. const char * compileTimeString = __DATE__ " " __TIME__;
extern const char * compileTimeString;

static uint32 version = 0;
static const char * versionString = "orange 0.1";

namespace ora
{
	uint32 getVersion()
	{
		return version;
	}

	const char* getVersionString()
	{
		return versionString;
	}

    const char * getCompileString()
    {
        return compileTimeString;
    }
    
    void checkStructAlign();
    
    void logEnvironment()
    {
        //print the delelopment
        ORA_LOG_INFO("engine version: %s", versionString);
        ORA_LOG_INFO("compile time: %s", compileTimeString);
        
#if defined(_DEBUG) || defined(DEBUG)
        ORA_LOG_INFO("Develop mode: debug");
#endif

#if defined(NDEBUG)
        ORA_LOG_INFO("Develop mode: release");
#endif

#if defined(_RELEASE)
        ORA_LOG_INFO("Develop mode: retail");
#endif
        
        //print the os type
#if defined(WIN32)
        ORA_LOG_INFO("OS platform: win32");
#elif defined(IOS)
        ORA_LOG_INFO("OS platform: ios");
#elif defined(ANDROID)
        ORA_LOG_INFO("OS platform: android");
#elif defined(APPLE)
        ORA_LOG_INFO("OS platform: APPLE");
#elif defined(__linux)
        ORA_LOG_INFO("OS platform: linux");
#elif defined(unix)
        ORA_LOG_INFO("OS platform: unix");
#else
        ORA_LOG_INFO("OS platform: unknown");
#endif

        // print the defined macro

#ifdef WIN32
        ORA_LOG_INFO("macro defined: WIN32");
#endif

#ifdef _WIN64
        ORA_LOG_INFO("macro defined: _WIN64");
#endif

#ifdef ANDROID
        ORA_LOG_INFO("macro defined: ANDROID");
#endif

#ifdef IOS
        ORA_LOG_INFO("macro defined: IOS");
#endif

#ifdef APPLE
        ORA_LOG_INFO("macro defined: APPLE");
#endif

#ifdef __APPLE__
        ORA_LOG_INFO("macro defined: __APPLE__");
        
#if TARGET_IPHONE_SIMULATOR
        ORA_LOG_INFO("macro defined: TARGET_IPHONE_SIMULATOR");
#endif

#if TARGET_OS_IPHONE
        // iOS device
        ORA_LOG_INFO("macro defined: TARGET_OS_IPHONE");
#endif

#if TARGET_OS_MAC
        // Other kinds of Mac OS
        ORA_LOG_INFO("macro defined: TARGET_OS_MAC");
#endif

#endif //__APPLE__


#ifdef __linux
        ORA_LOG_INFO("macro defined: __linux");
#endif

#ifdef __unix
        ORA_LOG_INFO("macro defined: __unix");
#endif

#ifdef __posix
        ORA_LOG_INFO("macro defined: __posix");
#endif

#ifdef unix
        ORA_LOG_INFO("macro defined: unix");
#endif

#ifdef __ppc__
        ORA_LOG_INFO("macro defined: __ppc__");
#endif

#ifdef __ppc64__
        ORA_LOG_INFO("macro defined: __ppc64__");
#endif

#ifdef __i386__
        ORA_LOG_INFO("macro defined: __i386__");
#endif

#ifdef __x86_64__
        ORA_LOG_INFO("macro defined: __x86_64__");
#endif

#ifdef __arm__
        ORA_LOG_INFO("macro defined: __arm__");
#endif

#ifdef __arm64__
        ORA_LOG_INFO("macro defined: __arm64__");
#endif

#ifdef __POWERPC__
        ORA_LOG_INFO("macro defined: __POWERPC__");
#endif

#ifdef __INTEL__
        ORA_LOG_INFO("macro defined: __INTEL__");
#endif

#ifdef __LITTLE_ENDIAN__
        ORA_LOG_INFO("macro defined: __LITTLE_ENDIAN__");
#endif

#ifdef __BIG_ENDIAN__
        ORA_LOG_INFO("macro defined: __BIG_ENDIAN__");
#endif


        // print data size
        
        ORA_LOG_DEBUG("size short : %d", sizeof(short));
        ORA_LOG_DEBUG("size int   : %d", sizeof(int));
        ORA_LOG_DEBUG("size long  : %d", sizeof(long));
        ORA_LOG_DEBUG("size size_t: %d", sizeof(size_t));
        ORA_LOG_DEBUG("size int32 : %d", sizeof(int32));
        ORA_LOG_DEBUG("size int64 : %d", sizeof(int64));
        ORA_LOG_DEBUG("size void* : %d", sizeof(void*));
        ORA_LOG_DEBUG("size float : %d", sizeof(float));
        ORA_LOG_DEBUG("size double: %d", sizeof(double));
        
        // check byte order
        uint32 tempValue = 0x12345678;
        char * tempPtr = (char*)&tempValue;
        if(tempPtr[0] == 0x78 && tempPtr[3] == 0x12)
            ORA_LOG_DEBUG("byte order: little endian.");
        else if(tempPtr[0] == 0x12 && tempPtr[3] == 0x78)
            ORA_LOG_ERROR("byte order: big endian.");
        else
            ORA_LOG_ERROR("byte order: unknown.");

        checkStructAlign();
    }

} //end namespace ora
