
#include "script.h"

#include "util/data_stream.h"
#include "util/assert_tool.h"
#include "util/log_tool.h"
#include "util/file_tool.h"
#include "util/timer.h"

#include "helper.h"
#include <tolua++/include/tolua++.h>

extern "C"
{
#include "lualib.h"
#include "lauxlib.h"
#include "luasocket/src/luasocket.h"
    
int luaopen_lpeg (lua_State *L);
}

#ifndef _RELEASE
void printCB(const char *msg)
{
    
}

static std::string s_msg;
#endif

extern FILE* ora_open(const char *file, const char *mode)
{
    return nullptr;
}

inline void flush_output()
{
#ifndef _RELEASE
    if (s_msg.empty())
        return;

    ora::ora_log(ORA_LOG_LVL_DEBUG, s_msg.c_str());

    if (printCB)
        printCB(s_msg.c_str());

    s_msg.clear();
#endif
}

extern void  ora_print(const char * msg)
{
#ifndef _RELEASE
    if(strcmp(msg, "\n") == 0)
    {
        flush_output();
    }
    else
    {
        s_msg += msg;
    }
#endif
}

extern void ora_print_w(const wchar_t * msg)
{
#ifndef _RELEASE
    
#endif
}

namespace LuaPlus
{
    void ErrorMessage(const char * message)
    {
        ora::ora_log_directly(ORA_LOG_LVL_ERROR, message);
#ifndef _RELEASE
		if (printCB)
			printCB(message);
#endif
    }
}

namespace ora
{
    IMPLEMENT_SINGLETON(ScriptMgr);
    
    ScriptMgr::ScriptMgr()
        : luaPlus_(nullptr)
        , luaState_(nullptr)
    {
        ORA_STACK_TRACE;
    }
    
    ScriptMgr::~ScriptMgr()
    {
        ORA_STACK_TRACE;
    }

    bool ScriptMgr::init()
    {
        ORA_STACK_TRACE;
        
        ASSERT_1(luaPlus_ == nullptr);
        
        std::string scriptEntry = g_sysConfig->readString("app/script_entry");
        if (scriptEntry.empty())
        {
            ORA_LOG_ERROR("ScriptMgr::init - Failed to find key 'app/script' in sysconfig!");
            return false;
        }
        
        luaPlus_ = LuaPlus::LuaState::Create();
        if (luaPlus_ == nullptr)
        {
            ORA_LOG_ERROR("ScriptMgr::init - Failed to create lua state!");
            return false;
        }
        
        luaState_ = luaPlus_->GetCState();


        // open basic lua libs.
        luaL_openlibs(luaState_);
        luaopen_lpeg(luaState_);


        // open lua socket
        luaL_findtable(luaState_, LUA_REGISTRYINDEX, "_LOADED", 1);
        lua_pushstring(luaState_, "socket.core");
        lua_pushcfunction(luaState_, luaopen_socket_core);
        lua_call(luaState_, 0, 1);
        lua_rawset(luaState_, -3);
        lua_pop(luaState_, 1); // pop the _LOADED tabel
        

        // open libs exported by tolua++


        // registe global functions.
        luaL_Reg _methods[] = {
            {"instance__index", instance__index},
            {"module__index", module__index},
            {0, 0},
        };
        for(luaL_Reg *p = _methods; p->name != 0; ++p)
            registe_global_function(luaState_, p->name, p->func);

        if(!loadScript(scriptEntry))
            return false;
        
        return true;
    }
    
    void ScriptMgr::fini()
    {
        ORA_STACK_TRACE;

        gcCollect();

        if(luaPlus_ != nullptr)
        {
            luaState_ = nullptr;
            LuaPlus::LuaState::Destroy(luaPlus_);
        }

    }
    
    lua_State * ScriptMgr::getLuaState()
    {
        return luaState_;
    }

    LuaPlus::LuaState * ScriptMgr::getLuaPlus()
    {
        return luaPlus_;
    }
    
    bool ScriptMgr::loadScript(const std::string & scriptEntry)
    {
        ORA_STACK_TRACE;

        if(luaPlus_->DoFile(scriptEntry.c_str()) != 0)
        {
            ORA_LOG_ERROR("ScriptMgr::loadScript - Failed to load script file '%s'\nTraceback %s",
                          scriptEntry.c_str(), lua_tostring(luaState_, -1));
            return false;
        }
        
        LuaPlus::LuaObject fun = luaPlus_->GetGlobal("init");
        if(!fun.IsFunction())
        {
            ORA_LOG_ERROR("ScriptMgr::loadScript - Can't find the entry function 'init'.");
            return false;
        }
        
        LuaPlus::LuaFunction<bool> initFun(fun);
        
        return initFun(33, "hello lua!");
    }

    void ScriptMgr::gcCollect()
    {
        ORA_LOG_DEBUG("gc collect");
        lua_gc(luaState_, LUA_GCCOLLECT, 0);
    }

    void ScriptMgr::tick(float elapse)
    {
    }

    void ScriptMgr::flushOutput()
    {
        flush_output();
    }

} // end namespace ora
