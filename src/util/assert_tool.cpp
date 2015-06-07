//
//  assert_tool.cpp
//  liborange
//
//  Created by zhoubao on 14-4-10.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef _RELEASE

#include "assert_tool.h"
#include "cstdmf/debug.hpp"

namespace ora
{
    bool doAssert(const char * exp,
                  const char * desc,
                  const char * file,
                  const char * func,
                  int line)
    {
        DebugMsgHelper().criticalMessage("ASSERT FAILED: %s\r\n%s\r\nFILE: %s(%d)\r\nFUNC: %s\r\n",
            exp, desc, file, line, func);
        return false;
    }
    
} // end namespace ora

#endif
