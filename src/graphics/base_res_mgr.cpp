
#include "base_res_mgr.h"

#include "util/assert_tool.h"
#include "util/log_tool.h"
#include "util/watcher.h"

namespace ora
{

    IResMgr::IResMgr(const char * name)
        : name_(name)
        , nbCached_(0)
        , nbCapacity_(32)
    {
        LRUHead_.prev = &LRUHead_;
        LRUHead_.next = &LRUHead_;

        pMutex_ = new std::mutex();

        ADD_WATCHER(name, nbCached_);
    }

    IResMgr::~IResMgr()
    {
        DEL_WATCHER(name_);
        LRUClear();

        delete pMutex_;
    }

    void IResMgr::setCapacity(size_t n)
    {
        ASSERT_1(n > 0);

        std::lock_guard<std::mutex> holder(*pMutex_);
        nbCapacity_ = n;

        while(nbCached_ >= nbCapacity_)
        {
            LRUPurge();
        }
    }

    ResPtr IResMgr::get(const std::string & name)
    {
        if(name.empty())
        {
            return nullptr;
        }

        std::lock_guard<std::mutex> holder(*pMutex_);

        TCache::iterator it = cache_.find(name);
        if(it != cache_.end())
        {
            LRUMoveToFront(it->second);
            return it->second->value;
        }

        ResPtr res = createRes();
        if(!res->load(name))
        {
            res = nullptr; // avoid load the invalid resource agin.
        }

        cache_[name] = LRUAddRes(name, res);
        return res;
    }

    void IResMgr::purgeByName(const std::string & name)
    {
        std::lock_guard<std::mutex> holder(*pMutex_);

        TCache::iterator it = cache_.find(name);
        if(it != cache_.end())
        {
            cache_.erase(it);
            LRUDelete(it->second);
        }
    }

    void IResMgr::purge(IRes *pRes)
    {
        IF_NOT_ASSERT(pRes) return;

        std::lock_guard<std::mutex> holder(*pMutex_);

        TCache::iterator it = cache_.begin();
        for(; it != cache_.end(); ++it)
        {
            if(pRes == it->second->value)
            {
                cache_.erase(it);
                LRUDelete(it->second);
                break;
            }
        }
    }

    void IResMgr::purgeAll()
    {
        std::lock_guard<std::mutex> holder(*pMutex_);

        cache_.clear();
        LRUClear();
    }

    void IResMgr::set(const std::string & name, ResPtr res)
    {
        if(name.empty())
        {
            return;
        }

        std::lock_guard<std::mutex> holder(*pMutex_);

        TCache::iterator it = cache_.find(name);
        if(it != cache_.end())
        {
            it->second->value = res;
            LRUMoveToFront(it->second);
            return;
        }
        else
        {
            cache_[name] = LRUAddRes(name, res);
        }
    }

    //TODO: may be the element to be removed is in using, it should be reserved.
    //find an element which is not in using instead.
    void IResMgr::LRUPurge()
    {
        LRUElement *p = LRUHead_.prev;
        if(p->value && p->value->refCount() > 1)
        {
            ORA_LOG_WARN("The resource to be purged is in using. '%s'", p->key.c_str());
        }

        cache_.erase(p->key);
        LRUDelete(p);
    }

    IResMgr::LRUElement* IResMgr::LRUAddRes(const std::string & name, ResPtr res)
    {
        if(nbCached_ >= nbCapacity_)
        {
            LRUPurge();
        }

        LRUElement *p = new LRUElement();
        p->key = name;
        p->value = res;
        LRUAdd(p);
        return p;
    }

    void IResMgr::LRUAdd(IResMgr::LRUElement *p)
    {
        ++nbCached_;
        p->prev = &LRUHead_;
        p->next = LRUHead_.next;

        LRUHead_.next->prev = p;
        LRUHead_.next = p;
    }

    void IResMgr::LRURemove(IResMgr::LRUElement *p)
    {
        ASSERT_1(nbCached_ > 0);
        --nbCached_;

        p->prev->next = p->next;
        p->next->prev = p->prev;
    }

    void IResMgr::LRUMoveToFront(IResMgr::LRUElement *p)
    {
        LRURemove(p);
        LRUAdd(p);
    }

    void IResMgr::LRUDelete(IResMgr::LRUElement *p)
    {
        LRURemove(p);
        delete p;
    }

    void IResMgr::LRUClear()
    {
        LRUElement *p = LRUHead_.next;
        while(p != &LRUHead_)
        {
            LRUElement * temp = p;
            p = p->next;
            delete temp;
        }

        LRUHead_.next = &LRUHead_;
        LRUHead_.prev = &LRUHead_;
    }

}// end namespace ora
