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
#include "compressed_texture.h"
#include "base_res_mgr.h"

namespace ora
{
    class TextureMgr : public BaseResMgr<TextureMgr, Texture>
    {
    public:
        TextureMgr();
        ~TextureMgr();
    };
    
#if defined(USE_PVRTC) || defined(USE_ETC)
    class CompressedTextureMgr : public BaseResMgr<CompressedTextureMgr, CompressedTexture>
    {
    public:
        CompressedTextureMgr();
        ~CompressedTextureMgr();
	};
#else
	typedef TextureMgr CompressedTextureMgr;
#endif


}//end namespace ora


#endif /* defined(__liborange__TextureMgr__) */
