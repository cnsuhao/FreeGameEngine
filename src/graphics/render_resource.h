#ifndef H_ORANGE_RENDER_RESOURCE
#define H_ORANGE_RENDER_RESOURCE

#include "base_res_mgr.h"
#include <set>

namespace ora
{
    class IResource : public IRes
    {
    public:
        IResource(void);
        virtual ~IResource(void);

        virtual void onDeviceCreate() {}
        virtual void onDeviceClose() {}

        virtual void onDeviceLost() {}
        virtual void onDeviceReset() {}
    };

    ///////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////

    class ResourceMonitor :
        public Singleton<ResourceMonitor>
    {
    public:
        ResourceMonitor();
        ~ResourceMonitor();

        void addResource(IResource *p);
        void delResource(IResource *p);

        void deviceCreate();
        void deviceClose();

        void deviceLost();
        void deviceReset();

    private:
        std::set<IResource*> resources_;
    };

}// end namespace ora

#endif //H_ORANGE_RENDER_RESOURCE