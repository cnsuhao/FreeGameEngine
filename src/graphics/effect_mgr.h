//
//  EffectManager.h
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__EffectManager__
#define __liborange__EffectManager__

#include "effect.h"
#include "base_res_mgr.h"

namespace ora
{

    class EffectMgr : public BaseResMgr<EffectMgr, Effect>
    {
    public:

        EffectMgr();
        ~EffectMgr();
    };

}

#endif /* defined(__liborange__EffectManager__) */
