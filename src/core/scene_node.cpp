//
//  Scenenode.cpp
//  my3d_libs
//
//  Created by Kevin on 14-3-20.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "scene_node.h"
#include "graphics/render_device.h"
#include "world.h"

#include "util/watcher.h"
#include "util/assert_tool.h"

namespace ora
{
    static int s_numNode = -1;
    
    SceneNode::SceneNode()
        : parent_(nullptr)
        , inWorld_(false)
		, color_(1.f, 1.f, 1.f, 1.f)
		, actions_(ActionContainerOder())
    {
        if(s_numNode < 0)
        {
            ADD_WATCHER("3dscene/num_node", s_numNode, true);
            s_numNode = 0;
        }
        
        ++s_numNode;
		actions_.owner(this);
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

    void SceneNode::removeChild(SceneNodePtr child)
    {
        ASSERT_2(child && child->parent_ == this, "SceneNode::removeChild");

        child->parent_ = nullptr;
        children_.remove(child);

        if(child->isInWorld())
            child->leaveWorld();
    }

    void SceneNode::removeChildByName(const std::string & name)
    {
        SceneNode * child = getChild(name);
        if(child) removeChild(child);
    }
    
    void SceneNode::topmost()
    {
        if(!parent_) return;
        
        SceneNodePtr pThis = this;

        parent_->children_.remove(pThis);
        parent_->children_.push_back(pThis);
    }

    void SceneNode::removeFromParent()
    {
        if(parent_)
            parent_->removeChild(this);
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
        actions_.tick(elapse);
        
        children_.lock();

        for(SceneNodePtr child : children_)
        {
            child->tick(elapse);
        }
        
        children_.unlock();
    }

    void SceneNode::draw()
    {
        actions_.draw();

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
        Transform::dirty(matrixDirtyBits);

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
        actions_.push(action);
    }

    void SceneNode::removeAction(ActionPtr action)
    {
        ASSERT_2(action && action->owner() == this, "SceneNode::removeAction");
        actions_.pop(action);
    }
    
    void SceneNode::clearActions()
    {
        actions_.clear();
    }
    

}; // end of namespace ora