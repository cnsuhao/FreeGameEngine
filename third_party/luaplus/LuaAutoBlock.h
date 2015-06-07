///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2010 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://luaplus.org/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifndef LUAAUTOBLOCK_H
#define LUAAUTOBLOCK_H

#include "LuaPlusInternal.h"

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus {

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/**
	A helper class for automatically setting the stack state back to point of
	LuaAutoBlock creation.
**/
class LuaAutoBlock {
public:
	LuaAutoBlock(LuaState* state)
		: L(LuaState_to_lua_State(state))
		, stackTop(lua_gettop(L))
    {
        lua_pushcfunction(L, errorHandler);
	}

	LuaAutoBlock(lua_State* state)
		: L(state)
		, stackTop(lua_gettop(L))
    {
        lua_pushcfunction(L, errorHandler);
	}

	LuaAutoBlock(LuaStackObject& object)
		: L(object.GetCState())
		, stackTop(lua_gettop(L))
    {
        lua_pushcfunction(L, errorHandler);
	}

	~LuaAutoBlock() {
		lua_settop(L, stackTop);
	}
    
    int pcall(int nargs, int nresults)
    {
        int ret = lua_pcall(L, nargs, nresults, stackTop+1);
        if(ret != 0)
        {
			const char* errorString = lua_tostring(L, -1);
            ErrorMessage(errorString);
        }
        
        return ret;
    }
    
    static int errorHandler(lua_State *L)
    {
        lua_getfield(L, LUA_GLOBALSINDEX, "debug");
        if (!lua_istable(L, -1))
        {
            lua_pop(L, 1);
            return 1;
        }
        lua_getfield(L, -1, "traceback");
        if (!lua_isfunction(L, -1))
        {
            lua_pop(L, 2);
            return 1;
        }
        lua_pushvalue(L, 1);
        lua_pushinteger(L, 2);
        lua_call(L, 2, 1);
        return 1;
    }

private:
	LuaAutoBlock(const LuaAutoBlock& src);					// Not implemented
	const LuaAutoBlock& operator=(const LuaAutoBlock& src);	// Not implemented

	lua_State* L;
	int stackTop;
};

} // namespace LuaPlus

#endif // LUAAUTOBLOCK_H
