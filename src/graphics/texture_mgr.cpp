//
//  texture_mgr.cpp
//  liborange
//
//  Created by zhoubao on 14-2-24.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "texture_mgr.h"
#include "util/log_tool.h"

IMPLEMENT_SINGLETON(ora::TextureMgr);


namespace ora
{

    TextureMgr::TextureMgr()
        : BaseResMgr<TextureMgr, Texture>("cache/resource/texture")
    {
    	ORA_STACK_TRACE;

        setCapacity(64);
    }

    TextureMgr::~TextureMgr()
    {
    	ORA_STACK_TRACE;
    }
    
#if defined(USE_PVRTC) || defined(USE_ETC)
    //////////////////////
    IMPLEMENT_SINGLETON(TextureMgr);
    
    TextureMgr::TextureMgr()
        : BaseResMgr<TextureMgr, CompressedTexture>("cache/resource/compressed_texture")
    {
        ORA_STACK_TRACE;
    }
    
    TextureMgr::~TextureMgr()
    {
        ORA_STACK_TRACE;
    }
#endif

}//end namespace ora
