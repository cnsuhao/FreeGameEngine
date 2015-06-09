
#include "script/helper.h"

#include "script/root_scene.h"

using namespace ora;

LUA_FUN_HEADER(lua_freeg_RootScene_create)
{
    int ut;
    LUA_PARSE_FUN(L, "U(freeg.RootScene)", &ut);
    
    RootScene *p = new RootScene();
    tolua_object(L, p, "freeg.RootScene");
    return 1;
}

int lua_freeg_register_RootScene(lua_State *L)
{
    tolua_usertype(L, "freeg.RootScene");
    tolua_cclass(L, "RootScene", "freeg.RootScene", "", nullptr);
    
    tolua_beginmodule(L, "RootScene");
    {
        tolua_function(L, "create", lua_freeg_RootScene_create);
    }
    tolua_endmodule(L);
    
    registerLuaTypeName<RootScene>("freeg.RootScene");
    return 0;
}

int register_freegame(lua_State *L)
{
    tolua_open(L);
    
    tolua_module(L, "freeg", 0);
    tolua_beginmodule(L, "freeg");
    {
        lua_freeg_register_RootScene(L);
    }
    tolua_endmodule(L);
    return 0;
}
