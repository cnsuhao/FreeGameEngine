//
//  lua_gui.cpp
//  orange
//
//  Created by zhoubao on 14-6-17.
//
//

#include "lua_gui.h"

namespace LPCD
{
    void Push(lua_State* L, const MyGUI::Widget * value)
    {
        tolua_pushusertype(L, (void*)value, "Widget");
    }
    
    void Push(lua_State* L, const MyGUI::ItemBox * value)
    {
        tolua_pushusertype(L, (void*)value, "ItemBox");
    }

    void Push(lua_State* L, const MyGUI::GridView * value)
    {
        tolua_pushusertype(L, (void*) value, "GridView");
    }

    void Push(lua_State* L, const MyGUI::TabControl * value)
    {
        tolua_pushusertype(L, (void*) value, "TabControl");
    }

    void Push(lua_State* L, const MyGUI::ListBox * value)
    {
        tolua_pushusertype(L, (void*) value, "ListBox");
    }
}

#include <luaplus/LuaPlus.h>
#include "lua_gui_delegate.h"

namespace ora
{
    // set foucs
    void* widget_addEvent_setFocus(MyGUI::Widget * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_WidgetWidget::IDelegate *p = new WidgetEventDelegate2<MyGUI::Widget*, MyGUI::Widget*>(fun);
        self->eventMouseSetFocus += p;
        return p;
    }

    void  widget_delEvent_setFocus(MyGUI::Widget * self, void *p)
    {
        self->eventMouseSetFocus -= (MyGUI::EventHandle_WidgetWidget::IDelegate *)p;
    }

    // lost foucs
    void* widget_addEvent_lostFocus(MyGUI::Widget * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_WidgetWidget::IDelegate *p = new WidgetEventDelegate2<MyGUI::Widget*, MyGUI::Widget*>(fun);
        self->eventMouseLostFocus += p;
        return p;
    }

    void  widget_delEvent_lostFocus(MyGUI::Widget * self, void *p)
    {
        self->eventMouseLostFocus -= (MyGUI::EventHandle_WidgetWidget::IDelegate *)p;
    }

    // button click
    void* widget_addEvent_buttonClick(MyGUI::Widget * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_WidgetVoid::IDelegate *p = new WidgetEventDelegate1<MyGUI::Widget*>(fun);
        self->eventMouseButtonClick += p;
        return p;
    }

    void  widget_delEvent_buttonClick(MyGUI::Widget * self, void *p)
    {
        self->eventMouseButtonClick -= (MyGUI::EventHandle_WidgetVoid::IDelegate *)p;
    }

    // 
    void* gridView_addEvent_itemClick(MyGUI::GridView * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_GridViewItemClick::IDelegate *p = new WidgetEventDelegate2<MyGUI::GridView*, int>(fun);
        self->eventGridViewItemClick += p;
        return p;
    }
    void  gridView_delEvent_itemClick(MyGUI::GridView * self, void *p)
    {
        self->eventGridViewItemClick -= (MyGUI::EventHandle_GridViewItemClick::IDelegate *)p;
    }

    //
    void* itemBox_addEvent_createItem(MyGUI::ItemBox * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_ItemBoxPtrWidgetPtr::IDelegate *p = new WidgetEventDelegate2<MyGUI::ItemBox*, MyGUI::Widget*>(fun);
        self->requestCreateWidgetItem = p;
        return p;
    }
    void  itemBox_delEvent_createItem(MyGUI::ItemBox * self, void *p)
    {
        self->requestCreateWidgetItem.clear();
    }

	//
	void* tab_addEvent_selectChange(MyGUI::TabControl * self, lua_State * L, lua_Function funID)
	{
        LuaPlus::LuaObject fun(L, funID);
		MyGUI::EventHandle_TabPtrSizeT::IDelegate *p = new WidgetEventDelegate2<MyGUI::TabControl*, size_t>(fun);
		self->eventTabChangeSelect += p;
		return p;
	}
	void  tab_delEvent_selectChange(MyGUI::TabControl * self, void *p)
	{
		self->eventTabChangeSelect -= (MyGUI::EventHandle_TabPtrSizeT::IDelegate *)p;
	}

	//
	void* edt_addEvent_textChange(MyGUI::EditBox * self, lua_State * L, lua_Function funID)
	{
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_EditPtr::IDelegate *p = new WidgetEventDelegate1<MyGUI::EditBox*>(fun);
		self->eventEditTextChange += p;
		return p;
	}
	void  edt_delEvent_textChange(MyGUI::EditBox * self, void *p)
	{
        self->eventEditTextChange -= (MyGUI::EventHandle_EditPtr::IDelegate *)p;
	}

    //
    void* window_addEvent_buttonClick(MyGUI::Window * self, lua_State * L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_WindowPtrCStringRef::IDelegate *p = new WidgetEventDelegate2<MyGUI::Window*, const std::string &>(fun);
		self->eventWindowButtonPressed += p;
		return p;
    }

    void  window_delEvent_buttonClick(MyGUI::Window * self, void *p)
    {
        self->eventWindowButtonPressed -= (MyGUI::EventHandle_WindowPtrCStringRef::IDelegate *)p;
    }

    //
    void* listBox_addEvent_itemClick(MyGUI::ListBox *self, lua_State *L, lua_Function funID)
    {
        LuaPlus::LuaObject fun(L, funID);
        MyGUI::EventHandle_ListPtrSizeT::IDelegate *p = new WidgetEventDelegate2<MyGUI::ListBox*, size_t>(fun);
        self->eventListMouseItemActivate += p;
        return p;
    }

    void  listBox_delEvent_itemClick(MyGUI::ListBox *self, void *p)
    {
        self->eventListMouseItemActivate -= (MyGUI::EventHandle_ListPtrSizeT::IDelegate *)p;
    }

} // end namespace ora
