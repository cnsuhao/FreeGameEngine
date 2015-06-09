
#include <luaplus/LuaPlus.h>
#include <tolua++/include/tolua++.h>

#include "util/log_tool.h"
#include "util/smartptr.h"
#include "script.h"

#include <cstdarg>
#include <type_traits>

//这两个函数放在引擎里实现，是为了不让lua调试器单步执行到。
int instance__index(lua_State * L);
int module__index(lua_State * L);

void registe_global_function(lua_State * L, const char * name, lua_CFunction fun);

#define LUA_FUN_HEADER(NAME) static int NAME(lua_State *L)

/** parse arguments according to the format.
 @code
 int age;
 const char * name;
 if(!lua_parse_args(L, "is", &age, &name)) return false;
 @endcode
 *
 *  @param format: is a string, contains the following characters.
 *  b: boolean
 *  c: char
 *  C: unsingend char
 *  h: short
 *  H: unsigned short
 *  i: int
 *  I: unsigned int
 *  l: long
 *  L: unsigned long
 *  v: long long
 *  V: unsigned long long
 *  f: float
 *  d: double
 *  F: function
 *  t: table
 *  T: usertype
 *  T(TypeName): usertype with typename
 *  u: userdata
 *  U(TypeName): usertable with typename
 *  *: anything the lua supported
 *  .: placeholder
 *  |: means the following arguments are default
 *  ?: ignore the remain arguments
 *
 *  @return return true if parse succed, else false.
 */
bool lua_parse_args(lua_State *L, const char * format, ...);

#define LUA_PARSE_METHOD(L, format, ...) \
if(!lua_parse_args(L, format, __VA_ARGS__, nullptr) || !self){\
    ORA_LOG_ERROR("Failed to parse method %s", __FUNCTION__ );\
    return 0;\
}

#define LUA_PARSE_FUN(L, format, ...)\
if(!lua_parse_args(L, format, __VA_ARGS__, nullptr)){\
    ORA_LOG_ERROR("Failed to parse function %s", __FUNCTION__ );\
    return 0;\
}

const char * getLuaTypeName(const char * cppTypeName);
bool registerLuaTypeName(const char * cppTypeName, const char * luaTypeName);

template<typename T>
bool registerLuaTypeName(const char * luaTypeName)
{
    return registerLuaTypeName(typeid(T).name(), luaTypeName);
}

template <class T>
void tolua_object(lua_State* L, T* ret, const char* type)
{
    if(nullptr != ret)
    {
        if(type == nullptr)
        {
            type = getLuaTypeName(typeid(T).name());
        }
        
        if (std::is_base_of<ora::IReferenceCount, T>::value)
        {
            ora::ScriptMgr::instance()->pushScriptObject(ret, type);
        }
        else
        {
            tolua_pushusertype(L, (void*)ret, type);
        }
    }
    else
    {
        lua_pushnil(L);
    }
}

template <class T>
void tolua_object(lua_State* L, const T * ret, const char* type)
{
    tolua_object(L, const_cast<T*>(ret), type);
}

