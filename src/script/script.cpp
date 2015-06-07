#include "duplo/pch.hpp"
#include "script.h"
#include "util/data_stream.h"
#include "util/assert_tool.h"
#include "util/log_tool.h"
#include "util/file_tool.h"
#include "util/timer.h"
#include "resmgr/bwresource.hpp"
#include "resmgr/multi_file_system.hpp"

#include "helper.h"
#include <toluaplus/include/tolua++.h>

#include "duplo/py_effect.hpp"

#include "input_mgr.h"

extern "C"
{
#include "lualib.h"
#include "lauxlib.h"
#include "luasocket/src/luasocket.h"
}

TOLUA_API int  tolua_engine_open(lua_State* tolua_S);
TOLUA_API int  tolua_gui_open (lua_State* tolua_S);
TOLUA_API int  tolua_util_open (lua_State* tolua_S);

extern "C" int luaopen_lpeg (lua_State *L);
int pick_model(lua_State *L);
int pick_gizmo(lua_State *L);


#ifndef _RELEASE
typedef std::function<void(const char *)> PrintHandle;
extern PrintHandle printCB;
static std::string s_msg;
#endif

extern FILE* ora_open(const char *file, const char *mode)
{
    return BWResource::instance().fileSystem()->posixFileOpen(file, mode);
}

inline void flush_output()
{
#ifndef _RELEASE
    if (s_msg.empty())
        return;

    ora::ora_log_directly(ORA_LOG_LVL_DEBUG, s_msg.c_str());

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
    std::string message;
    bw_wtoutf8(msg, message);

    ora_print(message.c_str());
#endif
}

// used by rldb lua debugger.
extern "C" char * rldb_full_path(char * absPath, const char * relPath, size_t maxLen)
{
    std::string path(relPath);

    if (IFileSystem::FT_NOT_FOUND == BWResource::resolveToAbsolutePath(path))
        return NULL;

    ora::canonicalizePath(path);

    if (path.length() + 1 <= maxLen)
    {
        strncpy(absPath, path.c_str(), path.length() + 1);
        return absPath;
    }
    return NULL;
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
        tolua_util_open(luaState_);
        tolua_engine_open(luaState_);
        tolua_gui_open(luaState_);


        // registe global functions.
        luaL_Reg _methods[] = {
            {"pick_model", pick_model},
            {"pick_gizmo", pick_gizmo},
            {"instance__index", instance__index},
            {"module__index", module__index},
            {0, 0},
        };
        for(luaL_Reg *p = _methods; p->name != 0; ++p)
            registe_global_function(luaState_, p->name, p->func);

        if(!loadScript(scriptEntry))
            return false;
        
        InputMgr::initInstance();
        if (!InputMgr::instance()->init())
        {
            ORA_LOG_ERROR("Failed to init input system!");
            return false;
        }
        return true;
    }
    
    void ScriptMgr::fini()
    {
        ORA_STACK_TRACE;

        gcCollect();

        if(InputMgr::hasInstance())
        {
            InputMgr::instance()->fini();
            InputMgr::instance()->finiInstance();
        }

        PyEffect::finialise();

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
        PyEffect::updateAll();
    }

    void ScriptMgr::flushOutput()
    {
        flush_output();
    }

} // end namespace ora
