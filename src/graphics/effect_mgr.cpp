//
//  EffectManager.cpp
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "effect_mgr.h"
#include "util/log_tool.h"

IMPLEMENT_SINGLETON(ora::EffectMgr);

namespace ora
{
    EffectMgr::EffectMgr()
        : BaseResMgr<EffectMgr, Effect>("cache/resource/effect")
    {
    	ORA_STACK_TRACE;
    }
    EffectMgr::~EffectMgr()
    {
    	ORA_STACK_TRACE;
    }
}