//
//  light_container.cpp
//  liborange
//
//  Created by zhoubao on 14-2-27.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "light_container.h"

namespace ora
{
    LightContainer::LightContainer()
    {

    }
    LightContainer::~LightContainer()
    {

    }

    void LightContainer::setAmbientColor(const Color & color)
    {
        ambient_ = color;
    }

    void LightContainer::addDirLight(DirLightPtr light)
    {
        dirLights_.push_back(light);
    }

    void LightContainer::clearDirLight()
    {
        dirLights_.clear();
    }


}//end namespace ora