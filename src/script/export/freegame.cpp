
#include "script/helper.h"

#include "script/root_scene.h"
#include "core/world.h"

using namespace ora;

////////////////////////////////////////////////////////////////////////////////
LUA_FUN_HEADER(lua_freeg_SceneNode_create)
{
    int ut;
    LUA_PARSE_FUN(L, "U(freeg.SceneNode)", &ut);
    
    SceneNode *p = new SceneNode();
    if(!p->init())
    {
        delete p;
        p = nullptr;
    }
    tolua_object(L, p, "freeg.SceneNode");
    return 1;
}

LUA_FUN_HEADER(lua_freeg_SceneNode_addChild)
{
    SceneNode *self;
    SceneNode *child;
    LUA_PARSE_METHOD(L, "T(freeg.SceneNode)T(freeg.SceneNode)", &self, &child);
    
    self->addChild(child);
    return 0;
}

LUA_FUN_HEADER(lua_freeg_SceneNode_delChild)
{
    SceneNode *self;
    SceneNode *child;
    LUA_PARSE_METHOD(L, "T(freeg.SceneNode)T(freeg.SceneNode)", &self, &child);
    
    self->delChild(child);
    return 0;
}

LUA_FUN_HEADER(lua_freeg_SceneNode_delChildByName)
{
    SceneNode *self;
    const char * name;
    LUA_PARSE_METHOD(L, "T(freeg.SceneNode)s", &self, &name);
    
    self->delChildByName(name);
    return 0;
}

int lua_freeg_register_SceneNode(lua_State *L)
{
    tolua_usertype(L, "freeg.SceneNode");
    tolua_cclass(L, "SceneNode", "freeg.SceneNode", "", nullptr);
    
    tolua_beginmodule(L, "SceneNode");
    {
        tolua_function(L, "create", lua_freeg_SceneNode_create);
        tolua_function(L, "addChild", lua_freeg_SceneNode_addChild);
        tolua_function(L, "delChild", lua_freeg_SceneNode_delChild);
        tolua_function(L, "delChildByName", lua_freeg_SceneNode_delChildByName);
    }
    tolua_endmodule(L);
    
    registerLuaTypeName<SceneNode>("freeg.SceneNode");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

LUA_FUN_HEADER(lua_freeg_RootScene_create)
{
    int ut;
    LUA_PARSE_FUN(L, "U(freeg.RootScene)", &ut);
    
    RootScene *p = new RootScene();
    if(!p->init())
    {
        delete p;
        p = nullptr;
    }
    tolua_object(L, p, "freeg.RootScene");
    return 1;
}

int lua_freeg_register_RootScene(lua_State *L)
{
    tolua_usertype(L, "freeg.RootScene");
    tolua_cclass(L, "RootScene", "freeg.RootScene", "freeg.SceneNode", nullptr);
    
    tolua_beginmodule(L, "RootScene");
    {
        tolua_function(L, "create", lua_freeg_RootScene_create);
    }
    tolua_endmodule(L);
    
    registerLuaTypeName<RootScene>("freeg.RootScene");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

LUA_FUN_HEADER(lua_freeg_MeshNode_create)
{
    int ut;
    const char * path;
    LUA_PARSE_FUN(L, "U(freeg.MeshNode)s", &ut, &path);
    
    MeshNode *p = MeshNode::create(path);
    tolua_object(L, p, "freeg.MeshNode");
    return 1;
}

int lua_freeg_register_MeshScene(lua_State *L)
{
    tolua_usertype(L, "freeg.MeshNode");
    tolua_cclass(L, "MeshNode", "freeg.MeshNode", "freeg.SceneNode", nullptr);
    
    tolua_beginmodule(L, "MeshNode");
    {
        tolua_function(L, "create", lua_freeg_MeshNode_create);
    }
    tolua_endmodule(L);
    
    registerLuaTypeName<MeshNode>("freeg.Mesh");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
LUA_FUN_HEADER(lua_freeg_World_instance)
{
    int ut;
    LUA_PARSE_FUN(L, "U(freeg.World)", &ut);
    
    tolua_object(L, World::instance(), "freeg.World");
    return 1;
}

LUA_FUN_HEADER(lua_freeg_World_setRoot)
{
    World *self;
    SceneNode *p;
    LUA_PARSE_METHOD(L, "T(freeg.World)T(freeg.SceneNode)", &self, &p);
    
    self->setRoot(p);
    return 0;
}

LUA_FUN_HEADER(lua_freeg_World_getRoot)
{
    World *self;
    LUA_PARSE_METHOD(L, "T(freeg.World)", &self);
    
    tolua_object(L, self->getRoot(), nullptr);
    return 0;
}

int lua_freeg_register_World(lua_State *L)
{
    tolua_usertype(L, "freeg.World");
    tolua_cclass(L, "World", "freeg.World", "", nullptr);
    
    tolua_beginmodule(L, "World");
    {
        tolua_function(L, "instance", lua_freeg_World_instance);
        tolua_function(L, "getRoot", lua_freeg_World_getRoot);
        tolua_function(L, "setRoot", lua_freeg_World_setRoot);
    }
    tolua_endmodule(L);
    
    registerLuaTypeName<World>("freeg.World");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
int register_freegame(lua_State *L)
{
    tolua_open(L);
    
    tolua_module(L, "freeg", 0);
    tolua_beginmodule(L, "freeg");
    {
        lua_freeg_register_SceneNode(L);
        lua_freeg_register_RootScene(L);
        lua_freeg_register_MeshScene(L);
        lua_freeg_register_World(L);
    }
    tolua_endmodule(L);
    return 0;
}
