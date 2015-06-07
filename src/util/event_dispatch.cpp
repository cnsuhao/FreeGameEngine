//
//  event_dispatch.cpp
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#include "event_dispatch.h"
#include "assert_tool.h"
#include "log_tool.h"

namespace ora
{
    
    IEventSender::IEventSender()
    {
    }
    
    /** sender析构的时候，需要通知所有listener撤销监听。*/
    IEventSender::~IEventSender()
    {
        ListenersMap temp;
        temp.swap(listeners_);
        
        ListenersMap::iterator it = temp.begin();
        for(; it != temp.end(); ++it)
        {
            for(IEventListener * listener : it->second)
            {
                listener->unsubscribeEvent(it->first, this);
            }
        }
    }
    
    void IEventSender::addListener(uint32 eventType, IEventListener *listener)
    {
        listeners_[eventType].insert(listener);
    }
    
    void IEventSender::delListener(uint32 eventType, IEventListener *listener)
    {
        ListenersMap::iterator it = listeners_.find(eventType);
        if(it != listeners_.end())
        {
            it->second.erase(listener);
        }
    }
    
    void IEventSender::sendEvent(uint32 eventType, VariantVector & args)
    {
        dispatchEvent(this, eventType, args);
        
        if(this != GlobalSender::instance())
        {
            GlobalSender::instance()->dispatchEvent(this, eventType, args);
        }
    }
    
    void IEventSender::dispatchEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        ListenersMap::iterator it = listeners_.find(eventType);
        if(it != listeners_.end() && !it->second.empty())
        {
            //拷贝一个set副本。因为在遍历的过程中，listener可能会被删除，引发迭代器失效。
            std::set<IEventListener*> copied = it->second;
            for(IEventListener * listener : copied)
            {
                listener->onEvent(sender, eventType, args);
            }
        }
    }
    
    IMPLEMENT_SINGLETON(GlobalSender);
    GlobalSender::GlobalSender()
    {
        ORA_STACK_TRACE;
    }
    
    GlobalSender::~GlobalSender()
    {
        ORA_STACK_TRACE;
    }

    
    IEventListener::IEventListener()
    {
    }
    
    /** listener析构的时候，需要撤销自己曾注册过的所有监听。*/
    IEventListener::~IEventListener()
    {
        SendersMap temp;
        temp.swap(senders_);
        
        SendersMap::iterator it = temp.begin();
        for(; it != temp.end(); ++it)
        {
            for(IEventSender *sender : it->second)
            {
                sender->delListener(it->first, this);
            }
        }
    }
    
    /** 监听特定发送者的事件*/
    void IEventListener::subscribeEvent(uint32 eventType, IEventSender *sender)
    {
        if(sender == nullptr)
            sender = GlobalSender::instance();
        
        sender->addListener(eventType, this);
        
        senders_[eventType].insert(sender);
    }
    
    /** 取消监听特定发送者的事件*/
    void IEventListener::unsubscribeEvent(uint32 eventType, IEventSender *sender)
    {
        if(sender == nullptr)
            sender = GlobalSender::instance();
        
        sender->delListener(eventType, this);
        
        SendersMap::iterator it = senders_.find(eventType);
        if(it != senders_.end())
        {
            it->second.erase(sender);
        }
    }
    
}// end namespace ora
