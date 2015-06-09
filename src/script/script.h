#ifndef _H_ORANGE_SCRIPT_H_
#define _H_ORANGE_SCRIPT_H_

#include "util/smartptr.h"
#include "util/singleton.h"

#include <string>
#include <vector>

//pre definations

struct lua_State;

namespace LuaPlus
{
    class LuaState;
}

namespace ora
{

    class ScriptMgr : public Singleton<ScriptMgr>
    {
    public:
        ScriptMgr();
        ~ScriptMgr();
        
        bool init();
        void fini();
        
        lua_State * getLuaState();
        LuaPlus::LuaState * getLuaPlus();
        
        void flushOutput();

        void gcCollect();

        void tick(float elapse);
        
        void removeScriptObjecrt(long ID);
        void pushScriptObject(IReferenceCount *p, const char * type);
        
    private:
        bool loadScript(const std::string & scriptEntry);
        
    private:
        LuaPlus::LuaState * luaPlus_;
        lua_State *         luaState_;
        long                luaIdCounter_;
        std::vector<IReferenceCount*> collector_;
    };
    
} // end namespace ora

#endif //_H_ORANGE_SCRIPT_H_
