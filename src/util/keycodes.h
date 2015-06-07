//
//  keycodes.h
//  orange
//
//  Created by zhoubao on 14-6-12.
//
//

#ifndef __orange__keycodes__
#define __orange__keycodes__

#include <SDL/include/SDL.h>
#include <mygui/MyGUIEngine/include/MyGUI.h>

namespace ora
{
    namespace KeyCode
    {
        void init();
        
        MyGUI::KeyCode::Enum sdl2mygui(SDL_Scancode key);
        SDL_Scancode mygui2sdl(MyGUI::KeyCode::Enum key);
    }
    
} // end namespace ora


#endif /* defined(__orange__keycodes__) */
