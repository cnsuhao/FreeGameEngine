//
//  lua_gui.h
//  orange
//
//  Created by zhoubao on 14-6-17.
//
//

#ifndef __orange__lua_gui__
#define __orange__lua_gui__

#include <mygui/MyGUIEngine/include/MyGUI.h>
#include <toluaplus/include/tolua++.h>

namespace ora
{
    void* widget_addEvent_setFocus(MyGUI::Widget * self, lua_State * L, lua_Function funID);
    void  widget_delEvent_setFocus(MyGUI::Widget * self, void *p);

    void* widget_addEvent_lostFocus(MyGUI::Widget * self, lua_State * L, lua_Function funID);
    void  widget_delEvent_lostFocus(MyGUI::Widget * self, void *p);

    void* widget_addEvent_buttonClick(MyGUI::Widget * self, lua_State * L, lua_Function funID);
    void  widget_delEvent_buttonClick(MyGUI::Widget * self, void *p);

    void* gridView_addEvent_itemClick(MyGUI::GridView * self, lua_State * L, lua_Function funID);
    void  gridView_delEvent_itemClick(MyGUI::GridView * self, void *p);

    void* itemBox_addEvent_createItem(MyGUI::ItemBox * self, lua_State * L, lua_Function funID);
    void  itemBox_delEvent_createItem(MyGUI::ItemBox * self, void *p);

    void* tab_addEvent_selectChange(MyGUI::TabControl * self, lua_State * L, lua_Function funID);
	void  tab_delEvent_selectChange(MyGUI::TabControl * self, void *p);
	
    void* edt_addEvent_textChange(MyGUI::EditBox * self, lua_State * L, lua_Function funID);
	void  edt_delEvent_textChange(MyGUI::EditBox * self, void *p);

    void* window_addEvent_buttonClick(MyGUI::Window * self, lua_State * L, lua_Function funID);
    void  window_delEvent_buttonClick(MyGUI::Window * self, void *p);

    void* listBox_addEvent_itemClick(MyGUI::ListBox *self, lua_State *L, lua_Function funID);
    void  listBox_delEvent_itemClick(MyGUI::ListBox *self, void *p);
    
} // end namespace ora

#endif /* defined(__orange__lua_gui__) */
