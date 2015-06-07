#ifndef AABBTREE_H_
#define AABBTREE_H_

#include "util/smartptr.h"
#include "aabbrigid.h"

namespace ora
{
    class IABTreeBase;
    class AABBTree;
    class IABTreeRayCollider;

    typedef std::vector<RigidBodyPtr> RigidPool;

    ///构造AABB树配置参数
    struct ABTreeConfig
    {
        size_t maxDepth;///<最大构造深度
        size_t minCount;///<叶结点最新最小区域数量
        float minSize;///<区域最小分隔尺寸
        bool  better;///<生成较好的四叉树。速度稍慢。
        bool  best;///<生成最优四叉树。最好，意味着消耗更多的时间。

        ABTreeConfig();
    };

    ///调试参数
    namespace ABTreeDebug
    {
        ///显示原始的包围盒
        extern bool showOriginBox;

        ///显示AABB树的包围盒。如果此值为false，则忽略showNodeBox，showLeafBox
        extern bool showAABBTree;

        ///显示结点的包围盒
        extern bool showNodeBox;

        ///显示叶结点包围盒
        extern bool showLeafBox;

        ///打印构建信息
        extern bool printBuildInfo;

    }

    ///AABB树基类
    class IABTreeBase : public IReferenceCount
    {
    public:
        IABTreeBase();
        IABTreeBase(const AABB & aabb);
        ~IABTreeBase();

        ///是否是叶结点
        virtual bool isLeaf() const
        {
            return false;
        }

        ///射线拾取
        virtual bool pick(IABTreeRayCollider *pCollider) const = 0;

        const AABB & getAABB() const
        {
            return aabb_;
        }

    public://调试

        ///渲染AABB树
        virtual void render() = 0;

        ///最大深度
        virtual size_t numMaxDepth() const
        {
            return 1;
        }

        ///结点个数
        virtual size_t numNode() const
        {
            return 0;
        }

        ///叶结点个数
        virtual size_t numLeaf() const
        {
            return 1;
        }

        ///占用的空间
        virtual size_t numBytes() const = 0;

    protected:
        AABB aabb_; ///<结点的包围盒
    };
    typedef SmartPtr<IABTreeBase> ABTreePtr;

    ///AABB树叶子
    class ABTreeLeaf : public IABTreeBase
    {
    public:
        ABTreeLeaf(AABBTree *pTree, const AABB & aabb, const RigidPool & rigids);
        ~ABTreeLeaf();

        ///是否是叶结点
        virtual bool isLeaf() const override
        {
            return true;
        }

        ///射线拾取
        virtual bool pick(IABTreeRayCollider *pCollider) const override;

    public://调试

        ///渲染AABB树
        virtual void render() override;

        ///占用的空间
        virtual size_t numBytes() const override;

    private:
        AABBTree        *pTree_;///<所属的AABB树
        RigidPool       rigids_;
    };


    ///AAABB树结点
    class ABTreeNode : public IABTreeBase
    {
    public:
        ABTreeNode(const AABB & aabb, int splitAxis, float splitPos);
        ~ABTreeNode();

        ///射线拾取
        virtual bool pick(IABTreeRayCollider *pCollider) const override;

        ///构造AABB树
        static bool build(ABTreePtr & child, AABBTree *pTree, const RigidPool & rigids, size_t depth);

    public://调试

        ///渲染AABB树
        virtual void render() override;

        ///最大深度
        virtual size_t numMaxDepth() const override;

        ///结点个数
        virtual size_t numNode() const override;

        ///叶结点个数
        virtual size_t numLeaf() const override;

        ///占用的空间
        virtual size_t numBytes() const override;

    private:
        ABTreePtr       front_;
        ABTreePtr       back_;
        int             splitAxis_;///<分割轴
        float           splitPos_;///<分隔点
    };


    ///AABB树
    class AABBTree : public IABTreeBase
    {
    public:
        AABBTree();
        AABBTree(const ABTreeConfig & config);
        ~AABBTree();

        bool update();

        void clear();

        size_t addRigidBody(RigidBodyPtr rigid);
        void delRigidBody(RigidBodyPtr rigid);
        void delRigidBodyById(size_t id);

        ///根据索引获取AABB
        RigidBodyPtr getRigidBody(size_t id) const;

        const AABB & getRigidAABB(size_t id) const;

        const ABTreeConfig & config() const;
        void setConfig(const ABTreeConfig & config);

        void makeDirty();

        ///射线拾取
        virtual bool pick(IABTreeRayCollider *pCollider) const;

    public://调试

        ///渲染AABB树
        virtual void render() override;

        ///最大深度
        virtual size_t numMaxDepth() const override;

        ///结点个数
        virtual size_t numNode() const override;

        ///叶结点个数
        virtual size_t numLeaf() const override;

        ///占用的空间
        virtual size_t numBytes() const override;

    protected:

        bool doBuild();

        bool            dirty_;
        ABTreeConfig    config_;   ///<配置
        RigidPool       rigids_;    ///<aabb数组
        ABTreePtr       root_;     ///<AABB树根节点

    private:
        AABBTree(const AABBTree &);
        const AABBTree & operator=(const AABBTree &);
    };

}//end namespace ora

#endif //AABBTREE_H_
