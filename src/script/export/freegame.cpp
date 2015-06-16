
#include "script/helper.h"
#include "script/tolua_converter.h"
#include "core/world.h"

using namespace ora;

////////////////////////////////////////////////////////////////////////////////

LUA_FUN_HEADER(lua_frg_SceneNode_getChildren)
{
    SceneNode *self;
    LUA_PARSE_METHOD(L, "T(frg.SceneNode)T(frg.SceneNode)", &self);
    
    lua_newtable(L);
    const SceneNode::Containder & children = self->getChildren();
    int i = 1;
    for(const SceneNodePtr & child : children)
    {
        object_to_luaval(L, child.get(), nullptr);
        lua_rawseti(L, -2, i++);
    }
    return 1;
}

int lua_frg_register_SceneNode(lua_State *L)
{
    tolua_beginmodule(L, "SceneNode");
    {
        tolua_function(L, "getChildren", lua_frg_SceneNode_getChildren);
    }
    tolua_endmodule(L);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
LUA_FUN_HEADER(lua_frg_World_instance)
{
    int ut;
    LUA_PARSE_FUN(L, "U(frg.World)", &ut);
    
    object_to_luaval(L, World::instance(), "frg.World");
    return 1;
}

LUA_FUN_HEADER(lua_frg_register_World)
{
    tolua_beginmodule(L, "World");
    {
        tolua_function(L, "instance", lua_frg_World_instance);
    }
    tolua_endmodule(L);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int register_frg_manual(lua_State *L)
{
    tolua_beginmodule(L, "frg");
    {
        lua_frg_register_SceneNode(L);
        lua_frg_register_World(L);
    }
    tolua_endmodule(L);
    return 0;
}
