
#include "helper.h"

#include <cassert>
#include <cstring>
#include <map>

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



#define CASE_NUMBER(T, TYPE) \
case T: \
{ \
    if(!lua_isnumber(L, index))\
    {\
        luaL_error(L, "argument %d: number type needed.", index);\
        return false;\
    }\
    TYPE *p = va_arg(ap, TYPE*);\
    *p = (TYPE)lua_tonumber(L, index);\
    ++index;\
    break;\
}


extern "C" {
    extern int lua_isusertype (lua_State* L, int lo, const char* type);
    extern int lua_isusertable (lua_State* L, int lo, const char* type);
}

static size_t parse_type_name(char * dst, const char * src)
{
    if(*src++ != '(')
    {
        return 0;
    }
    
    // parse the type name.
    const char *pch = src;
    while(*pch != ')' && (*pch != '\0'))
    {
        ++pch;
    }
    
    if(*pch != ')')
    {
        return 0;
    }
    
    size_t length = pch - src;
    strncpy(dst, src, length);
    dst[length] = '\0';
    return length;
}

bool lua_parse_args(lua_State *L, const char * format, ...)
{
    assert(format != nullptr);
    
    const char * pch = format;
    int index = 1;
    int iTop = lua_gettop(L);
    bool isdefault = false;
    bool ignoreRemain = false;
    
    va_list ap;
    va_start(ap, format);
    
    while(*pch != '\0')
    {
        if(*pch == '?')
        {
            ignoreRemain = true;
            break;
        }
        else if(*pch == '|')
        {
            isdefault = true;
            ++pch;
            continue;
        }
        
        if(iTop < index)
        {
            if(isdefault)
            {
                break;
            }
            
            luaL_error(L, "need more than %d args, %d was given.", index, iTop);
            return false;
        }
        
        switch (*pch)
        {
                CASE_NUMBER('c', char);
                CASE_NUMBER('C', unsigned char);
                CASE_NUMBER('h', short);
                CASE_NUMBER('H', unsigned short);
                CASE_NUMBER('i', int);
                CASE_NUMBER('I', unsigned int);
                CASE_NUMBER('l', long);
                CASE_NUMBER('L', unsigned long);
                CASE_NUMBER('v', long long);
                CASE_NUMBER('V', unsigned long long);
                CASE_NUMBER('f', float);
                CASE_NUMBER('d', double);
                
            case 'b': // boolean
            {
                if(!lua_isboolean(L, index))
                {
                    luaL_error(L, "argument %d: boolean type needed.", index);
                    return false;
                }
                
                bool *p = va_arg(ap, bool*);
                *p = lua_toboolean(L, index);
                ++index;
                break;
            }
            case 's': // string
            {
                if(!lua_isstring(L, index))
                {
                    luaL_error(L, "argument %d: string type needed.", index);
                    return false;
                }
                
                const char **p = va_arg(ap, const char **);
                *p = lua_tostring(L, index);
                ++index;
                break;
            }
            case 't': // table
            {
                if(!lua_istable(L, index))
                {
                    luaL_error(L, "argument %d: table type needed.", index);
                    return false;
                }
                
                int *p = va_arg(ap, int*);
                *p = tolua_tovalue(L, index, 0);
                ++index;
                break;
            }
            case 'u': // user data
            {        
                const void **p = va_arg(ap, const void**);
                
                if(lua_isuserdata(L, index))
                {        
                    *p = lua_touserdata(L, index);
                }
                else if(lua_isnil(L, index))
                {
                    *p = nullptr;
                }
                else
                {
                    luaL_error(L, "argument %d: usertdata type needed.", index);
                    return false;
                }
                ++index;
                break;
            }
            case 'U': // user table
            {
                ++pch;
                char typeName[256];
                size_t length = parse_type_name(typeName, pch);
                if(length == 0)
                {
                    luaL_error(L, "argument %d: failed to parse usertable typename", index);
                    return false;
                }
                pch += length + 1; //'(' + typename
                
                if(!lua_isusertable(L, index, typeName))
                {
                    luaL_error(L, "argument %d: usertable '%s' needed.", index, typeName);
                    return false;
                }
                
                int *p = va_arg(ap, int*);
                *p = tolua_tovalue(L, index, 0);
                
                ++index;
                break;
            }
            case 'T': // user type. T or T(RealTypeName)
            {
                const void **p = va_arg(ap, const void**);
                if(*(pch + 1) == '(')
                {
                    ++pch;
                    char typeName[256];
                    size_t length = parse_type_name(typeName, pch);
                    if(length == 0)
                    {
                        luaL_error(L, "argument %d: failed to parse usertype typename", index);
                        return false;
                    }
                    pch += length + 1; //'(' + typename
                    
                    if(lua_isnil(L, index))
                    {
                        *p = nullptr;
                    }
                    else if(lua_isusertype(L, index, typeName))
                    {        
                        *p = tolua_tousertype(L, index, nullptr);
                    }
                    else
                    {
                        luaL_error(L, "argument %d: usertype type '%s' needed.", index, typeName);
                        return false;
                    }
                }
                else if(lua_isnil(L, index))
                {
                    *p = nullptr;
                }
                else if(lua_isuserdata(L, index))
                {
                    *p = tolua_tousertype(L, index, nullptr);
                }
                else
                {
                    luaL_error(L, "argument %d: usertype type needed.", index);
                    return false;
                }
                ++index;
                break;
            }
            case 'F':
            {
                if(!lua_isfunction(L, index))
                {
                    luaL_error(L, "argument %d: function type needed.", index);
                    return false;
                }
                
                int *p = va_arg(ap, int*);
                *p = tolua_tovalue(L, index, 0);
                ++index;
                break;
            }
            case '*':
            {
                int *p = va_arg(ap, int*);
                *p = tolua_tovalue(L, index, 0);
                ++index;
                break;
            }
            case '.':
            {
                ++index;
                break;
            }
                
            default:
                luaL_error(L, "argument %d: Invalid format character '%c'", index, *pch);
                return false;
        }
        
        ++pch;
    }
    
    va_end(ap);
    
    index -= 1;
    if(iTop < index || (iTop > index && !ignoreRemain))
    {
        luaL_error(L, "needed %d arguments, but %d was given.", index, iTop);
        return false;
    }
    
    return true;
}

#undef CASE_NUMBER

static std::map<std::string, const char*> g_luaTypes;

bool registerLuaTypeName(const char * cppTypeName, const char * luaTypeName)
{
    auto ret = g_luaTypes.insert(std::pair<std::string, const char *>(cppTypeName, luaTypeName));
    if(!ret.second)
    {
        ORA_LOG_ERROR("the cpp type name '%s' has been exist.", cppTypeName);
    }
    return ret.second;
}

const char * getLuaTypeName(const char * cppTypeName)
{
    auto it = g_luaTypes.find(cppTypeName);
    if(it != g_luaTypes.end())
    {
        return it->second;
    }
    
    return nullptr;
}
