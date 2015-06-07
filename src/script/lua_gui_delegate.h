#ifndef LUA_GUI_DELEGATE_H
#define LUA_GUI_DELEGATE_H

#include <mygui/MyGUIEngine/include/MyGUI.h>
#include <luaplus/LuaPlus.h>

namespace ora
{
    template<typename T1>
    class WidgetEventDelegate1 : public MyGUI::delegates::IDelegate1<T1>
    {
        LuaPlus::LuaFunction<void> fun_;
    public:
        typedef MyGUI::delegates::IDelegate1<T1> base_type;
        typedef WidgetEventDelegate1<T1> this_type;

        WidgetEventDelegate1(LuaPlus::LuaFunction<void> fun) : fun_(fun) {}

        virtual bool isType(const std::type_info& _type){ return typeid(this_type) == _type; }
        virtual void invoke(T1 t1){ fun_(t1); }
        virtual bool compare(base_type * _delegate) const { return this == _delegate; }
    };

    template<typename T1, typename T2>
    class WidgetEventDelegate2 : public MyGUI::delegates::IDelegate2<T1, T2>
    {
        LuaPlus::LuaFunction<void> fun_;
    public:
        typedef MyGUI::delegates::IDelegate2<T1, T2> base_type;
        typedef WidgetEventDelegate2<T1, T2> this_type;

        WidgetEventDelegate2(LuaPlus::LuaFunction<void> fun) : fun_(fun) {}

        virtual bool isType(const std::type_info& _type){ return typeid(this_type) == _type; }
        virtual void invoke(T1 t1, T2 t2){ fun_(t1, t2); }
        virtual bool compare(base_type * _delegate) const { return this == _delegate; }
    };

    template<typename T1, typename T2, typename T3>
    class WidgetEventDelegate3 : public MyGUI::delegates::IDelegate3<T1, T2, T3>
    {
        LuaPlus::LuaFunction<void> fun_;
    public:
        typedef MyGUI::delegates::IDelegate3<T1, T2, T3> base_type;
        typedef WidgetEventDelegate3<T1, T2, T3> this_type;

        WidgetEventDelegate3(LuaPlus::LuaFunction<void> fun) : fun_(fun) {}

        virtual bool isType(const std::type_info& _type){ return typeid(this_type) == _type; }
        virtual void invoke(T1 t1, T2 t2, T3 t3){ fun_(t1, t2, t3); }
        virtual bool compare(base_type * _delegate) const { return this == _delegate; }
    };

    template<typename T1, typename T2, typename T3, typename T4>
    class WidgetEventDelegate4 : public MyGUI::delegates::IDelegate4<T1, T2, T3, T4>
    {
        LuaPlus::LuaFunction<void> fun_;
    public:
        typedef MyGUI::delegates::IDelegate4<T1, T2, T3, T4> base_type;
        typedef WidgetEventDelegate4<T1, T2, T3, T4> this_type;

        WidgetEventDelegate4(LuaPlus::LuaFunction<void> fun) : fun_(fun) {}

        virtual bool isType(const std::type_info& _type){ return typeid(this_type) == _type; }
        virtual void invoke(T1 t1, T2 t2, T3 t3, T4 t4){ fun_(t1, t2, t3, t4); }
        virtual bool compare(base_type * _delegate) const { return this == _delegate; }
    };

}

#endif //LUA_GUI_DELEGATE_H
