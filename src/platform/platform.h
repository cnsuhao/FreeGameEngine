#ifndef H_ORANGE_PLATFORM_H
#define H_ORANGE_PLATFORM_H

#if defined(ANDROID)

#pragma message("platform android")

#include "platform/android/file_tool_imp.h"

#elif defined(IOS)

#pragma message("platform ios")
#include "platform/ios/file_tool_imp.h"

#elif defined(WIN32)

#pragma message("platform win32")
#include "platform/win32/file_tool_imp.h"

#else

#include "platform/unix/file_tool_imp.h"

#endif

#endif //H_ORANGE_PLATFORM_H
