//
//  event_listener.h
//  orange
//
//  Created by zhoubao on 14-6-11.
//
//

#ifndef __orange__event_listener__
#define __orange__event_listener__

#include "smartptr.h"
#include "event_dispatch.h"
#include "util_config.h"

#include <functional>

namespace ora
{
    typedef std::function<void(IEventSender *, uint32, VariantVector &)> ListenerFunction;
    
    class EventListenerWraper :
        public IEventListener,
        public IReferenceCount
    {
        MAKE_UNCOPYABLE(EventListenerWraper);
    public:
    
        EventListenerWraper(ListenerFunction fun);
        ~EventListenerWraper();
        
        void onEvent(IEventSender * sender, uint32 eventType, VariantVector & args) override;
        
    private:
        ListenerFunction function_;
    };
    
    typedef SmartPtr<EventListenerWraper> EventListenerWraperPtr;
    
    inline EventListenerWraperPtr newListener(void(*func)(IEventSender *, uint32, VariantVector &))
    {
        return new EventListenerWraper(func);
    }
    
    template<typename T>
    EventListenerWraperPtr newListener(T * listener, void(T::*func)(IEventSender *, uint32, VariantVector &))
    {
        auto f = std::bind(func, listener, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        return new EventListenerWraper(f);
    }
    
} // end namespace ora

#endif /* defined(__orange__event_listener__) */
