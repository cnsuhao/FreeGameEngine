
#include "script.h"
#include "helper.h"
#include "input_mgr.h"

#include "util/data_stream.h"
#include "util/assert_tool.h"
#include "util/log_tool.h"
#include "util/file_tool.h"
#include "util/timer.h"

#include <luaplus/LuaPlus.h>
#include <tolua++/include/tolua++.h>

extern "C"
{
#include "lualib.h"
#include "lauxlib.h"
#include "luasocket/src/luasocket.h"
    
int luaopen_lpeg (lua_State *L);
}

const char * FreeGameRefTable = "__freeg_r";
extern int register_all_frg(lua_State* L);
extern int register_frg_manual(lua_State *L);

#ifndef _RELEASE
void printCB(const char *msg)
{
    
}

static std::string s_msg;
#endif

extern FILE* ora_open(const char *file, const char *mode)
{
    return ora::FileSystemMgr::getInstance()->openRawFile(file, mode);
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
        , luaIdCounter_(0)
    {
        ORA_STACK_TRACE;
        InputMgr::initInstance();
    }
    
    ScriptMgr::~ScriptMgr()
    {
        ORA_STACK_TRACE;
        InputMgr::finiInstance();
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
        
        lua_pushcfunction(luaState_, luaopen_lpeg);
        lua_call(luaState_, 0, 0);
        
        // open lua socket
        luaL_findtable(luaState_, LUA_REGISTRYINDEX, "_LOADED", 1);
        lua_pushstring(luaState_, "socket.core");
        lua_pushcfunction(luaState_, luaopen_socket_core);
        lua_call(luaState_, 0, 1);
        lua_rawset(luaState_, -3);
        lua_pop(luaState_, 1); // pop the _LOADED tabel
        
        int iTop = lua_gettop(luaState_);
        
        // registe global functions.
        luaL_Reg _methods[] = {
            {"instance__index", instance__index},
            {"module__index", module__index},
            {0, 0},
        };
        luaL_register(luaState_, "_G", _methods);
        
        // open libs exported by tolua++
        iTop = lua_gettop(luaState_);
        lua_newtable(luaState_);
        lua_setglobal(luaState_, FreeGameRefTable);

        register_all_frg(luaState_);
        register_frg_manual(luaState_);
        
        InputMgr::instance()->init();
        
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

        InputMgr::instance()->fini();
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
        std::vector<IReferenceCount*> collector;
        collector.swap(collector_);
        
        for(IReferenceCount *p : collector)
        {
            p->release();
        }
    }

    void ScriptMgr::flushOutput()
    {
        flush_output();
    }
    
    void ScriptMgr::removeScriptObjecrt(long ID)
    {
        lua_getglobal(luaState_, FreeGameRefTable);
        lua_pushnil(luaState_);
        lua_rawseti(luaState_, -2, ID);
        lua_pop(luaState_, 1); // pop FreeGameRefTable
    }
    
    void ScriptMgr::pushScriptObject(IReferenceCount *p, const char * type)
    {
        if(nullptr == p)
        {
            lua_pushnil(luaState_);
        }
        else if(p->getScriptID() != 0)
        {
            lua_getglobal(luaState_, FreeGameRefTable);
            lua_rawgeti(luaState_, -1, p->getScriptID());
            lua_remove(luaState_, -2); // remove FreeGameRefTable
        }
        else
        {
            ASSERT_2(type != nullptr, "The lua type for p must not be null");
            tolua_pushusertype(luaState_, p, type);
            
            lua_getglobal(luaState_, FreeGameRefTable);
            lua_pushvalue(luaState_, -2);
            lua_rawseti(luaState_, -2, ++luaIdCounter_);
            lua_pop(luaState_, 1); // pop FreeGameRefTable

            p->setScriptID(luaIdCounter_);
            
            // add to collect list
            p->retain();
            collector_.push_back(p);
        }
    }

} // end namespace ora
