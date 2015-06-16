//
//  Scenenode.cpp
//  my3d_libs
//
//  Created by Kevin on 14-3-20.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "scene_node.h"
#include "world.h"
#include "action_container.h"

#include "graphics/render_device.h"

#include "util/watcher.h"
#include "util/assert_tool.h"

#ifndef CODE_INLINE
#include "scene_node.inl"
#endif

namespace ora
{
    static int s_numNode = -1;
    
    SceneNode::SceneNode()
        : position_(0.f, 0.f, 0.f)
        , scale_(1.0f, 1.0f, 1.0f)
        , matrixDirtyBits_(DIRTY_ALL)
        , parent_(nullptr)
        , inWorld_(false)
		, color_(1.f, 1.f, 1.f, 1.f)
		, actions_(new ActionContainerOder())
    {
        if(s_numNode < 0)
        {
            ADD_WATCHER("3dscene/num_node", s_numNode, true);
            s_numNode = 0;
        }
        
        rotation_.setIdentity();
        matrix_.setIdentity();
        matWorld_.setIdentity();

        ++s_numNode;
		actions_->owner(this);
    }

    SceneNode::~SceneNode()
    {
        --s_numNode;
        
        clearChildren();

        ASSERT_2(parent_ == nullptr, "SceneNode::~SceneNode - invalid delete call, the node is in a scene tree.");

        // this code is useless, because the parent keeps a reference for this.
        // so, this will never destroyed automatically.
        //removeFromParent();
    }

    /*static*/SceneNode *SceneNode::create()
    {
        SceneNode *pNode = new SceneNode();
        if(pNode && pNode->init())
        {
            return pNode;
        }
        else
        {
            delete pNode;
            return nullptr;
        }
    }

    bool SceneNode::init()
    {
        return true;
    }

    void SceneNode::destroy()
    {
        children_.lock();
        for(SceneNodePtr child : children_)
        {
            child->parent_ = nullptr;
            child->destroy();
        }
        children_.unlock();
        children_.clear();

        clearActions();
        removeFromParent();
        onDestroy();
    }

    SceneNode * SceneNode::getRoot()
    {
        //return World::instance()->getRoot();
        SceneNode *p = this;
        while(p->getParent()) p = p->getParent();

        return p;
    }
    
    SceneNode * SceneNode::getChild(const std::string & name)
    {
        SceneNode * child;
        
        size_t pos = name.find('/');
        std::string tag = (pos == name.npos) ? name : name.substr(0, pos);
        
        for(SceneNodePtr chd : children_)
        {
            if(chd->getName() == tag)
            {
                child = chd.get();
                break;
            }
        }

        if(pos != name.npos && child)
            return child->getChild(name.substr(pos+1));

        return child;
    }

    void SceneNode::addChild(SceneNodePtr child)
    {
        ASSERT_2(child && child->parent_ == nullptr, "SceneNode::addChild");

        child->parent_ = this;
        children_.push_back(child);

        if(isInWorld())
            child->enterWorld();
    }

    void SceneNode::delChild(SceneNodePtr child)
    {
        ASSERT_2(child && child->parent_ == this, "SceneNode::delChild");

        child->parent_ = nullptr;
        children_.remove(child);

        if(child->isInWorld())
            child->leaveWorld();
    }
    
    void SceneNode::topmost()
    {
        if(!parent_) return;
        
        SceneNodePtr pThis = this;

        parent_->children_.remove(pThis);
        parent_->children_.push_back(pThis);
    }

    void SceneNode::clearChildren()
    {
        children_.lock();
        for(SceneNodePtr child : children_)
        {
            child->parent_ = nullptr;
            child->clearChildren();
        }
        children_.unlock();
    }

    void SceneNode::tick(float elapse)
    {
        actions_->tick(elapse);
        
        children_.lock();

        for(SceneNodePtr child : children_)
        {
            child->tick(elapse);
        }
        
        children_.unlock();
    }

    void SceneNode::draw()
    {
        actions_->draw();

        children_.lock();

        for(SceneNodePtr child : children_)
        {
            child->draw();
        }

        children_.unlock();
    }

    void SceneNode::onDestroy()
    {
        
    }

    void SceneNode::dirty(int matrixDirtyBits)
    {
        matrixDirtyBits_ |= matrixDirtyBits;

        for(SceneNodePtr child : children_)
        {
            child->dirty(matrixDirtyBits);
        }
    }

    
    const Matrix & SceneNode::getWorldMatrix() const
    {
        if(matrixDirtyBits_)
        {
            matWorld_ = getMatrix();
            if(parent_)
                matWorld_.postMultiply(parent_->getWorldMatrix());
        }
        return matWorld_;
    }

    // world = local * parent
    // local = world * inv parent

    void SceneNode::setWorldMatrix(const Matrix & mat)
    {
        if (parent_)
        {
            Matrix matrix;
            matrix.invert(parent_->getWorldMatrix());
            matrix.preMultiply(mat);
            setMatrix(matrix);
        }
        else
            setMatrix(mat);
    }
    
    void SceneNode::enterWorld()
    {
        IF_NOT_ASSERT(!inWorld_) return;

	    inWorld_ = true;

        children_.lock();
        for(SceneNodePtr child : children_)
        {
            child->enterWorld();
        }
        children_.unlock();
    }

    void SceneNode::leaveWorld()
    {
        IF_NOT_ASSERT(inWorld_) return;

        children_.lock();
        for(SceneNodePtr child : children_)
        {
            child->leaveWorld();
        }
        children_.unlock();

	    inWorld_ = false;
    }
    
    void SceneNode::addAction(ActionPtr action)
    {
        ASSERT_2(action && action->owner() == nullptr, "SceneNode::addAction");
        actions_->push(action);
    }

    void SceneNode::removeAction(ActionPtr action)
    {
        ASSERT_2(action && action->owner() == this, "SceneNode::removeAction");
        actions_->pop(action);
    }
    
    void SceneNode::clearActions()
    {
        actions_->clear();
    }
    
    const Matrix& SceneNode::getMatrix() const
    {
        if (matrixDirtyBits_)
        {
            matrixDirtyBits_ = 0;
            
            matrix_.setScale(scale_);
        
            Matrix temp;
            temp.setRotate(rotation_);
            matrix_.postMultiply(temp);
            
            matrix_.translation(position_);
        }
        return matrix_;
    }

    void SceneNode::lookAt(const Vector3& up, const Vector3& target)
    {
        Vector3 Up;
        Vector3 Look(target - position_);
        Vector3 Right;

        Look.normalise( );
        Right.crossProduct( up, Look );
        Right.normalise( );
        Up.crossProduct( Look, Right );
        Up.normalise();

        Matrix rot;
        rot.m00 = Right.x;
        rot.m01 = Right.y;
        rot.m02 = Right.z;
        rot.m03 = 0.f;

        rot.m10 = Up.x;
        rot.m11 = Up.y;
        rot.m12 = Up.z;
        rot.m13 = 0.f;

        rot.m20 = Look.x;
        rot.m21 = Look.y;
        rot.m22 = Look.z;
        rot.m23 = 0.f;

        rot.m30 = 0;
        rot.m31 = 0;
        rot.m32 = 0;
        rot.m33 = 1.f;

        setRotation(rot);
    }

    void SceneNode::setLook(const Vector3 & look, const Vector3 & up)
    {
        Vector3 Look = look;
        Vector3 Up = up;
        Vector3 Right;

        Right.crossProduct( up, Look );
        Right.normalise( );
        Up.crossProduct( Look, Right );
        Up.normalise();

        Matrix rot;
        rot.m00 = Right.x;
        rot.m01 = Right.y;
        rot.m02 = Right.z;
        rot.m03 = 0.f;

        rot.m10 = Up.x;
        rot.m11 = Up.y;
        rot.m12 = Up.z;
        rot.m13 = 0.f;

        rot.m20 = Look.x;
        rot.m21 = Look.y;
        rot.m22 = Look.z;
        rot.m23 = 0.f;

        rot.m30 = 0;
        rot.m31 = 0;
        rot.m32 = 0;
        rot.m33 = 1.f;

        setRotation(rot);
    }

}; // end of namespace ora
