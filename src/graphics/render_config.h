//
//  Renderconfig.h
//  my3d_libs
//
//  Created by zhoubao on 14-3-31.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef _my3d_libs__RenderConfig___
#define _my3d_libs__RenderConfig___

#include "render_state.h"
#include "util/singleton.h"

namespace ora
{
    class RenderConfig :
        public Singleton<RenderConfig>
    {
    public:
        RenderConfig();
        ~RenderConfig();

        bool load(const std::string & fname);
        void save(const std::string & fname);

        TextureQuality getTextureQuality() const { return textureQuality_; }

    private:
        TextureQuality      textureQuality_;
    };
}

#endif /* defined(_my3d_libs__RenderConfig___) */
