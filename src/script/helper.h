

#include <luaplus/LuaPlus.h>

//这两个函数放在引擎里实现，是为了不让lua调试器单步执行到。
int instance__index(lua_State * L);
int module__index(lua_State * L);

void registe_global_function(lua_State * L, const char * name, lua_CFunction fun);
