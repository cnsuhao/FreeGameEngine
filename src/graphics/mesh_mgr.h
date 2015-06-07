//
//  mesh_mgr.h
//  liborange
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__MeshMgr__
#define __liborange__MeshMgr__

#include "mesh.h"
#include "base_res_mgr.h"

namespace ora
{
    class MeshMgr : public BaseResMgr<MeshMgr, Mesh>
    {
    public:
        MeshMgr();
        ~MeshMgr();
    };

}//end namespace ora

#endif /* defined(__liborange__MeshMgr__) */
