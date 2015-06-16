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
#include "math/math.hpp"

namespace ora
{
    extern Vector4 actionAlphaToken; // defined in effect_auto_const_imp.cpp

    class SceneNode;
    typedef SmartPtr<SceneNode> SceneNodePtr;
    
    class ActionContainerOder;
    typedef SmartPtr<ActionContainerOder> ActionContainerOderPtr;

    ///3d场景结点
    class SceneNode: public IReferenceCount
    {
    public:
        typedef list_proxy<SceneNodePtr> Containder;
		typedef SmartPtr<class IAction> ActionPtr;

        SceneNode();
        virtual ~SceneNode();

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

        void setColor(const Vector4 & color) { color_ = color; }
        
        
    public: // transforms
        
        void setTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
        void setTransformIdentity();
        
        void setPosition(const Vector3& translation);
        const Vector3& getPosition() const;

        void setScale(const Vector3& scale);
        const Vector3& getScale() const;
        
        void setRotation(const Quaternion& rotation);
        const Quaternion& getRotation() const;
        void setRotationX(float angle);
        void setRotationY(float angle);
        void setRotationZ(float angle);
        
        float getYaw() const;
        float getPitch() const;
        float getRoll() const;
        
        const Matrix& getMatrix() const;
        void setMatrix(const Matrix & mat);
        
        Vector3 getForwardVector() const;
        Vector3 getUpVector() const;
        Vector3 getRightVector() const;

        void rotate(const Quaternion& rotation);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
        
        void scale(const Vector3& scale);
        
        void translate(const Vector3& translation);
        void translateRight(float amount);
        void translateUp(float amount);
        void translateForward(float amount);
        
        void lookAt(const Vector3& up, const Vector3& target);
        void setLook(const Vector3 & look, const Vector3 & up);
        
    protected:

        virtual void dirty(int matrixDirtyBits);
        virtual void onDestroy();
        
        enum MatrixDirtyBits
        {
            DIRTY_TRANSLATION = 0x01,
            DIRTY_SCALE = 0x02,
            DIRTY_ROTATION = 0x04,
            DIRTY_ALL = 0xff
        };
        

        Vector3             position_;
        Vector3             scale_;
        Quaternion          rotation_;
        
        mutable int         matrixDirtyBits_;
        
        
        std::string         name_;
        SceneNode*          parent_;
        Containder          children_;
	    int	    		    inWorld_;
        Vector4             color_;


    private:
        mutable Matrix      matrix_;
        mutable Matrix      matWorld_;
        ActionContainerOderPtr	actions_;

        friend class World;
    };

}; // end of namespace ora

#ifdef CODE_INLINE
#include "scene_node.inl"
#endif

#endif /* defined(__my3d_libs__SceneNode__) */
