//
//  input_mgr.h
//  orange
//
//  Created by zhoubao on 14-6-11.
//
//

#ifndef __orange__touch_mgr__
#define __orange__touch_mgr__

#include "util/singleton.h"
#include "util/event_listener.h"
#include "math/vector2.hpp"

#include <SDL/include/SDL.h>

namespace ora
{
    typedef SDL_TouchFingerEvent    TouchEvent;
    typedef SDL_MouseButtonEvent    MouseButtonEvent;
    typedef SDL_MouseMotionEvent    MouseMoveEvent;
    typedef SDL_MouseWheelEvent     MouseWheelEvent;
    typedef SDL_KeyboardEvent       KeyboardEvent;
    typedef SDL_TextEditingEvent    TextEditingEvent;
    typedef SDL_TextInputEvent      TextInputEvent;
    
    class InputMgr :
        public Singleton<InputMgr>
    {
    public:
        InputMgr();
        ~InputMgr();
        
        bool init();
        void fini();
        
        void onTouchEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onMouseButtonEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onMouseMoveEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onMouseWheelEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onKeyboardEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onTextEditingEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        void onTextInputEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        
    private:
        void onTouchBegin(const Vector2 & pos, int btn);
        void onTouchMove(const Vector2 & pos, int btn, const Vector2 & delta);
        void onMouseMove(const Vector2 & pos, int btn, const Vector2 & delta);
        void onMouseWheel(const Vector2 & delta);
        void onTouchEnd(const Vector2 & pos, int btn);

    private:
        EventListenerWraperPtr touchEvent_;
        EventListenerWraperPtr mouseMoveEvent_;
        EventListenerWraperPtr mouseWheelEvent_;
        EventListenerWraperPtr mouseButtonEvent_;
        EventListenerWraperPtr keyboardEvent_;
        EventListenerWraperPtr textEditingEvent_;
        EventListenerWraperPtr textInputEvent_;
        
    };
    
} // end namespace ora

#endif /* defined(__orange__touch_mgr__) */
