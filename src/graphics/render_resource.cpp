#include "render_resource.h"


namespace ora
{
    IResource::IResource(void)
    {
        if(ResourceMonitor::hasInstance())
            ResourceMonitor::instance()->addResource(this);
    }

    IResource::~IResource(void)
    {
        if(ResourceMonitor::hasInstance())
            ResourceMonitor::instance()->delResource(this);
    }

    ///////////////////////////////////////////////////////////////////
    ///
    ///////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(ResourceMonitor);
        
    ResourceMonitor::ResourceMonitor()
    {
    }

    ResourceMonitor::~ResourceMonitor()
    {
    }

    
    void ResourceMonitor::addResource(IResource *p)
    {
        resources_.insert(p);
    }

    void ResourceMonitor::delResource(IResource *p)
    {
        resources_.erase(p);
    }

    void ResourceMonitor::deviceCreate()
    {
        for(IResource *p : resources_)
        {
            p->onDeviceCreate();
        }
    }

    void ResourceMonitor::deviceClose()
    {
        for(IResource *p : resources_)
        {
            p->onDeviceClose();
        }
    }

    void ResourceMonitor::deviceLost()
    {
        for(IResource *p : resources_)
        {
            p->onDeviceLost();
        }
    }

    void ResourceMonitor::deviceReset()
    {
        for(IResource *p : resources_)
        {
            p->onDeviceReset();
        }
    }

}
