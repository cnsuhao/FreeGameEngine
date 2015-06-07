
#include "helper.h"

//两个__index函数写在引擎里，只是为了让lua调试器单步执行时忽略这index操作。

int instance__index(lua_State * L)
{
    int iTop = lua_gettop(L);

    if(!lua_istable(L, 1))
        return luaL_argerror(L, 1, "table needed.");
    if(!lua_isstring(L, 2))
        return luaL_argerror(L, 2, "string needed.");
    if(!lua_isnone(L, 3))
        return luaL_argerror(L, 3, "no object expected.");

    // metatable is class table, try get the attribute from class.
    lua_getmetatable(L, 1);
    if(lua_isnil(L, -1))
    {
        lua_pop(L, 1);
        return luaL_error(L, "instance__index: no metatable.");
    }

    lua_pushvalue(L, 2);
    lua_gettable(L, -2);

    if(lua_isnil(L, -1)) // it was not found.
    {
        lua_remove(L, -1); // remove the nil value.

        // get the class name.
        lua_pushstring(L, "__name");
        lua_rawget(L, -2);
        const char * name = lua_tostring(L, -1);

        luaL_error(L, "instance__index: can't find attribute '%s' for <%s: %p>", 
            lua_tostring(L, 2), name ? name : "table", lua_topointer(L, 1));

        lua_settop(L, iTop);
        return 0;
    }

    lua_remove(L, -2); // remove the metatable.
    assert(iTop + 1 == lua_gettop(L));
    return 1;
}


int module__index(lua_State * L)
{
    int iTop = lua_gettop(L);

    if(!lua_istable(L, 1))
        return luaL_argerror(L, 1, "table needed.");
    if(!lua_isstring(L, 2))
        return luaL_argerror(L, 2, "string needed.");
    if(!lua_isnone(L, 3))
        return luaL_argerror(L, 3, "no object expected.");

    const char * attr = lua_tostring(L, 2);
    
    // try get the attribute from global table.
    lua_pushvalue(L, 2);
    lua_rawget(L, LUA_GLOBALSINDEX);
    if(lua_isnil(L, -1))
    {
        // get the class name.
        lua_pushstring(L, "__name");
        lua_rawget(L, 1);
        const char * name = lua_tostring(L, -1);

        luaL_error(L, "module__index: can't find attribute '%s' for <%s: %p>", 
            attr, name ? name : "table", lua_topointer(L, 1));

        lua_settop(L, iTop);
        return 0;
    }

    assert(iTop + 1 == lua_gettop(L));
    return 1;
}

void registe_global_function(lua_State * L, const char * name, lua_CFunction fun)
{
    lua_pushstring(L, name);
    lua_pushcfunction(L, fun);
    lua_rawset(L, LUA_GLOBALSINDEX);
}

