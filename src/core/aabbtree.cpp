
#include "aabbtree.h"
#include "aabbcollider.h"
#include "aabbrigid.h"

#include "util/assert_tool.h"
#include "util/log_tool.h"

#include "graphics/render_device.h"
#include "graphics/render_helper.h"

#include <sstream>

namespace ora
{
    enum SIDE
    {
        SIDE_ON = 0,
        SIDE_FRONT = 1,
        SIDE_BACK = 2,
        SIDE_BOTH = 3,
    };

    namespace ABTreeDebug
    {
        bool showOriginBox = false;
        bool showAABBTree = true;
        bool showNodeBox = false;
        bool showLeafBox = true;
        bool printBuildInfo = false;
    }

    int maxAxis(const Vector3 & pos)
    {
        int axis = AXIS_X;
        if(pos.x < pos.y)
            axis = AXIS_Y;

        if(pos[axis] < pos.z)
            axis = AXIS_Z;

        return axis;
    }

    ///将一个aabb，按照最长轴的一半，分割成两个
    void splitAABBHalf(AABB & rcFront, AABB & rcBack, float & splitPos, const AABB & rect, int splitAxis)
    {
        rcFront = rcBack = rect;
        splitPos = (rect.min[splitAxis] + rect.max[splitAxis]) * 0.5f;
        rcFront.max[splitAxis] = rcBack.min[splitAxis] = splitPos;
    }

    ///较好的分隔法
    bool splitAABBBetter(AABB & rcFront, AABB & rcBack, float & splitPos, const AABB & rect,
                         const RigidPool & rigids, int splitAxis)
    {
        rcFront = rcBack = rect;

        int nIndices = int(rigids.size());
        int bestRate = 0x7fffffff;
        for (RigidBodyPtr x : rigids)
        {
            float pos = x->getAABB().min[splitAxis];

            int a = 0;
            int b = 0;

            for (RigidBodyPtr y : rigids)
            {
                const AABB & aabb = y->getAABB();
                if (aabb.min[splitAxis] < pos) ++a;
                if (aabb.max[splitAxis] > pos) ++b;
            }

            int t = abs(a - b);
            if (bestRate > t)
            {
                bestRate = t;
                splitPos = pos;
            }
        }

        if (bestRate >= nIndices)//出现了退化，所有aabb此边坐标都相等
        {
            splitAABBHalf(rcFront, rcBack, splitPos, rect, splitAxis);
            return true;
        }

        rcFront.max[splitAxis] = rcBack.min[splitAxis] = splitPos;
        return true;
    }

    ///最优分隔法
    bool splitAABBBest(AABB & rcFront, AABB & rcBack, float & splitPos, const AABB & rect,
                       const RigidPool & rigids, int & splitAxis)
    {
        rcFront = rcBack = rect;

        int nIndices = int(rigids.size());
        int minGap = 0x7fffffff;
        bool ok = false;

        for (int i = 0; i < 3; ++i)
        {
            int axis = (i + splitAxis) % 3;

            for (RigidBodyPtr x : rigids)
            {
                float pos = x->getAABB().min[axis];

                int a = 0;
                int b = 0;

                for (RigidBodyPtr y : rigids)
                {
                    const AABB & aabb = y->getAABB();
                    if (aabb.min[axis] < pos) ++a;
                    if (aabb.max[axis] > pos) ++b;
                }

                ASSERT_2((a != 0 || b != 0), "splitAABBBest");

                int t = abs(a - b);
                if (minGap > t)
                {
                    minGap = t;
                    splitPos = pos;
                    splitAxis = axis;

                    if (minGap == nIndices % 2) //已经不能再小了。
                    {
                        ok = true;
                        break;
                    }
                }
            }

            if (ok) break;
        }

        if (minGap >= nIndices)//出现了退化，所有aabb此边坐标都相等
        {
            return false;
        }

        rcFront.max[splitAxis] = rcBack.min[splitAxis] = splitPos;
        return true;
    }

    ///判断区域在分隔点的哪侧
    int wichSide(int splitAxis, float splitPos, const AABB & rc)
    {
        int side = 0;

        if (rc.min[splitAxis] < splitPos) side |= SIDE_FRONT;
        if (rc.max[splitAxis] > splitPos) side |= SIDE_BACK;

        return side;
    }

    void splitAABBOut(AABB & rcFront, AABB & rcBack, const AABB & aabb, float splitPos, int splitAxis)
    {
        ASSERT_1(splitPos > aabb.min[splitAxis] && splitPos < aabb.max[splitAxis]);

        rcFront = rcBack = aabb;
        rcFront.max[splitAxis] = rcBack.min[splitAxis] = splitPos;
    }

    /////////////////////////////////////////////////////////////////

    ABTreeConfig::ABTreeConfig()
    {
        maxDepth = 9;
        minCount = 1;
        minSize = 0.2f;
        better = true;
        best = true;
    }

    /////////////////////////////////////////////////////////////////

    IABTreeBase::IABTreeBase()
    {

    }

    IABTreeBase::IABTreeBase(const AABB & aabb)
        : aabb_(aabb)
    {

    }

    IABTreeBase::~IABTreeBase()
    {

    }

    /////////////////////////////////////////////////////////////////

    ABTreeLeaf::ABTreeLeaf(AABBTree* pTree, const AABB & aabb, const RigidPool & rigids)
        : IABTreeBase(aabb)
        , pTree_(pTree)
        , rigids_(rigids)
    {
    }

    ABTreeLeaf::~ABTreeLeaf()
    {

    }

    ///射线拾取
    bool ABTreeLeaf::pick(IABTreeRayCollider *pCollider) const
    {
        if (!aabb_.intersectsRay(pCollider->origin, pCollider->dir)) return false;

        return pCollider->query(pTree_, rigids_);
    }

    ///渲染AABB树
    void ABTreeLeaf::render()
    {
        if (ABTreeDebug::showLeafBox)
            RenderHelper::instance()->drawAABB(aabb_);
    }

    ///占用的空间
    size_t ABTreeLeaf::numBytes() const
    {
        return sizeof(*this) + rigids_.capacity() * sizeof(RigidBodyPtr);
    }

    /////////////////////////////////////////////////////////////////

    ABTreeNode::ABTreeNode(const AABB & aabb, int splitAxis, float splitPos)
        : IABTreeBase(aabb)
        , splitAxis_(splitAxis)
        , splitPos_(splitPos)
    {

    }

    ABTreeNode::~ABTreeNode()
    {

    }

    ///射线拾取
    bool ABTreeNode::pick(IABTreeRayCollider *pCollider) const
    {
        if (!aabb_.intersectsRay(pCollider->origin, pCollider->dir)) return false;

        if (pCollider->origin[splitAxis_] < splitPos_)
        {
            if (front_ && front_->pick(pCollider)) return true;
            if (back_ && back_->pick(pCollider)) return true;
        }
        else
        {
            if (back_ && back_->pick(pCollider)) return true;
            if (front_ && front_->pick(pCollider)) return true;
        }

        return false;
    }

    ///渲染AABB树
    void ABTreeNode::render()
    {
        if (front_) front_->render();
        if (back_) back_->render();

        if (ABTreeDebug::showNodeBox)
            RenderHelper::instance()->drawAABB(aabb_);
    }

    ///构造AABB树
    /*static*/bool ABTreeNode::build(ABTreePtr & child,
                                     AABBTree *pTree, const RigidPool & rigids, size_t depth)
    {
        ASSERT_2(pTree, "ABTreeNode::build");

        AABB newAABB;
        newAABB.reset();
        for (RigidBodyPtr ptr : rigids)
        {
            newAABB.add(ptr->getAABB());
        }

        if (rigids.size() <= pTree->config().minCount || depth >= pTree->config().maxDepth)
        {
            //do something
        }
        else
        {
            int splitAxis;
            float splitPos;
            AABB rcFront, rcBack;
            RigidPool front, back;

            while (true)
            {
                Vector3 diameter;//直径
                newAABB.getDiameter(diameter);

                splitAxis = maxAxis(diameter);//最大分割轴

                //到达最小分割粒度
                if (fabs(diameter[splitAxis]) <= pTree->config().minSize)
                {
                    break;
                }

                if (pTree->config().best)//最优分隔
                {
                    if (!splitAABBBest(rcFront, rcBack, splitPos,
                                       newAABB, rigids, splitAxis)) break;
                }
                else if (pTree->config().better)//最优分隔
                {
                    if (!splitAABBBetter(rcFront, rcBack, splitPos,
                                         newAABB, rigids, splitAxis)) break;
                }
                else//快速分隔
                {
                    splitAABBHalf(rcFront, rcBack, splitPos, newAABB, splitAxis);
                }

                front.clear();
                back.clear();

                for (RigidBodyPtr x : rigids)
                {
                    int side = wichSide(splitAxis, splitPos, x->getAABB());

                    if (side == SIDE_FRONT) front.push_back(x);
                    else if (side == SIDE_BACK) back.push_back(x);
                    else if (side == SIDE_BOTH)
                    {
                        AABB aF, aB;
                        splitAABBOut(aF, aB, x->getAABB(), splitPos, splitAxis);
                        front.push_back(new RigidProxy(x, aF));
                        back.push_back(new RigidProxy(x, aB));
                    }
                }

                if (front.empty())
                {
                    newAABB = rcBack;
                }
                else if (back.empty())
                {
                    newAABB = rcFront;
                }
                else
                {
                    ABTreeNode *pNode = new ABTreeNode(newAABB, splitAxis, splitPos);
                    child = pNode;
                    build(pNode->front_, pTree, front, depth + 1);
                    build(pNode->back_, pTree, back, depth + 1);

                    return true;
                }
            }
        }

        child = new ABTreeLeaf(pTree, newAABB, rigids);
        return true;
    }

    ///最大深度
    size_t ABTreeNode::numMaxDepth() const
    {
        size_t d1 = 0, d2 = 0;
        if (front_) d1 = front_->numMaxDepth();
        if (back_) d2 = back_->numMaxDepth();

        return 1 + std::max(d1, d2);
    }

    ///结点个数
    size_t ABTreeNode::numNode() const
    {
        size_t n = 1;
        if (front_) n += front_->numNode();
        if (back_) n += back_->numNode();
        return n;
    }

    ///叶结点个数
    size_t ABTreeNode::numLeaf() const
    {
        size_t n = 0;
        if (front_) n += front_->numLeaf();
        if (back_) n += back_->numLeaf();
        return n;
    }

    ///占用的空间
    size_t ABTreeNode::numBytes() const
    {
        size_t size = sizeof(*this);
        if (front_) size += front_->numBytes();
        if (back_) size += back_->numBytes();
        return size;
    }

    /////////////////////////////////////////////////////////////////
    AABBTree::AABBTree()
        : dirty_(false)
    {}

    AABBTree::AABBTree(const ABTreeConfig & config)
        : dirty_(false)
        , config_(config)
    {
    }


    AABBTree::~AABBTree()
    {
        clear();
    }

    void AABBTree::clear()
    {
        dirty_ = false;

        for(RigidBodyPtr rigid : rigids_) rigid->pABTree_ = nullptr;

        rigids_.clear();
        root_ = nullptr;
    }

    size_t AABBTree::addRigidBody(RigidBodyPtr rigid)
    {
        M_ASSERT(rigid, "AABBTree::addRigidBody");
        M_ASSERT(rigid->pABTree_ == nullptr, "AABBTree::addRigidBody - rigid has been added to another AABBTree!");

        rigid->pABTree_ = this;
        rigids_.push_back(rigid);
        dirty_ = true;

        return rigids_.size() - 1;
    }

    void AABBTree::delRigidBody(RigidBodyPtr rigid)
    {
        M_ASSERT(rigid, "AABBTree::addRigidBody");

        auto it = std::find(rigids_.begin(), rigids_.end(), rigid);
        if (it != rigids_.end())
        {
            (*it)->pABTree_ = nullptr;

            rigids_.erase(it);
            dirty_ = true;
        }
    }

    void AABBTree::delRigidBodyById(size_t id)
    {
        if (id < rigids_.size())
        {
            auto it = rigids_.begin() + id;
            (*it)->pABTree_ = nullptr;

            rigids_.erase(it);
            dirty_ = true;
        }
    }

    bool AABBTree::update()
    {
        if (dirty_)
        {
            dirty_ = false;
            return doBuild();
        }

        return false;
    }

    bool AABBTree::doBuild()
    {
        bool ret = ABTreeNode::build(root_, this, rigids_, 1);
        if (root_) aabb_ = root_->getAABB();

        if(ABTreeDebug::printBuildInfo)
        {
            std::ostringstream ss;
            ss  << "maxDepth: " << numMaxDepth() << "\n"
                << "numNode: " << numNode() << "\n"
                << "numLeaf: " << numLeaf() << "\n"
                << "bytes: " << numBytes()
                ;
            ORA_LOG_INFO("AABBTree info: %s", ss.str().c_str());
        }

        return ret;
    }

    ///渲染AABB树
    void AABBTree::render()
    {
        if (ABTreeDebug::showAABBTree && root_) root_->render();

        if (ABTreeDebug::showOriginBox)
        {
            for (RigidBodyPtr rb : rigids_)
            {
                RenderHelper::instance()->drawAABB(rb->getAABB());
            }
        }
    }

    ///最大深度
    size_t AABBTree::numMaxDepth() const
    {
        if (root_) return root_->numMaxDepth();
        return 0;
    }

    ///结点个数
    size_t AABBTree::numNode() const
    {
        if (root_) return root_->numNode();
        return 0;
    }

    ///叶结点个数
    size_t AABBTree::numLeaf() const
    {
        if (root_) return root_->numLeaf();
        return 0;
    }

    size_t AABBTree::numBytes() const
    {
        size_t size = sizeof(*this);
        size += rigids_.capacity() * sizeof(RigidBodyPtr);
        if (root_) size += root_->numBytes();
        return size;
    }

    bool AABBTree::pick(IABTreeRayCollider *pCollider) const
    {
        return root_ && root_->pick(pCollider);
    }

    RigidBodyPtr AABBTree::getRigidBody(size_t id) const
    {
        M_ASSERT(id < rigids_.size(), "AABBTree::getRigidBody");
        return rigids_[id];
    }

    const AABB & AABBTree::getRigidAABB(size_t id) const
    {
        return getRigidBody(id)->getAABB();
    }

    const ABTreeConfig & AABBTree::config() const
    {
        return config_;
    }

    void AABBTree::setConfig(const ABTreeConfig & config)
    {
        config_ = config;
    }

    void AABBTree::makeDirty()
    {
        dirty_ = true;
    }

}//namespace Physics