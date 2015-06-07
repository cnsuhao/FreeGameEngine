//
//  mesh_mgr.cpp
//  liborange
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "mesh_mgr.h"
#include "util/log_tool.h"

IMPLEMENT_SINGLETON(ora::MeshMgr);

namespace ora
{
    MeshMgr::MeshMgr()
        : BaseResMgr<MeshMgr, Mesh>("cache/resource/mesh")
    {
    	ORA_STACK_TRACE;
    }

    MeshMgr::~MeshMgr()
    {
    	ORA_STACK_TRACE;
    }

}//end namespace ora