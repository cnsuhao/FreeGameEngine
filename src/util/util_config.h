//
//  util_config.h
//  liborange
//
//  Created by zhoubao on 14-4-14.
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __liborange__util_config__
#define __liborange__util_config__

#include "cstdmf/stdmf.hpp"

#define MAKE_UNCOPYABLE(CLASS)              \
private:                                    \
    CLASS(const CLASS &);                   \
    const CLASS & operator=(const CLASS &);

#define ORA_BREAK_IF( cont )    if ( cont ) break;

#define ORA_BREAK_NOT_IF(COND)  if(!(COND)) break;

#endif /* defined(__liborange__util_config__) */
