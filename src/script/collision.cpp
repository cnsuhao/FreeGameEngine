#include "duplo/pch.hpp"
#include "collision.h"
#include "duplo/pymodel.hpp"
#include "core/world.h"
#include "graphics/aabb.h"
#include "core/gizmo.h"

#include <luaplus/LuaPlus.h>

int pick_model(lua_State *L)
{
    if(!lua_istable(L, 1))
        return luaL_argerror(L, 1, "models table needed.");
    if(!lua_isnumber(L, 2))
        return luaL_argerror(L, 2, "float value 'x' needed.");
    if(!lua_isnumber(L, 3))
        return luaL_argerror(L, 3, "float value 'y' needed.");

    LuaPlus::LuaObject entities(L, 1);
    Vector2 screenPos((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));

    Vector3 rayDir = ora::World::instance()->screenPosToWorld(screenPos);
    Vector3 rayOrg = ora::World::instance()->getCamera()->getPosition();

    float neastDist = FloatMax;
    int entityID = 0;

    for(LuaPlus::LuaTableIterator it(entities); it; ++it)
    {
        LuaPlus::LuaObject model = it.GetValue();
        if(!model.IsUserData())
            continue;

        PyModel * pModel = (PyModel *)model.GetUserType();
        if(!pModel)
            continue;
		
		Matrix mat = pModel->getWorldMatrix();
		Matrix matInv;
		matInv.invert(mat);
		
		Vector3 rayDirInGizmoSpace = matInv.applyVector(rayDir);
		Vector3 rayOrgInGizmoSpace = matInv.applyPoint(rayOrg);

        BoundingBox bb;
        pModel->localBoundingBox(bb, true);

	    float dist;
        ora::AABB aabb(bb);
		if (!aabb.intersectsRay(rayOrgInGizmoSpace, rayDirInGizmoSpace, &dist))
			continue;

		Vector3 point = rayOrgInGizmoSpace + rayDirInGizmoSpace * dist;
		mat.applyPoint(point);

		dist = (point - rayOrg).length();

        if( dist < neastDist)
        {
            neastDist = dist;
            entityID = it.GetKey().GetInteger();
        }
    }

    lua_pushinteger(L, entityID);
    return 1;
}

int pick_gizmo(lua_State *L)
{
    if(!lua_istable(L, 1))
        return luaL_argerror(L, 1, "gizmo table needed.");
    if(!lua_isnumber(L, 2))
        return luaL_argerror(L, 2, "float value 'x' needed.");
    if(!lua_isnumber(L, 3))
        return luaL_argerror(L, 3, "float value 'y' needed.");

    LuaPlus::LuaObject gizmos(L, 1);
    Vector2 screenPos((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3));

    Vector3 rayDir = ora::World::instance()->screenPosToWorld(screenPos);
    Vector3 rayOrg = ora::World::instance()->getCamera()->getPosition();

    float neastDist = FloatMax;
    int gizmoID = 0;

    for(LuaPlus::LuaTableIterator it(gizmos); it; ++it)
    {
        LuaPlus::LuaObject gizmo = it.GetValue();
        if(!gizmo.GetUserType())
            continue;

        ora::Gizmo * pGizmo = (ora::Gizmo *)gizmo.GetUserType();
        if(!pGizmo)
            continue;

        BoundingBox bb = pGizmo->localBoundingBox();

		Matrix mat = pGizmo->getWorldMatrix();
		Matrix matInv;
		matInv.invert(mat);
		
		Vector3 rayDirInGizmoSpace = matInv.applyVector(rayDir);
		Vector3 rayOrgInGizmoSpace = matInv.applyPoint(rayOrg);

        float dist;
        ora::AABB aabb(bb);
		if (!aabb.intersectsRay(rayOrgInGizmoSpace, rayDirInGizmoSpace, &dist))
			continue;

		Vector3 point = rayOrgInGizmoSpace + rayDirInGizmoSpace * dist;
		mat.applyPoint(point);

		dist = (point - rayOrg).length();

        if( dist < neastDist)
        {
            neastDist = dist;
            gizmoID = it.GetKey().GetInteger();
        }
    }

    lua_pushinteger(L, gizmoID);
    return 1;
}
