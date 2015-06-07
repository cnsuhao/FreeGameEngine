//
//  object_linker.cpp
//  liborange
//
//  Created by zhoubao on 14-3-27.
//
//

#ifndef _RELEASE

#include "object_linker.h"

#include <map>
#include <set>
#include <string>
#include <mutex>

#include "assert_tool.h"
#include "log_tool.h"

int g_ref_counter = 0;
static RefHeader * s_pRoot = nullptr;

static std::mutex * getLinkMutex()
{
    static std::mutex *pLinkMutex = new std::mutex();
    return pLinkMutex;
}

//#define USE_REF_HEADER

#ifdef USE_REF_HEADER

#define CHECK_LINK_REF(p)\
do{\
    if (!p) break; \
    if (p->__next_ref_)\
    {\
        ASSERT_2(p->__next_ref_->__prev_ref_ == p, "CHECK_LINK_REF"); \
    }\
    if (p->__prev_ref_)\
    {\
        ASSERT_2(p->__prev_ref_->__next_ref_ == p, "CHECK_LINK_REF"); \
    }\
} while (0)

#else

static std::set<RefHeader*> * getInstanceCollector()
{
    static std::set<RefHeader*> *pCollector = new std::set<RefHeader*>();
    return pCollector;
}

#endif



/*static*/ RefHeader * RefHeader::getRootRef()
{
    return s_pRoot;
}

/*static */void RefHeader::__linkRef(RefHeader *p)
{
    std::lock_guard<std::mutex> lk(*getLinkMutex());

#ifdef USE_REF_HEADER
    
    if(!s_pRoot)
    {
        s_pRoot = p;
    }
    else
    {
        p->__next_ref_ = s_pRoot;
        p->__prev_ref_ = s_pRoot->__prev_ref_;
        s_pRoot->__prev_ref_ = p;
        s_pRoot = p;
    }
    
    CHECK_LINK_REF(p);
    CHECK_LINK_REF(p->__next_ref_);
    CHECK_LINK_REF(p->__prev_ref_);
#else
    getInstanceCollector()->insert(p);
#endif
}

/*static */void RefHeader::__unlinkRef(RefHeader *p)
{
    std::lock_guard<std::mutex> lk(*getLinkMutex());

#ifdef USE_REF_HEADER
    
    CHECK_LINK_REF(p);
    
    if(p->__next_ref_)
    {
        p->__next_ref_->__prev_ref_ = p->__prev_ref_;
    }
    
    if(p->__prev_ref_)
    {
        p->__prev_ref_->__next_ref_ = p->__next_ref_;
    }
    
    CHECK_LINK_REF(p->__next_ref_);
    CHECK_LINK_REF(p->__prev_ref_);
    
    if(p == s_pRoot) s_pRoot = p->__next_ref_;
    
    p->__prev_ref_ = p->__next_ref_ = nullptr;
#else
    getInstanceCollector()->erase(p);
#endif
}


RefHeader::RefHeader()
: __next_ref_(nullptr)
, __prev_ref_(nullptr)
{
    ++g_ref_counter;
    
    __linkRef(this);
}

RefHeader::RefHeader(const RefHeader &)
{
    ++g_ref_counter;
    
    __linkRef(this);
}

RefHeader::~RefHeader()
{
    --g_ref_counter;
    
    __unlinkRef(this);
}

const RefHeader & RefHeader::operator = (const RefHeader &)
{
    return *this;
}

void RefHeader::dumpAllLinkedObjects()
{
    std::map<std::string, int> counter;

    getLinkMutex()->lock();
#ifdef USE_REF_HEADER

    RefHeader *p = RefHeader::getRootRef();
    while(p)
    {
        CHECK_LINK_REF(p);
        
        ++counter[typeid(*p).name()];
        p = p->__next_ref_;
    }
    
#else
    
    auto pCollector = getInstanceCollector();
    for(RefHeader * p : (*pCollector))
    {
        ++counter[typeid(*p).name()];
    }

#endif
    getLinkMutex()->unlock();

    if(counter.empty())
    {
        ORA_LOG_INFO("---The linked object is clean.");
    }
    else
    {
        ORA_LOG_WARN("---The object bellow doen't deleted!---");
        for(auto it : counter)
        {
            ORA_LOG_WARN("---object: %-4d %s", it.second, it.first.c_str());
        }
        ORA_LOG_WARN("---The object above doen't deleted!---");
    }
}

#endif
