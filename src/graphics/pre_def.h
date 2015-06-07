//
//  pre_def.h
//  liborange
//
//  Created by zhoubao on 14-2-27.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef liborange_PreDef_h
#define liborange_PreDef_h

#include "util/smartptr.h"

namespace ora
{
    typedef SmartPtr<class DataStream> DataStreamPtr;
    
    
    class RenderDevice;
    
    RenderDevice * renderDev();
    bool renderDevValid();
}

#endif
