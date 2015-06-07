//
//  touch_mgr.cpp
//  orange
//
//  Created by zhoubao on 14-6-11.
//
//

#include "input_mgr.h"
#include "util/event_def.h"
#include "util/assert_tool.h"
#include "util/keycodes.h"

#include "graphics/render_device.h"
#include "core/world.h"

#include <string>

#include "script.h"
#include <luaplus/LuaPlus.h>


namespace ora
{
    IMPLEMENT_SINGLETON(InputMgr);
    
    InputMgr::InputMgr()
    {
    }
    
    InputMgr::~InputMgr()
    {
    }
    
    bool InputMgr::init()
    {
#if defined(IOS) || defined(ANDROID)
        touchEvent_ = newListener(this, &InputMgr::onTouchEvent);
        touchEvent_->subscribeEvent(ET::Touch);
#else
        mouseMoveEvent_ = newListener(this, &InputMgr::onMouseMoveEvent);
        mouseMoveEvent_->subscribeEvent(ET::MouseMove);
        
        mouseButtonEvent_ = newListener(this, &InputMgr::onMouseButtonEvent);
        mouseButtonEvent_->subscribeEvent(ET::MouseButton);

        mouseWheelEvent_ = newListener(this, &InputMgr::onMouseWheelEvent);
        mouseWheelEvent_->subscribeEvent(ET::MouseWheel);
#endif
        keyboardEvent_ = newListener(this, &InputMgr::onKeyboardEvent);
        keyboardEvent_->subscribeEvent(ET::Keyboard);
        
        textEditingEvent_ = newListener(this, &InputMgr::onTextEditingEvent);
        textEditingEvent_->subscribeEvent(ET::TextEditing);
        
        textInputEvent_ = newListener(this, &InputMgr::onTextInputEvent);
        textInputEvent_->subscribeEvent(ET::TextInput);
        
        KeyCode::init();
        
        return true;
    }
    
    void InputMgr::fini()
    {
    }
    
    
    void InputMgr::onTouchBegin(const Vector2 & pos, int btn)
    {
         LuaPlus::LuaState * pState = ScriptMgr::instance()->getLuaPlus();
         LuaPlus::LuaObject fun = pState->GetGlobal("onTouchBegin");
         if(fun.IsFunction())
         {
             LuaPlus::LuaFunction<void> _fun(fun);
             _fun(pos.x, pos.y, btn);
         }
    }
    
    void InputMgr::onTouchMove(const Vector2 & pos, int btn, const Vector2 & delta)
    {
        LuaPlus::LuaState * pState = ScriptMgr::instance()->getLuaPlus();
        LuaPlus::LuaObject fun = pState->GetGlobal("onTouchMove");
        if(fun.IsFunction())
        {
            LuaPlus::LuaFunction<void> _fun(fun);
            _fun(pos.x, pos.y, btn, delta.x, delta.y);
        }
    }

    void InputMgr::onTouchEnd(const Vector2 & pos, int btn)
    {
        LuaPlus::LuaState * pState = ScriptMgr::instance()->getLuaPlus();
        LuaPlus::LuaObject fun = pState->GetGlobal("onTouchEnd");
        if(fun.IsFunction())
        {
            LuaPlus::LuaFunction<void> _fun(fun);
            _fun(pos.x, pos.y, btn);
        }
    }

    void InputMgr::onTouchEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::Touch);
    
        TouchEvent event;
        if(parse_arguments(args, &event))
        {
            const Viewport & vp = renderDev()->getViewPort();
            Vector2 pos(event.x * vp.width_, event.y * vp.height_);
            
            if(event.type == SDL_FINGERDOWN)
            {
                onTouchBegin(pos, MyGUI::MouseButton::Left);
            }
            else if(event.type == SDL_FINGERMOTION)
            {
                Vector2 delta(event.dx * vp.width_, event.dy * vp.height_);
                onTouchMove(pos, MyGUI::MouseButton::Left, delta);
            }
            else if(event.type == SDL_FINGERUP)
            {
                onTouchEnd(pos, MyGUI::MouseButton::Left);
            }
        }
    }
    
    void InputMgr::onMouseButtonEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::MouseButton);
        
        MouseButtonEvent event;
        if(parse_arguments(args, &event))
        {
			MyGUI::MouseButton::Enum btn = MyGUI::MouseButton::Enum::None;
			if (event.button == 1) btn = MyGUI::MouseButton::Enum::Left;
			if (event.button == 2) btn = MyGUI::MouseButton::Enum::Middle;
			if (event.button == 3) btn = MyGUI::MouseButton::Enum::Right;

            Vector2 pos((float)event.x, (float)event.y);
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
                onTouchBegin(pos, btn);
			}
            else if(event.type == SDL_MOUSEBUTTONUP)
            {
                onTouchEnd(pos, btn);
            }
        }
    }
    
    void InputMgr::onMouseMoveEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::MouseMove);
        
        MouseMoveEvent event;
        if(parse_arguments(args, &event))
        {
            if(event.type == SDL_MOUSEMOTION)
            {
                Vector2 pos((float)event.x, (float)event.y);
                
                MyGUI::MouseButton::Enum btn = MyGUI::MouseButton::None;
			    if (event.state & SDL_BUTTON_LMASK) btn = MyGUI::MouseButton::Left;
			    if (event.state & SDL_BUTTON_MMASK) btn = MyGUI::MouseButton::Middle;
			    if (event.state & SDL_BUTTON_RMASK) btn = MyGUI::MouseButton::Right;
                
                if(btn != MyGUI::MouseButton::None)
                {
                    Vector2 delta((float)event.xrel, (float)event.yrel);
                    onTouchMove(pos, btn, delta);
                }
                else
                {
                    if(MyGUI::InputManager::getInstance().injectMouseMove(event.x, event.y, 0))
                        return;
                }
            }
        }
    }

    void InputMgr::onMouseWheelEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::MouseWheel);

        MouseWheelEvent event;
        if(!parse_arguments(args, &event)) return;

#ifndef _RELEASE
        if(event.type == SDL_MOUSEWHEEL)
        {
            ora::CameraPtr camera = ora::World::instance()->getCamera();
            if(camera)
            {
                camera->translateForward(event.y * 0.5f);
            }
        }
#endif
    }
    
    void InputMgr::onKeyboardEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::Keyboard);
        KeyboardEvent event;
        if(parse_arguments(args, &event))
        {
            LuaPlus::LuaState * pState = ScriptMgr::instance()->getLuaPlus();
            LuaPlus::LuaObject fun = pState->GetGlobal("onKeyEvent");
            if (fun.IsFunction())
            {
                LuaPlus::LuaFunction<void> _fun(fun);
                _fun(event.type == SDL_KEYDOWN, int(event.keysym.scancode));
            }
        }
    }

    void InputMgr::onTextEditingEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::TextEditing);
        
        TextEditingEvent event;
        if(parse_arguments(args, &event))
        {
            //ORA_LOG_DEBUG("onTextEditingEvent: [%s]", event.text);
        }
    }
    
    void InputMgr::onTextInputEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ASSERT_1(eventType == ET::TextInput);
        
        TextInputEvent event;
        if(parse_arguments(args, &event))
        {
            MyGUI::KeyCode::Enum key = MyGUI::KeyCode::Enum::None;
            MyGUI::UString text(event.text);
            for(size_t i = 0; i < text.size(); ++i)
            {
                MyGUI::InputManager::getInstance().injectKeyPress(key, text[i]);
            }
        }
    }
    
} // end namespace ora
