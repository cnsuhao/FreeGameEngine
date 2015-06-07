#include "aabbcollider.h"
#include "aabbtree.h"
#include "aabbrigid.h"

namespace ora
{

    /////////////////////////////////////////////////////////////////

    IABTreeRayCollider::IABTreeRayCollider()
    {}

    IABTreeRayCollider::~IABTreeRayCollider()
    {}

    /////////////////////////////////////////////////////////////////

    ABTreeRayColliderOne::ABTreeRayColliderOne()
    {}

    ABTreeRayColliderOne::~ABTreeRayColliderOne()
    {}

    bool ABTreeRayColliderOne::query(AABBTree *pTree, const RigidPool & rigids)
    {
        float minDist = FloatMax;
        for (RigidBodyPtr ptr : rigids)
        {
            float dist;
            if(ptr->getAABB().intersectsRay(origin, dir, &dist))
            {
                if(minDist > dist)
                {
                    minDist = dist;
                    rigid_ = ptr;
                }
            }
        }

        return minDist < FloatMax;
    }

    ///////////////////////////////////////////////////////////////////
    ABTreeRayColliderAll::ABTreeRayColliderAll()
    {}

    ABTreeRayColliderAll::~ABTreeRayColliderAll()
    {}

    bool ABTreeRayColliderAll::query(AABBTree *pTree, const RigidPool & rigids)
    {
        rigids_ = rigids;
        return true;
    }


}//end namespace ora