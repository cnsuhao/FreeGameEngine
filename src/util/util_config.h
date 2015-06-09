//
//  util_config.h
//  liborange
//
//  Created by zhoubao on 14-4-14.
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __liborange__util_config__
#define __liborange__util_config__

#include <cstdlib>

#ifdef _WIN32

typedef signed char     int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef long long       int64;
typedef unsigned long long uint64;

#else

typedef int8_t      int8;
typedef uint8_t     uint8;
typedef int16_t     int16;
typedef uint16_t    uint16;
typedef int32_t     int32;
typedef uint32_t    uint32;
typedef int64_t     int64;
typedef uint64_t    uint64;

#endif

typedef uint32          uint;
//}


#define MAKE_UNCOPYABLE(CLASS)              \
private:                                    \
    CLASS(const CLASS &);                   \
    const CLASS & operator=(const CLASS &);

#define ORA_BREAK_IF( cont )    if ( cont ) break;

#define ORA_BREAK_NOT_IF(COND)  if(!(COND)) break;

// NOMINMAX makes sure that windef.h doesn't add macros min and max
#ifdef WIN32
#define NOMINMAX
#endif

#ifdef CODE_INLINE
#   define INLINE inline
#else
#   define INLINE
#endif

#endif /* defined(__liborange__util_config__) */
