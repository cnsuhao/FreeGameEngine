//
//  keycodes.cpp
//  orange
//
//  Created by zhoubao on 14-6-12.
//
//

#include "keycodes.h"
#include "log_tool.h"

#include <map>

namespace ora
{
    namespace KeyCode
    {
        static std::map<SDL_Scancode, MyGUI::KeyCode::Enum> sdlKeyMap;
        static std::map<MyGUI::KeyCode::Enum, SDL_Scancode> myguiKeyMap;
        
        void init()
        {
            ORA_STACK_TRACE;
            
#define KEY_MAP(SDL_KEY, MYGUI_KEY)\
    sdlKeyMap[SDL_KEY] = MYGUI_KEY; \
    myguiKeyMap[MYGUI_KEY] = SDL_KEY
    
            KEY_MAP(SDL_SCANCODE_UNKNOWN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_A, MyGUI::KeyCode::A);
            KEY_MAP(SDL_SCANCODE_B, MyGUI::KeyCode::B);
            KEY_MAP(SDL_SCANCODE_C, MyGUI::KeyCode::C);
            KEY_MAP(SDL_SCANCODE_D, MyGUI::KeyCode::D);
            KEY_MAP(SDL_SCANCODE_E, MyGUI::KeyCode::E);
            KEY_MAP(SDL_SCANCODE_F, MyGUI::KeyCode::F);
            KEY_MAP(SDL_SCANCODE_G, MyGUI::KeyCode::G);
            KEY_MAP(SDL_SCANCODE_H, MyGUI::KeyCode::H);
            KEY_MAP(SDL_SCANCODE_I, MyGUI::KeyCode::I);
            KEY_MAP(SDL_SCANCODE_J, MyGUI::KeyCode::J);
            KEY_MAP(SDL_SCANCODE_K, MyGUI::KeyCode::K);
            KEY_MAP(SDL_SCANCODE_L, MyGUI::KeyCode::L);
            KEY_MAP(SDL_SCANCODE_M, MyGUI::KeyCode::M);
            KEY_MAP(SDL_SCANCODE_N, MyGUI::KeyCode::N);
            KEY_MAP(SDL_SCANCODE_O, MyGUI::KeyCode::O);
            KEY_MAP(SDL_SCANCODE_P, MyGUI::KeyCode::P);
            KEY_MAP(SDL_SCANCODE_Q, MyGUI::KeyCode::Q);
            KEY_MAP(SDL_SCANCODE_R, MyGUI::KeyCode::R);
            KEY_MAP(SDL_SCANCODE_S, MyGUI::KeyCode::S);
            KEY_MAP(SDL_SCANCODE_T, MyGUI::KeyCode::T);
            KEY_MAP(SDL_SCANCODE_U, MyGUI::KeyCode::U);
            KEY_MAP(SDL_SCANCODE_V, MyGUI::KeyCode::V);
            KEY_MAP(SDL_SCANCODE_W, MyGUI::KeyCode::W);
            KEY_MAP(SDL_SCANCODE_X, MyGUI::KeyCode::X);
            KEY_MAP(SDL_SCANCODE_Y, MyGUI::KeyCode::Y);
            KEY_MAP(SDL_SCANCODE_Z, MyGUI::KeyCode::Z);
            
            KEY_MAP(SDL_SCANCODE_1, MyGUI::KeyCode::One);
            KEY_MAP(SDL_SCANCODE_2, MyGUI::KeyCode::Two);
            KEY_MAP(SDL_SCANCODE_3, MyGUI::KeyCode::Three);
            KEY_MAP(SDL_SCANCODE_4, MyGUI::KeyCode::Four);
            KEY_MAP(SDL_SCANCODE_5, MyGUI::KeyCode::Five);
            KEY_MAP(SDL_SCANCODE_6, MyGUI::KeyCode::Six);
            KEY_MAP(SDL_SCANCODE_7, MyGUI::KeyCode::Seven);
            KEY_MAP(SDL_SCANCODE_8, MyGUI::KeyCode::Eight);
            KEY_MAP(SDL_SCANCODE_9, MyGUI::KeyCode::Nine);
            KEY_MAP(SDL_SCANCODE_0, MyGUI::KeyCode::Zero);
            
            KEY_MAP(SDL_SCANCODE_RETURN, MyGUI::KeyCode::Return);
            KEY_MAP(SDL_SCANCODE_ESCAPE, MyGUI::KeyCode::Escape);
            KEY_MAP(SDL_SCANCODE_BACKSPACE, MyGUI::KeyCode::Backspace);
            KEY_MAP(SDL_SCANCODE_TAB, MyGUI::KeyCode::Tab);
            KEY_MAP(SDL_SCANCODE_SPACE, MyGUI::KeyCode::Space);
            
            KEY_MAP(SDL_SCANCODE_MINUS, MyGUI::KeyCode::Minus);
            KEY_MAP(SDL_SCANCODE_EQUALS, MyGUI::KeyCode::Equals);
            KEY_MAP(SDL_SCANCODE_LEFTBRACKET, MyGUI::KeyCode::LeftBracket);
            KEY_MAP(SDL_SCANCODE_RIGHTBRACKET, MyGUI::KeyCode::RightBracket);
            KEY_MAP(SDL_SCANCODE_BACKSLASH, MyGUI::KeyCode::Backslash);
//            KEY_MAP(SDL_SCANCODE_NONUSHASH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_SEMICOLON, MyGUI::KeyCode::Semicolon);
            KEY_MAP(SDL_SCANCODE_APOSTROPHE, MyGUI::KeyCode::Apostrophe);
            KEY_MAP(SDL_SCANCODE_GRAVE, MyGUI::KeyCode::Grave);
            KEY_MAP(SDL_SCANCODE_COMMA, MyGUI::KeyCode::Comma);
            KEY_MAP(SDL_SCANCODE_PERIOD, MyGUI::KeyCode::Period);
            KEY_MAP(SDL_SCANCODE_SLASH, MyGUI::KeyCode::Slash);
            
            KEY_MAP(SDL_SCANCODE_CAPSLOCK, MyGUI::KeyCode::Capital);
            
            KEY_MAP(SDL_SCANCODE_F1, MyGUI::KeyCode::F1);
            KEY_MAP(SDL_SCANCODE_F2, MyGUI::KeyCode::F2);
            KEY_MAP(SDL_SCANCODE_F3, MyGUI::KeyCode::F3);
            KEY_MAP(SDL_SCANCODE_F4, MyGUI::KeyCode::F4);
            KEY_MAP(SDL_SCANCODE_F5, MyGUI::KeyCode::F5);
            KEY_MAP(SDL_SCANCODE_F6, MyGUI::KeyCode::F6);
            KEY_MAP(SDL_SCANCODE_F7, MyGUI::KeyCode::F7);
            KEY_MAP(SDL_SCANCODE_F8, MyGUI::KeyCode::F8);
            KEY_MAP(SDL_SCANCODE_F9, MyGUI::KeyCode::F9);
            KEY_MAP(SDL_SCANCODE_F10, MyGUI::KeyCode::F10);
            KEY_MAP(SDL_SCANCODE_F11, MyGUI::KeyCode::F11);
            KEY_MAP(SDL_SCANCODE_F12, MyGUI::KeyCode::F12);
            
//            KEY_MAP(SDL_SCANCODE_PRINTSCREEN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_SCROLLLOCK, MyGUI::KeyCode::ScrollLock);
            KEY_MAP(SDL_SCANCODE_PAUSE, MyGUI::KeyCode::Pause);
            KEY_MAP(SDL_SCANCODE_INSERT, MyGUI::KeyCode::Insert);
            KEY_MAP(SDL_SCANCODE_HOME, MyGUI::KeyCode::Home);
            KEY_MAP(SDL_SCANCODE_PAGEUP, MyGUI::KeyCode::PageUp);
            KEY_MAP(SDL_SCANCODE_DELETE, MyGUI::KeyCode::Delete);
            KEY_MAP(SDL_SCANCODE_END, MyGUI::KeyCode::End);
            KEY_MAP(SDL_SCANCODE_PAGEDOWN, MyGUI::KeyCode::PageDown);
            KEY_MAP(SDL_SCANCODE_RIGHT, MyGUI::KeyCode::ArrowRight);
            KEY_MAP(SDL_SCANCODE_LEFT, MyGUI::KeyCode::ArrowLeft);
            KEY_MAP(SDL_SCANCODE_DOWN, MyGUI::KeyCode::ArrowDown);
            KEY_MAP(SDL_SCANCODE_UP, MyGUI::KeyCode::ArrowUp);
            
            KEY_MAP(SDL_SCANCODE_NUMLOCKCLEAR, MyGUI::KeyCode::NumLock);
            KEY_MAP(SDL_SCANCODE_KP_DIVIDE, MyGUI::KeyCode::Divide);
            KEY_MAP(SDL_SCANCODE_KP_MULTIPLY, MyGUI::KeyCode::Multiply);
            KEY_MAP(SDL_SCANCODE_KP_MINUS, MyGUI::KeyCode::Subtract);
            KEY_MAP(SDL_SCANCODE_KP_PLUS, MyGUI::KeyCode::Add);
            KEY_MAP(SDL_SCANCODE_KP_ENTER, MyGUI::KeyCode::NumpadEnter);
            KEY_MAP(SDL_SCANCODE_KP_1, MyGUI::KeyCode::Numpad1);
            KEY_MAP(SDL_SCANCODE_KP_2, MyGUI::KeyCode::Numpad2);
            KEY_MAP(SDL_SCANCODE_KP_3, MyGUI::KeyCode::Numpad3);
            KEY_MAP(SDL_SCANCODE_KP_4, MyGUI::KeyCode::Numpad4);
            KEY_MAP(SDL_SCANCODE_KP_5, MyGUI::KeyCode::Numpad5);
            KEY_MAP(SDL_SCANCODE_KP_6, MyGUI::KeyCode::Numpad6);
            KEY_MAP(SDL_SCANCODE_KP_7, MyGUI::KeyCode::Numpad7);
            KEY_MAP(SDL_SCANCODE_KP_8, MyGUI::KeyCode::Numpad8);
            KEY_MAP(SDL_SCANCODE_KP_9, MyGUI::KeyCode::Numpad9);
            KEY_MAP(SDL_SCANCODE_KP_0, MyGUI::KeyCode::Numpad0);
            KEY_MAP(SDL_SCANCODE_KP_PERIOD, MyGUI::KeyCode::NumpadComma);
            
#if 0
            KEY_MAP(SDL_SCANCODE_NONUSBACKSLASH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_APPLICATION, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_POWER, MyGUI::KeyCode::None); 
            KEY_MAP(SDL_SCANCODE_KP_EQUALS, MyGUI::KeyCode::None);
#endif

            KEY_MAP(SDL_SCANCODE_F13, MyGUI::KeyCode::F13);
            KEY_MAP(SDL_SCANCODE_F14, MyGUI::KeyCode::F14);
            KEY_MAP(SDL_SCANCODE_F15, MyGUI::KeyCode::F15);
            
#if 0
            KEY_MAP(SDL_SCANCODE_F16, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F17, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F18, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F19, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F20, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F21, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F22, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F23, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_F24, MyGUI::KeyCode::None);
            
            KEY_MAP(SDL_SCANCODE_EXECUTE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_HELP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_MENU, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_SELECT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_STOP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AGAIN, MyGUI::KeyCode::None);   /**< redo */
            KEY_MAP(SDL_SCANCODE_UNDO, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CUT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_COPY, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_PASTE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_FIND, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_MUTE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_VOLUMEUP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_VOLUMEDOWN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_COMMA, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_EQUALSAS400, MyGUI::KeyCode::None);
            
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL1, MyGUI::KeyCode::None); 
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL2, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL3, MyGUI::KeyCode::None); /**< Yen */
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL4, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL5, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL6, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL7, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL8, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_INTERNATIONAL9, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_LANG1, MyGUI::KeyCode::None); /**< Hangul/English toggle */
            KEY_MAP(SDL_SCANCODE_LANG2, MyGUI::KeyCode::None); /**< Hanja conversion */
            KEY_MAP(SDL_SCANCODE_LANG3, MyGUI::KeyCode::None); /**< Katakana */
            KEY_MAP(SDL_SCANCODE_LANG4, MyGUI::KeyCode::None); /**< Hiragana */
            KEY_MAP(SDL_SCANCODE_LANG5, MyGUI::KeyCode::None); /**< Zenkaku/Hankaku */
            KEY_MAP(SDL_SCANCODE_LANG6, MyGUI::KeyCode::None); /**< reserved */
            KEY_MAP(SDL_SCANCODE_LANG7, MyGUI::KeyCode::None); /**< reserved */
            KEY_MAP(SDL_SCANCODE_LANG8, MyGUI::KeyCode::None); /**< reserved */
            KEY_MAP(SDL_SCANCODE_LANG9, MyGUI::KeyCode::None); /**< reserved */
            
            KEY_MAP(SDL_SCANCODE_ALTERASE, MyGUI::KeyCode::None); /**< Erase-Eaze */
            KEY_MAP(SDL_SCANCODE_SYSREQ, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CANCEL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CLEAR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_PRIOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_RETURN2, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_SEPARATOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_OUT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_OPER, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CLEARAGAIN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CRSEL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_EXSEL, MyGUI::KeyCode::None);
            
            KEY_MAP(SDL_SCANCODE_KP_00, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_000, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_THOUSANDSSEPARATOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_DECIMALSEPARATOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CURRENCYUNIT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CURRENCYSUBUNIT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_LEFTPAREN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_RIGHTPAREN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_LEFTBRACE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_RIGHTBRACE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_TAB, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_BACKSPACE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_A, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_B, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_C, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_D, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_E, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_F, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_XOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_POWER, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_PERCENT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_LESS, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_GREATER, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_AMPERSAND, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_DBLAMPERSAND, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_VERTICALBAR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_DBLVERTICALBAR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_COLON, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_HASH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_SPACE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_AT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_EXCLAM, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMSTORE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMRECALL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMCLEAR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMADD, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMSUBTRACT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMMULTIPLY, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_MEMDIVIDE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_PLUSMINUS, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_CLEAR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_CLEARENTRY, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_BINARY, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_OCTAL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_DECIMAL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KP_HEXADECIMAL, MyGUI::KeyCode::None);
#endif

            KEY_MAP(SDL_SCANCODE_LCTRL, MyGUI::KeyCode::LeftControl);
            KEY_MAP(SDL_SCANCODE_LSHIFT, MyGUI::KeyCode::LeftShift);
            KEY_MAP(SDL_SCANCODE_LALT, MyGUI::KeyCode::LeftAlt);
            KEY_MAP(SDL_SCANCODE_LGUI, MyGUI::KeyCode::LeftWindows);
            KEY_MAP(SDL_SCANCODE_RCTRL, MyGUI::KeyCode::RightControl);
            KEY_MAP(SDL_SCANCODE_RSHIFT, MyGUI::KeyCode::RightShift);
            KEY_MAP(SDL_SCANCODE_RALT, MyGUI::KeyCode::RightAlt);
            KEY_MAP(SDL_SCANCODE_RGUI, MyGUI::KeyCode::RightWindow);
            
#if 0
            KEY_MAP(SDL_SCANCODE_MODE, MyGUI::KeyCode::None);

            KEY_MAP(SDL_SCANCODE_AUDIONEXT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AUDIOPREV, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AUDIOSTOP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AUDIOPLAY, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AUDIOMUTE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_MEDIASELECT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_WWW, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_MAIL, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_CALCULATOR, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_COMPUTER, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_SEARCH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_HOME, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_BACK, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_FORWARD, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_STOP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_REFRESH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_AC_BOOKMARKS, MyGUI::KeyCode::None);

            KEY_MAP(SDL_SCANCODE_BRIGHTNESSDOWN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_BRIGHTNESSUP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_DISPLAYSWITCH, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KBDILLUMTOGGLE, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KBDILLUMDOWN, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_KBDILLUMUP, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_EJECT, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_SLEEP, MyGUI::KeyCode::None);

            KEY_MAP(SDL_SCANCODE_APP1, MyGUI::KeyCode::None);
            KEY_MAP(SDL_SCANCODE_APP2, MyGUI::KeyCode::None);
#endif
    
#undef KEY_MAP
        }
        
        MyGUI::KeyCode::Enum sdl2mygui(SDL_Scancode key)
        {
            auto it = sdlKeyMap.find(key);
            if(it != sdlKeyMap.end())
                return it->second;
            
            return MyGUI::KeyCode::None;
        }
        
        SDL_Scancode mygui2sdl(MyGUI::KeyCode::Enum key)
        {
            auto it = myguiKeyMap.find(key);
            if(it != myguiKeyMap.end())
                return it->second;
            
            return SDL_SCANCODE_UNKNOWN;
        }
    }
    
} // end namespace ora