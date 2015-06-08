//
//  texture_mgr.h
//  liborange
//
//  Created by zhoubao on 14-2-24.
//  Copyright (c) 2014zhoubao. All rights reserved.
//

#ifndef __liborange__TextureMgr__
#define __liborange__TextureMgr__


#include "texture.h"
#include "base_res_mgr.h"

namespace ora
{
    class TextureMgr : public BaseResMgr<TextureMgr, Texture>
    {
    public:
        TextureMgr();
        ~TextureMgr();
    };
    
}//end namespace ora


#endif /* defined(__liborange__TextureMgr__) */
