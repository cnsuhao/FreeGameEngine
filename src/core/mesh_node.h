//
//  Model.h
//  my3d_libs
//
//  Created by Kevin on 14-3-20.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __my3d_libs__Model__
#define __my3d_libs__Model__

#include "scene_node.h"
#include "graphics/mesh.h"
#include "aabbcollider.h"
#include "aabbrigid.h"

namespace ora
{
    typedef SmartPtr<class MeshNode> MeshNodePtr;

    class MeshNode : public SceneNode
    {
    public:

        static MeshNode* create(const std::string & resource);

        MeshNode();
        MeshNode(const std::string & resource);
        virtual ~MeshNode();

        virtual void tick(float elapse) override;
        virtual void draw() override;

        bool load(const std::string & name);
        void setMesh(MeshPtr mesh);

        MeshPtr getMesh() const { return mesh_; }
        RigidBodyPtr getRigid() const { return rigid_; }

        AABB getWorldAABB() const;

    protected:
        void createRigid();
        void updateAABB();

        MeshPtr mesh_;
        RigidBodyPtr rigid_;
    };

}; // end of namespace ora

#endif /* defined(__my3d_libs__Model__) */
