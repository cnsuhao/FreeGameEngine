//
//  render_config.cpp
//  my3d_libs
//
//  Created by zhoubao on 14-3-31.
//  Copyright (c) 2014  jjgame. All rights reserved.
//

#include "render_config.h"

namespace ora
{
    IMPLEMENT_SINGLETON(RenderConfig);

	RenderConfig::RenderConfig()
        : textureQuality_(TextureQuality::ThreeLinear)
	{

	}

	RenderConfig::~RenderConfig()
	{

	}

	bool RenderConfig::load(const std::string & fname)
	{
		return true;
	}

	void RenderConfig::save(const std::string & fname)
	{
	}

}// end namesapce my3d