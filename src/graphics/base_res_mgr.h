#ifndef __liborange__base_res_mgr__
#define __liborange__base_res_mgr__

#include "util/smartptr.h"
#include "util/singleton.h"

#include <map>
#include <string>
#include <thread>

namespace ora
{
    class IRes : public SafeReferenceCount
    {
    public:

        virtual bool load(const std::string & path) { return false; };

    };
    typedef SmartPtr<IRes> ResPtr;

    class IResMgr
    {
    public:
        explicit IResMgr(const char * name);
        virtual ~IResMgr();

        virtual IRes * createRes() = 0;

        void setCapacity(size_t n);
        size_t getCapacity() const { return nbCapacity_; }

        ResPtr get(const std::string & name);
        void set(const std::string & name, ResPtr res);

        void purge(IRes *pRes);
        void purgeByName(const std::string & name);
        void purgeAll();

    protected:
        struct LRUElement
        {
            LRUElement* prev;
            LRUElement* next;
            std::string key;
            ResPtr      value;
        };
        typedef std::map<std::string, LRUElement*> TCache;
        
        LRUElement* LRUAddRes(const std::string & name, ResPtr res);

        void LRUPurge();
        void LRUAdd(LRUElement *p);
        void LRURemove(LRUElement *p);
        void LRUMoveToFront(LRUElement *p);
        void LRUDelete(LRUElement *p);
        void LRUClear();
        
    private:
        const char *    name_;
        size_t          nbCached_;
        size_t          nbCapacity_;
        TCache          cache_;
        LRUElement      LRUHead_;
        std::mutex*     pMutex_;
    };
    
    template<typename TSelf, typename TRes>
    class BaseResMgr : public IResMgr, public Singleton<TSelf>
    {
    public:
        typedef SmartPtr<TRes> TResPtr;

        explicit BaseResMgr(const char * name)
            : IResMgr(name)
        {}

        ~BaseResMgr()
        {}

        inline virtual IRes * createRes() override
        {
            return new TRes();
        }

        inline TResPtr get(const std::string & name)
        {
            ResPtr res = IResMgr::get(name);
            return (TRes*)(res.get());
        }

        inline void set(const std::string & name, TResPtr res)
        {
            IResMgr::set(name, res.get());
        }
    };

}// end namespace ora



#endif /* defined(__liborange__base_res_mgr__) */
