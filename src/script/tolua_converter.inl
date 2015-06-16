
template <typename T>
bool tolua_register_typename(const char * luaTypeName)
{
    return tolua_register_typename(typeid(T).name(), luaTypeName);
}

template <typename T>
bool luaval_to_object(lua_State* L, int lo, T** ret, const char* type)
{
    if(nullptr == L || lua_gettop(L) < lo)
        return false;
    
    if(type == nullptr) type = tolua_get_typename(typeid(T).name());
    
    if (!luaval_is_usertype(L, lo, type, 0))
        return false;
    
    *ret = static_cast<T*>(tolua_tousertype(L, lo, 0));
    return true;
}

template <typename T>
bool luaval_to_smartptr(lua_State* L, int lo, ora::SmartPtr<T>* out, const char* type)
{
    T * p;
    bool ret = luaval_to_object(L, lo, &p, type);
    if(ret)
    {
        *out = p;
    }
    return ret;
}

template <typename T>
void object_to_luaval(lua_State* L, T* ret, const char* type)
{
    if(nullptr != ret)
    {
        if(type == nullptr) type = tolua_get_typename(typeid(*ret).name());
        
        if (std::is_base_of<ora::IReferenceCount, T>::value)
        {
            tolua_push_ref_object((ora::IReferenceCount*)ret, type);
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

template <typename T>
void object_to_luaval(lua_State* L, const T * ret, const char* type)
{
    object_to_luaval(L, const_cast<T*>(ret), type);
}

template <typename T>
void object_to_luaval(lua_State* L, const T & ret, const char* type)
{
    T * p = new T(ret);
    object_to_luaval(L, p, type);
    
    if (!std::is_base_of<ora::IReferenceCount, T>::value)
    {
        tolua_register_gc(L, lua_gettop(L));
    }
}
