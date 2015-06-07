//
//  main_def.h
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#ifndef liborange_main_def_h
#define liborange_main_def_h

#ifdef WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

// Define a platform-specific main function, which in turn executes the user-defined function

// MSVC debug mode: use memory leak reporting
#if defined(_MSC_VER) && defined(_DEBUG)

#define DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    /*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);*/ \
    return function(); \
}

// Other Win32 or minidumps disabled: just execute the function
#elif defined(WIN32)

#define DEFINE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    return function(); \
}

// Android or iOS: use SDL_main
#elif defined(ANDROID) || defined(IOS)

#define DEFINE_MAIN(function) \
extern "C" int SDL_main(int argc, char** argv); \
int SDL_main(int argc, char** argv) \
{ \
    return function(); \
}

// Linux or OS X: use main
#else

#define DEFINE_MAIN(function) \
int main(int argc, char** argv) \
{ \
    return function(); \
}

#endif


#endif
