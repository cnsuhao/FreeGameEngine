//
//  Model.cpp
//  my3d_libs
//
//  Created by Kevin on 14-3-20.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "mesh_node.h"
#include "aabbrigid.h"
#include "graphics/mesh_mgr.h"
#include "graphics/render_device.h"

namespace ora
{

    MeshNode* MeshNode::create(const std::string & resource)
    {
        MeshPtr mesh = MeshMgr::instance()->get(resource);
        if(mesh)
        {
            MeshNode* node = new MeshNode();
            node->setMesh(mesh);
            return node;
        }
        return nullptr;
    }

    MeshNode::MeshNode()
        : mesh_(nullptr)
    {
        createRigid();
    }

    MeshNode::MeshNode(const std::string & resource)
    {
        load(resource);
        createRigid();
    }

    MeshNode::~MeshNode()
    {
        setMesh(nullptr);

        if(rigid_)
            rigid_->removeFromTree();
    }

    void MeshNode::createRigid()
    {
        if(rigid_) return;

        rigid_ = new IRigidBody();
        rigid_->setUserData(this);
    }

    bool MeshNode::load(const std::string & resource)
    {
        MeshPtr mesh = MeshMgr::instance()->get(resource);
        setMesh(mesh);
        return mesh.exists();
    }

    void MeshNode::setMesh(MeshPtr mesh)
    {
        mesh_ = mesh;

        if(!mesh_)
        {
            rigid_->setAABB(AABB());
        }
    }

    AABB MeshNode::getWorldAABB() const
    {
        AABB aabb;
        if(mesh_) aabb = mesh_->getAABB();

        aabb.applyMatrix(getWorldMatrix());
        return aabb;
    }

    void MeshNode::updateAABB()
    {
        if(mesh_ && rigid_) rigid_->setAABB(getWorldAABB());
    }

    void MeshNode::tick(float elapse)
    {
        SceneNode::tick(elapse);
        updateAABB();
    }

    void MeshNode::draw()
    {
        SceneNode::draw();
		
        if(mesh_)
        {
			actionAlphaToken = color_;
            renderDev()->pushWorld(getWorldMatrix());
            mesh_->draw();
            renderDev()->popWorld();
        }
    }

}; // end of namespace ora