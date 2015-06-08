#ifndef AABB_COLLIDER_H_
#define AABB_COLLIDER_H_

#include "util/smartptr.h"
#include "math/vector3.hpp"

#include <vector>

namespace ora
{
    class AABBTree;
    typedef SmartPtr<class IRigidBody> RigidBodyPtr;
    typedef std::vector<RigidBodyPtr> RigidPool;


    ///射线碰撞检测接口
    class IABTreeRayCollider
    {
    public:

        Vector3 origin;
        Vector3 dir;

        IABTreeRayCollider();
        virtual ~IABTreeRayCollider();

        ///是否与AABB树叶结点碰撞。
        virtual bool query(AABBTree *pTree, const RigidPool & rigids) = 0;
    };


    class ABTreeRayColliderOne : public IABTreeRayCollider
    {
        RigidBodyPtr rigid_;
    public:
        ABTreeRayColliderOne();
        ~ABTreeRayColliderOne();

        virtual bool query(AABBTree *pTree, const RigidPool & rigids) override;

        RigidBodyPtr getRigid() const
        {
            return rigid_;
        }
    };

    class ABTreeRayColliderAll : public IABTreeRayCollider
    {
        RigidPool rigids_;
    public:
        ABTreeRayColliderAll();
        ~ABTreeRayColliderAll();

        virtual bool query(AABBTree *pTree, const RigidPool & rigids_) override;

        const RigidPool & getRigids() const
        {
            return rigids_;
        }
    };

}//end namespace ora


#endif //AABB_COLLIDER_H_