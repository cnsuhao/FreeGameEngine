//
//  assert_tool.h
//  liborange
//
//  Created by zhoubao on 14-4-10.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__assert_tool__
#define __liborange__assert_tool__

#ifndef _RELEASE

namespace ora
{
    bool doAssert(const char * exp,
                  const char * desc,
                  const char * file,
                  const char * func,
                  int line);
}

#define M_ASSERT(COND, DESC) \
do{\
    if(!(COND)) ora::doAssert(#COND, DESC, __FILE__, __FUNCTION__, __LINE__);\
}while(0)


#define IF_NOT_ASSERT(COND) \
if(!(COND) && ora::doAssert(#COND, "", __FILE__, __FUNCTION__, __LINE__))

#else //_RELEASE

#define M_ASSERT(COND, DESC)
#define IF_NOT_ASSERT(COND) if(!(COND))

#endif //_RELEASE

#define ASSERT_0()              M_ASSERT(false, "Force Asset.")
#define ASSERT_1(COND)          M_ASSERT(COND, "")
#define ASSERT_2(COND, DESC)    M_ASSERT(COND, DESC)

#endif /* defined(__liborange__assert_tool__) */
