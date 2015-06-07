#ifndef AABB_RIGID_H_
#define AABB_RIGID_H_

#include "util/smartptr.h"
#include "graphics/aabb.h"

namespace ora
{
    class AABBTree;

    ///刚体基类
    class IRigidBody : public IReferenceCount
    {
    public:
        IRigidBody();
        virtual ~IRigidBody();

        virtual const AABB & getAABB() const;
        virtual void setAABB(const AABB & aabb);

        virtual void * getUserData() const;
        virtual void setUserData(void *pData);

        void removeFromTree();

    protected:
        AABB        aabb_;
        AABBTree    *pABTree_;
        void        *pUserData_;

        friend class AABBTree;
    };
    typedef SmartPtr<IRigidBody> RigidBodyPtr;


    class RigidProxy : public IRigidBody
    {
    public:
        RigidProxy(RigidBodyPtr source, const AABB & aabb);
        ~RigidProxy();

        virtual const AABB & getAABB() const;
        virtual void setAABB(const AABB & aabb);

        virtual void * getUserData() const;
        virtual void setUserData(void *pData);

    protected:
        RigidBodyPtr source_;
    };

}//end namespace ora

#endif //AABB_RIGID_H_