#include "lua_freegame_auto.hpp"
#include "freegame.h"
#include "script/tolua_converter.h"
#include "script/helper.h"


int lua_frg_IReferenceCount_release(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.IReferenceCount";
    const char * lua_fun_name = "frg.IReferenceCount:release";
    ora::IReferenceCount* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::IReferenceCount*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.IReferenceCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->release();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_IReferenceCount_release'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_IReferenceCount_retain(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.IReferenceCount";
    const char * lua_fun_name = "frg.IReferenceCount:retain";
    ora::IReferenceCount* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::IReferenceCount*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.IReferenceCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->retain();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_IReferenceCount_retain'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_IReferenceCount_refCount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.IReferenceCount";
    const char * lua_fun_name = "frg.IReferenceCount:refCount";
    ora::IReferenceCount* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::IReferenceCount*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.IReferenceCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        long ret = self->refCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_IReferenceCount_refCount'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_IReferenceCount_getScriptID(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.IReferenceCount";
    const char * lua_fun_name = "frg.IReferenceCount:getScriptID";
    ora::IReferenceCount* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::IReferenceCount*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.IReferenceCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        long ret = self->getScriptID();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_IReferenceCount_getScriptID'.",&tolua_err);
#endif
    return 0;
}
static int lua_frg_IReferenceCount_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (IReferenceCount)");
    return 0;
}

int lua_register_frg_IReferenceCount(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.IReferenceCount";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "IReferenceCount", lua_cls_name, "", nullptr);

    tolua_beginmodule(tolua_S,"IReferenceCount");
        tolua_function(tolua_S, "release", lua_frg_IReferenceCount_release);
        tolua_function(tolua_S, "retain", lua_frg_IReferenceCount_retain);
        tolua_function(tolua_S, "refCount", lua_frg_IReferenceCount_refCount);
        tolua_function(tolua_S, "getScriptID", lua_frg_IReferenceCount_getScriptID);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::IReferenceCount>(lua_cls_name);
    return 1;
}

int lua_frg_SceneNode_addChild(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:addChild";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::SceneNode> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->addChild(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_addChild'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_addAction(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:addAction";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::IAction> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->addAction(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_addAction'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_clearActions(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:clearActions";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->clearActions();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_clearActions'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_clearChildren(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:clearChildren";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->clearChildren();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_clearChildren'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_leaveWorld(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:leaveWorld";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->leaveWorld();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_leaveWorld'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_getName(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:getName";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        const std::string& ret = self->getName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_getName'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_topmost(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:topmost";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->topmost();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_topmost'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_init(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:init";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_init'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_delChildByName(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:delChildByName";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;
        ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->delChildByName(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_delChildByName'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_getChild(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:getChild";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;
        ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        ora::SceneNode* ret = self->getChild(arg0);
        object_to_luaval(tolua_S, ret, "frg.SceneNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_getChild'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_destroy(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:destroy";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->destroy();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_destroy'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_enterWorld(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:enterWorld";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->enterWorld();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_enterWorld'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_getWorldMatrix(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:getWorldMatrix";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        const ora::Matrix& ret = self->getWorldMatrix();
        matrix_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_getWorldMatrix'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_setName(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:setName";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;
        ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->setName(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_setName'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_removeFromParent(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:removeFromParent";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->removeFromParent();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_removeFromParent'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_setWorldMatrix(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:setWorldMatrix";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::Matrix arg0;
        ok &= luaval_to_matrix(tolua_S, 2, &arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->setWorldMatrix(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_setWorldMatrix'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_removeAction(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:removeAction";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::IAction> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->removeAction(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_removeAction'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_isInWorld(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:isInWorld";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->isInWorld();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_isInWorld'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_setColor(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:setColor";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::Vector4 arg0;
        ok &= luaval_to_vec4(tolua_S, 2, &arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->setColor(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_setColor'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_getRoot(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:getRoot";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SceneNode* ret = self->getRoot();
        object_to_luaval(tolua_S, ret, "frg.SceneNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_getRoot'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_getParent(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:getParent";
    ora::SceneNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SceneNode* ret = self->getParent();
        object_to_luaval(tolua_S, ret, "frg.SceneNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_getParent'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_delChild(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:delChild";
    ora::SceneNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::SceneNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.SceneNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::SceneNode> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.SceneNode'", nullptr);
            return 0;
        }
        self->delChild(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_delChild'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_create(lua_State* tolua_S)
{
    const char * lua_fun_name = "frg.SceneNode:create";
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif

#ifndef TOLUA_RELEASE
    if (!tolua_isusertable(tolua_S,1,"frg.SceneNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_frg_SceneNode_create'", nullptr);
            return 0;
        }
        ora::SceneNode* ret = ora::SceneNode::create();
        object_to_luaval(tolua_S, ret, "frg.SceneNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", lua_fun_name, argc, 0);
    return 0;
#ifndef TOLUA_RELEASE
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_create'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_SceneNode_constructor(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.SceneNode";
    const char * lua_fun_name = "frg.SceneNode:SceneNode";
    ora::SceneNode* self = nullptr;
    int argc = 0;


    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self = new ora::SceneNode();
        object_to_luaval(tolua_S, self, lua_cls_name);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
    tolua_error(tolua_S,"#ferror in function 'lua_frg_SceneNode_constructor'.",&tolua_err);
#endif
    return 0;
}

static int lua_frg_SceneNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (SceneNode)");
    return 0;
}

int lua_register_frg_SceneNode(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.SceneNode";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "SceneNode", lua_cls_name, "frg.IReferenceCount",nullptr);

    tolua_beginmodule(tolua_S,"SceneNode");
        tolua_function(tolua_S, "new", lua_frg_SceneNode_constructor);
        tolua_function(tolua_S, "addChild", lua_frg_SceneNode_addChild);
        tolua_function(tolua_S, "addAction", lua_frg_SceneNode_addAction);
        tolua_function(tolua_S, "clearActions", lua_frg_SceneNode_clearActions);
        tolua_function(tolua_S, "clearChildren", lua_frg_SceneNode_clearChildren);
        tolua_function(tolua_S, "leaveWorld", lua_frg_SceneNode_leaveWorld);
        tolua_function(tolua_S, "getName", lua_frg_SceneNode_getName);
        tolua_function(tolua_S, "topmost", lua_frg_SceneNode_topmost);
        tolua_function(tolua_S, "init", lua_frg_SceneNode_init);
        tolua_function(tolua_S, "delChildByName", lua_frg_SceneNode_delChildByName);
        tolua_function(tolua_S, "getChild", lua_frg_SceneNode_getChild);
        tolua_function(tolua_S, "destroy", lua_frg_SceneNode_destroy);
        tolua_function(tolua_S, "enterWorld", lua_frg_SceneNode_enterWorld);
        tolua_function(tolua_S, "getWorldMatrix", lua_frg_SceneNode_getWorldMatrix);
        tolua_function(tolua_S, "setName", lua_frg_SceneNode_setName);
        tolua_function(tolua_S, "removeFromParent", lua_frg_SceneNode_removeFromParent);
        tolua_function(tolua_S, "setWorldMatrix", lua_frg_SceneNode_setWorldMatrix);
        tolua_function(tolua_S, "removeAction", lua_frg_SceneNode_removeAction);
        tolua_function(tolua_S, "isInWorld", lua_frg_SceneNode_isInWorld);
        tolua_function(tolua_S, "setColor", lua_frg_SceneNode_setColor);
        tolua_function(tolua_S, "getRoot", lua_frg_SceneNode_getRoot);
        tolua_function(tolua_S, "getParent", lua_frg_SceneNode_getParent);
        tolua_function(tolua_S, "delChild", lua_frg_SceneNode_delChild);
        tolua_function(tolua_S, "create", lua_frg_SceneNode_create);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::SceneNode>(lua_cls_name);
    return 1;
}

int lua_frg_Camera_getView(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:getView";
    ora::Camera* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        const ora::Matrix& ret = self->getView();
        matrix_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_getView'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_doRotation(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:doRotation";
    ora::Camera* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        ora::Vector3 arg0;
        double arg1;
        ok &= luaval_to_vec3(tolua_S, 2, &arg0, lua_fun_name);
        ok &= luaval_to_number(tolua_S, 3,&arg1, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.Camera'", nullptr);
            return 0;
        }
        self->doRotation(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 2);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_doRotation'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_getInvView(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:getInvView";
    ora::Camera* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        const ora::Matrix& ret = self->getInvView();
        matrix_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_getInvView'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_doTranslation(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:doTranslation";
    ora::Camera* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        ora::Vector3 arg0;
        double arg1;
        ok &= luaval_to_vec3(tolua_S, 2, &arg0, lua_fun_name);
        ok &= luaval_to_number(tolua_S, 3,&arg1, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.Camera'", nullptr);
            return 0;
        }
        self->doTranslation(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 2);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_doTranslation'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_active(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:active";
    ora::Camera* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->active();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_active'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_setControled(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:setControled";
    ora::Camera* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.Camera'", nullptr);
            return 0;
        }
        self->setControled(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_setControled'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_getControled(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:getControled";
    ora::Camera* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::Camera*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.Camera'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->getControled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_getControled'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_Camera_constructor(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.Camera";
    const char * lua_fun_name = "frg.Camera:Camera";
    ora::Camera* self = nullptr;
    int argc = 0;


    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self = new ora::Camera();
        object_to_luaval(tolua_S, self, lua_cls_name);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
    tolua_error(tolua_S,"#ferror in function 'lua_frg_Camera_constructor'.",&tolua_err);
#endif
    return 0;
}

static int lua_frg_Camera_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Camera)");
    return 0;
}

int lua_register_frg_Camera(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.Camera";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "Camera", lua_cls_name, "frg.SceneNode",nullptr);

    tolua_beginmodule(tolua_S,"Camera");
        tolua_function(tolua_S, "new", lua_frg_Camera_constructor);
        tolua_function(tolua_S, "getView", lua_frg_Camera_getView);
        tolua_function(tolua_S, "doRotation", lua_frg_Camera_doRotation);
        tolua_function(tolua_S, "getInvView", lua_frg_Camera_getInvView);
        tolua_function(tolua_S, "doTranslation", lua_frg_Camera_doTranslation);
        tolua_function(tolua_S, "active", lua_frg_Camera_active);
        tolua_function(tolua_S, "setControled", lua_frg_Camera_setControled);
        tolua_function(tolua_S, "getControled", lua_frg_Camera_getControled);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::Camera>(lua_cls_name);
    return 1;
}

int lua_frg_MeshNode_load(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:load";
    ora::MeshNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::MeshNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.MeshNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;
        ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.MeshNode'", nullptr);
            return 0;
        }
        bool ret = self->load(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_load'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_getWorldAABB(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:getWorldAABB";
    ora::MeshNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::MeshNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.MeshNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::AABB ret = self->getWorldAABB();
        object_to_luaval(tolua_S, ret, "frg.AABB");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_getWorldAABB'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_getMesh(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:getMesh";
    ora::MeshNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::MeshNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.MeshNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SmartPtr<ora::Mesh> ret = self->getMesh();
        object_to_luaval(tolua_S, ret.get(), nullptr);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_getMesh'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_setMesh(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:setMesh";
    ora::MeshNode* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::MeshNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.MeshNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::Mesh> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.MeshNode'", nullptr);
            return 0;
        }
        self->setMesh(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_setMesh'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_getRigid(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:getRigid";
    ora::MeshNode* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::MeshNode*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.MeshNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SmartPtr<ora::IRigidBody> ret = self->getRigid();
        object_to_luaval(tolua_S, ret.get(), nullptr);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_getRigid'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_create(lua_State* tolua_S)
{
    const char * lua_fun_name = "frg.MeshNode:create";
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif

#ifndef TOLUA_RELEASE
    if (!tolua_isusertable(tolua_S,1,"frg.MeshNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_frg_MeshNode_create'", nullptr);
            return 0;
        }
        ora::MeshNode* ret = ora::MeshNode::create(arg0);
        object_to_luaval(tolua_S, ret, "frg.MeshNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", lua_fun_name, argc, 1);
    return 0;
#ifndef TOLUA_RELEASE
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_create'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_MeshNode_constructor(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.MeshNode";
    const char * lua_fun_name = "frg.MeshNode:MeshNode";
    int argc = 0;
    ora::MeshNode* self = nullptr;
    bool ok  = true;
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif

    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_string(tolua_S, 2,&arg0, lua_fun_name);

            if (!ok) { break; }
            self = new ora::MeshNode(arg0);
            object_to_luaval(tolua_S, self, lua_cls_name);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            self = new ora::MeshNode();
            object_to_luaval(tolua_S, self, lua_cls_name);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
    tolua_error(tolua_S,"#ferror in function 'lua_frg_MeshNode_constructor'.",&tolua_err);
#endif
    return 0;
}

static int lua_frg_MeshNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (MeshNode)");
    return 0;
}

int lua_register_frg_MeshNode(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.MeshNode";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "MeshNode", lua_cls_name, "frg.SceneNode",nullptr);

    tolua_beginmodule(tolua_S,"MeshNode");
        tolua_function(tolua_S, "new", lua_frg_MeshNode_constructor);
        tolua_function(tolua_S, "load", lua_frg_MeshNode_load);
        tolua_function(tolua_S, "getWorldAABB", lua_frg_MeshNode_getWorldAABB);
        tolua_function(tolua_S, "getMesh", lua_frg_MeshNode_getMesh);
        tolua_function(tolua_S, "setMesh", lua_frg_MeshNode_setMesh);
        tolua_function(tolua_S, "getRigid", lua_frg_MeshNode_getRigid);
        tolua_function(tolua_S, "create", lua_frg_MeshNode_create);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::MeshNode>(lua_cls_name);
    return 1;
}

int lua_frg_World_setLineMode(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:setLineMode";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setLineMode(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_setLineMode'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getCamera(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getCamera";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SmartPtr<ora::Camera> ret = self->getCamera();
        object_to_luaval(tolua_S, ret.get(), nullptr);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getCamera'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getRoot(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getRoot";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::SceneNode* ret = self->getRoot();
        object_to_luaval(tolua_S, ret, "frg.SceneNode");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getRoot'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_worldPosToScreen(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:worldPosToScreen";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::Vector3 arg0;
        ok &= luaval_to_vec3(tolua_S, 2, &arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        ora::Vector2 ret = self->worldPosToScreen(arg0);
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_worldPosToScreen'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getTimeScale(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getTimeScale";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        double ret = self->getTimeScale();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getTimeScale'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getTotalFrame(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getTotalFrame";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        unsigned int ret = self->getTotalFrame();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getTotalFrame'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getPause(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getPause";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->getPause();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getPause'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_screenPosToWorld(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:screenPosToWorld";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::Vector2 arg0;
        ok &= luaval_to_vec2(tolua_S, 2, &arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        ora::Vector3 ret = self->screenPosToWorld(arg0);
        vec3_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_screenPosToWorld'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getLineMode(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getLineMode";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->getLineMode();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getLineMode'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_setPause(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:setPause";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;
        ok &= luaval_to_boolean(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setPause(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_setPause'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getViewSize(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getViewSize";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        ora::Vector2 ret = self->getViewSize();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getViewSize'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_fini(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:fini";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->fini();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_fini'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_init(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:init";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        bool ret = self->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_init'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_setTimeScale(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:setTimeScale";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setTimeScale(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_setTimeScale'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getFps(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getFps";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        double ret = self->getFps();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getFps'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_worldPosToProj(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:worldPosToProj";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::Vector3 arg0;
        ok &= luaval_to_vec3(tolua_S, 2, &arg0, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        ora::Vector3 ret = self->worldPosToProj(arg0);
        vec3_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_worldPosToProj'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getElapse(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getElapse";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        double ret = self->getElapse();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getElapse'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_getElapse_ms(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:getElapse_ms";
    ora::World* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        unsigned int ret = self->getElapse_ms();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_getElapse_ms'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_setCamera(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:setCamera";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::Camera> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setCamera(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_setCamera'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_World_setRoot(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.World";
    const char * lua_fun_name = "frg.World:setRoot";
    ora::World* self = nullptr;
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::World*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.World'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ora::SmartPtr<ora::SceneNode> arg0;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setRoot(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        ora::SmartPtr<ora::SceneNode> arg0;
        bool arg1;
        ok &= luaval_to_smartptr(tolua_S, 2, &arg0, nullptr);
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, lua_fun_name);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'frg.World'", nullptr);
            return 0;
        }
        self->setRoot(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 1);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_World_setRoot'.",&tolua_err);
#endif
    return 0;
}
static int lua_frg_World_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (World)");
    return 0;
}

int lua_register_frg_World(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.World";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "World", lua_cls_name, "frg.Object",nullptr);

    tolua_beginmodule(tolua_S,"World");
        tolua_function(tolua_S, "setLineMode", lua_frg_World_setLineMode);
        tolua_function(tolua_S, "getCamera", lua_frg_World_getCamera);
        tolua_function(tolua_S, "getRoot", lua_frg_World_getRoot);
        tolua_function(tolua_S, "worldPosToScreen", lua_frg_World_worldPosToScreen);
        tolua_function(tolua_S, "getTimeScale", lua_frg_World_getTimeScale);
        tolua_function(tolua_S, "getTotalFrame", lua_frg_World_getTotalFrame);
        tolua_function(tolua_S, "getPause", lua_frg_World_getPause);
        tolua_function(tolua_S, "screenPosToWorld", lua_frg_World_screenPosToWorld);
        tolua_function(tolua_S, "getLineMode", lua_frg_World_getLineMode);
        tolua_function(tolua_S, "setPause", lua_frg_World_setPause);
        tolua_function(tolua_S, "getViewSize", lua_frg_World_getViewSize);
        tolua_function(tolua_S, "fini", lua_frg_World_fini);
        tolua_function(tolua_S, "init", lua_frg_World_init);
        tolua_function(tolua_S, "setTimeScale", lua_frg_World_setTimeScale);
        tolua_function(tolua_S, "getFps", lua_frg_World_getFps);
        tolua_function(tolua_S, "worldPosToProj", lua_frg_World_worldPosToProj);
        tolua_function(tolua_S, "getElapse", lua_frg_World_getElapse);
        tolua_function(tolua_S, "getElapse_ms", lua_frg_World_getElapse_ms);
        tolua_function(tolua_S, "setCamera", lua_frg_World_setCamera);
        tolua_function(tolua_S, "setRoot", lua_frg_World_setRoot);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::World>(lua_cls_name);
    return 1;
}

int lua_frg_RootScene_renderAxis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.RootScene";
    const char * lua_fun_name = "frg.RootScene:renderAxis";
    ora::RootScene* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::RootScene*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.RootScene'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->renderAxis();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_RootScene_renderAxis'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_RootScene_testRendertarget(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.RootScene";
    const char * lua_fun_name = "frg.RootScene:testRendertarget";
    ora::RootScene* self = nullptr;
    int argc = 0;

#ifndef TOLUA_RELEASE
    if (!tolua_isusertype(tolua_S,1,lua_cls_name,0,&tolua_err)) goto tolua_lerror;
#endif
    self = (ora::RootScene*)tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
    if (!self) 
    {
        tolua_error(tolua_S,"invalid 'self' in function 'frg.RootScene'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self->testRendertarget();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_RootScene_testRendertarget'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_RootScene_create(lua_State* tolua_S)
{
    const char * lua_fun_name = "frg.RootScene:create";
    int argc = 0;
    bool ok  = true;

#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif

#ifndef TOLUA_RELEASE
    if (!tolua_isusertable(tolua_S,1,"frg.RootScene",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_frg_RootScene_create'", nullptr);
            return 0;
        }
        ora::RootScene* ret = ora::RootScene::create();
        object_to_luaval(tolua_S, ret, "frg.RootScene");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", lua_fun_name, argc, 0);
    return 0;
#ifndef TOLUA_RELEASE
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_frg_RootScene_create'.",&tolua_err);
#endif
    return 0;
}
int lua_frg_RootScene_constructor(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
#endif
    const char * lua_cls_name = "frg.RootScene";
    const char * lua_fun_name = "frg.RootScene:RootScene";
    ora::RootScene* self = nullptr;
    int argc = 0;


    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        self = new ora::RootScene();
        object_to_luaval(tolua_S, self, lua_cls_name);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", lua_fun_name, argc, 0);
    return 0;

#ifndef TOLUA_RELEASE
    tolua_error(tolua_S,"#ferror in function 'lua_frg_RootScene_constructor'.",&tolua_err);
#endif
    return 0;
}

static int lua_frg_RootScene_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (RootScene)");
    return 0;
}

int lua_register_frg_RootScene(lua_State* tolua_S)
{
    const char * lua_cls_name = "frg.RootScene";
    tolua_usertype(tolua_S, lua_cls_name);
    tolua_cclass(tolua_S, "RootScene", lua_cls_name, "frg.SceneNode",nullptr);

    tolua_beginmodule(tolua_S,"RootScene");
        tolua_function(tolua_S, "new", lua_frg_RootScene_constructor);
        tolua_function(tolua_S, "renderAxis", lua_frg_RootScene_renderAxis);
        tolua_function(tolua_S, "testRendertarget", lua_frg_RootScene_testRendertarget);
        tolua_function(tolua_S, "create", lua_frg_RootScene_create);
    tolua_endmodule(tolua_S);
    tolua_register_typename<ora::RootScene>(lua_cls_name);
    return 1;
}
TOLUA_API int register_all_frg(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"frg",0);
	tolua_beginmodule(tolua_S,"frg");

	lua_register_frg_IReferenceCount(tolua_S);
	lua_register_frg_SceneNode(tolua_S);
	lua_register_frg_MeshNode(tolua_S);
	lua_register_frg_RootScene(tolua_S);
	lua_register_frg_Camera(tolua_S);
	lua_register_frg_World(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

