#include "aabbrigid.h"
#include "aabbtree.h"

namespace ora
{
    IRigidBody::IRigidBody()
        : pABTree_(nullptr)
        , pUserData_(nullptr)
    {}

    IRigidBody::~IRigidBody()
    {
        removeFromTree();
    }

    void IRigidBody::removeFromTree()
    {
        if(pABTree_) pABTree_->delRigidBody(this);
    }

    void IRigidBody::setAABB(const AABB & aabb)
    {
        if(almostEqual(aabb_.min, aabb.min) &&
                almostEqual(aabb_.max, aabb.max)) return;

        aabb_ = aabb;
        if (pABTree_ != nullptr) pABTree_->makeDirty();
    }

    const AABB & IRigidBody::getAABB() const
    {
        return aabb_;
    }

    void * IRigidBody::getUserData() const
    {
        return pUserData_;
    }

    void IRigidBody::setUserData(void *pData)
    {
        pUserData_ = pData;
    }

    ////////////////////////////////////////////////////////////////////
    RigidProxy::RigidProxy(RigidBodyPtr source, const AABB & aabb)
        : source_(source)
    {
        aabb_ = aabb;
    }

    RigidProxy::~RigidProxy()
    {

    }

    const AABB & RigidProxy::getAABB() const
    {
        return aabb_;
    }

    void RigidProxy::setAABB(const AABB & aabb)
    {
        source_->setAABB(aabb);
    }

    void * RigidProxy::getUserData() const
    {
        return source_->getUserData();
    }

    void RigidProxy::setUserData(void *pData)
    {
        source_->setUserData(pData);
    }



}//end namespace ora