
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <lobject.h>
#include <lgc.h>
#include <lstate.h>

TValue *index2adr(lua_State *L, int idx);
}

#include <vector>

#define LUA_VECTOR_LIBNAME	"vector"

typedef std::vector<TValue> lua_vector_t;

static TValue nil;

bool checktype(lua_State *L, int narg, int tp)
{
    if (lua_type(L, narg) != tp)
    {
        luaL_typerror(L, narg, lua_typename(L, tp));
        return false;
    }
    return true;
}

#define R_CHECK_TYPE(L, narg, type) if(!checktype(L, narg, type)) return -1
#define R_CHECK(L, EXP, ...) if(!(EXP)){luaL_error(L, __VA_ARGS__); return -1;}


static int vector_new(lua_State *L)
{
    int top = lua_gettop(L);
    lua_vector_t * p = (lua_vector_t*) lua_newuserdata(L, sizeof(lua_vector_t));
    new (p) lua_vector_t(top);

    for (int i = 1; i <= top; ++i)
    {
        TValue* pv = index2adr(L, i);
        ((*p)[i - 1]) = *pv;
        luaC_barrier(L, L->top - 1, pv); // keep reference
    }

    luaL_getmetatable(L, LUA_VECTOR_LIBNAME);
    if (lua_isnil(L, -1))
    {
        lua_settop(L, top);
        luaL_error(L, "can't find metatable " LUA_VECTOR_LIBNAME);
        return -1;
    }
    lua_setmetatable(L, -2);
    return 1;
}

static int vector_size(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    lua_pushinteger(L, p->size());
    return 1;
}

static int vector_empty(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    lua_pushboolean(L, p->empty());
    return 1;
}

static int vector_get(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    R_CHECK_TYPE(L, 2, LUA_TNUMBER);

    size_t i = (size_t) lua_tointeger(L, 2);
    R_CHECK(L, i < p->size(), "index(%d) out of range", i);

    lua_pushnil(L);
    setobj(L, L->top - 1, &(p->at(i)));
    return 1;
}

static int vector_set(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;
    R_CHECK_TYPE(L, 2, LUA_TNUMBER);
    R_CHECK(L, !lua_isnone(L, 3), "lack of argument #3");

    size_t i = (size_t) lua_tointeger(L, 2);
    R_CHECK(L, i < p->size(), "index(%d) out of range", i);
        
    setobj(L, &(p->at(i)), index2adr(L, 3));
    return 0;
}

static int vector_push_back(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    R_CHECK(L, !lua_isnone(L, 2), "lack of argument #2");

    p->push_back(*index2adr(L, 2));
    return 0;
}

static int vector_pop_back(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    R_CHECK(L, !p->empty(), "the vector is empty");

    lua_pushnil(L);
    setobj(L, L->top - 1, &p->back());
    luaC_barrier(L, index2adr(L, 1), L->top - 1);

    p->pop_back();
    return 1;
}

static int vector_clear(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    p->clear();
    return 0;
}

static int vector_resize(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    luaL_checktype(L, 2, LUA_TNUMBER);
    if (lua_isnumber(L, 2))
    {
        size_t size = (size_t) lua_tonumber(L, 2);
        p->reserve(size);

        while (p->size() < size)
            p->push_back(nil);

        while (p->size() > size)
            p->pop_back();
    }

    p->clear();
    return 0;
}

static int vector__index(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    if (lua_isnumber(L, 2))
        return vector_get(L); // the arguments are same, so we can call it directly.
    
    luaL_getmetatable(L, LUA_VECTOR_LIBNAME);
    if (lua_isnil(L, -1))
    {
        lua_pop(L, 1); // pop the nil value
        luaL_error(L, "can't find metatable " LUA_VECTOR_LIBNAME);
        return -1;
    }

    lua_pushvalue(L, 2);
    lua_rawget(L, -2);
    lua_remove(L, -2); // remove the metatable
    return 1;
}


static int vector__newindex(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    if (lua_isnumber(L, 2))
        return vector_set(L); // the arguments are same, so we can call it directly.
    
    luaL_error(L, "can't set attribute for userdata type");
    return -1;
}

static int vector__gc(lua_State *L)
{
    lua_vector_t * p = (lua_vector_t*) luaL_checkudata(L, 1, LUA_VECTOR_LIBNAME);
    if (!p) return -1;

    p->~lua_vector_t();
    return 0;
}

static const luaL_Reg vectorlib [] = {
    { "new", vector_new },
    { "size", vector_size },
    { "get", vector_get },
    { "set", vector_set },
    { "push_back", vector_push_back },
    { "pop_back", vector_pop_back },
    { "clear", vector_clear },
    { "resize", vector_resize },
    { "__index", vector__index },
    { "__newindex", vector__newindex },
    { "__gc", vector__gc },
    { NULL, NULL },
};

void luaopen_vector(lua_State *L)
{
    setnilvalue(&nil);

    lua_pushstring(L, LUA_VECTOR_LIBNAME);
    luaL_newmetatable(L, LUA_VECTOR_LIBNAME);
    for (const luaL_Reg * p = vectorlib; p->name != NULL; ++p)
    {
        lua_pushstring(L, p->name);
        lua_pushcfunction(L, p->func);
        lua_rawset(L, -3);
    }

    lua_rawset(L, LUA_GLOBALSINDEX);
}
