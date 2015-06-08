//
//  light_container.h
//  liborange
//
//  Created by zhoubao on 14-2-27.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__LightContainer__
#define __liborange__LightContainer__

#include "light_dir.h"

#include <vector>

namespace ora
{
    typedef std::vector<DirLightPtr> DirLightVector;

    class LightContainer : public IReferenceCount
    {
    public:
        LightContainer();
        ~LightContainer();

        void setAmbientColor(const Color & color);
        const Color & getAmbientColor() const
        {
            return ambient_;
        }

        void addDirLight(DirLightPtr light);
        void clearDirLight();
        DirLightVector & getDirLights()
        {
            return dirLights_;
        }

    private:
        Color ambient_;
        DirLightVector dirLights_;
    };

    typedef SmartPtr<LightContainer> LightContainerPtr;

}//end namespace ora

#endif /* defined(__liborange__LightContainer__) */
