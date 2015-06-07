//
//  event_listener.cpp
//  orange
//
//  Created by zhoubao on 14-6-11.
//
//

#include "event_listener.h"

namespace ora
{
    EventListenerWraper::EventListenerWraper(ListenerFunction fun)
        : function_(fun)
    {
    }
    
    EventListenerWraper::~EventListenerWraper()
    {
    }
    
    void EventListenerWraper::onEvent(IEventSender * sender, uint32 eventType, VariantVector & args)
    {
        function_(sender, eventType, args);
    }
    
} // end namespace ora
