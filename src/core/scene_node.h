//
//  Scenenode.h
//  my3d_libs
//
//  Created by Kevin on 14-3-20.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __my3d_libs__SceneNode__
#define __my3d_libs__SceneNode__

#include "util/smartptr.h"
#include "util/list_proxy.h"
#include "transform.h"
#include "action_container.h"

namespace ora
{
    extern Vector4 actionAlphaToken; // defined in effect_auto_const_imp.cpp

    class ActionContainerOder;
    typedef SmartPtr<class SceneNode> SceneNodePtr;

    ///3d场景结点
    class SceneNode: public IReferenceCount, public Transform
    {
    public:
        typedef list_proxy<SceneNodePtr> Containder;
		typedef SmartPtr<class IAction> ActionPtr;

        SceneNode();
        ~SceneNode();

        static SceneNode *create();

        virtual bool init();
        virtual void destroy();


        const std::string & getName() const { return name_; }
        void setName(const std::string & name) { name_ = name; }

        SceneNode * getParent() { return parent_; }
        SceneNode * getRoot();

        const Matrix & getWorldMatrix() const;
        void setWorldMatrix(const Matrix & mat);

        SceneNode * getChild(const std::string & name);

        const Containder & getChildren() const { return children_; }
        
        void topmost();

        void removeFromParent();

        void addChild(SceneNodePtr child);
        void delChild(SceneNodePtr child);
        void delChildByName(const std::string & name);

        void clearChildren();
        
        virtual void tick(float elapse);
        virtual void draw();
        
	    bool isInWorld() const { return inWorld_ != 0; }
	    virtual void enterWorld();
	    virtual void leaveWorld();
        
        void addAction(ActionPtr action);
        void removeAction(ActionPtr action);
        void clearActions();

        void setColor(Vector4 color) { color_ = color; }
        
    protected:

        virtual void dirty(int matrixDirtyBits);
        virtual void onDestroy();

        std::string         name_;
        SceneNode*          parent_;
        Containder          children_;
	    int	    		    inWorld_;
        Vector4             color_;


    private:
        mutable Matrix      matWorld_;
        ActionContainerOder	actions_;

        friend class World;
    };

}; // end of namespace ora

#endif /* defined(__my3d_libs__SceneNode__) */
