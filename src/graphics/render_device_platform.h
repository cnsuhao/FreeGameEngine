//
//  render_device_platform.h
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#ifndef _orange__render_device_platform___
#define _orange__render_device_platform___

#include <SDL/include/SDL.h>

namespace ora
{
    class RenderDevicePlatform
    {
    public:
        RenderDevicePlatform();
        
        ~RenderDevicePlatform();
        
        
        SDL_Window*     window_;
        SDL_Renderer*   render_;
    };
    
} // end namespace ora


#endif /* defined(_orange__render_device_platform___) */
