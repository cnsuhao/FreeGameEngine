#include "light_dir.h"

namespace ora
{

    DirLight::DirLight()
    {
    }


    DirLight::~DirLight()
    {
    }

    void DirLight::getEffectData(DirLightInfo & info)
    {
        info.direction.set(direction_.x, direction_.y, direction_.z, 1.0f);
        info.color = color_;
    }

}//end namespace ora