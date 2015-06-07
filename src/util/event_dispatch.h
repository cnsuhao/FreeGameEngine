//
//  event_dispatch.h
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#ifndef __orange__event_dispatch__
#define __orange__event_dispatch__

#include "variant.h"
#include "singleton.h"
#include <map>
#include <set>

namespace ora
{
    class IEventListener;
    
    class IEventSender
    {
    public:
        
        IEventSender();
        virtual ~IEventSender();
        
        void sendEvent(uint32 eventType, VariantVector & args);
        
#if 0
        template<typename ...ARGS>
        void sendEventVS(uint32 eventType, ARGS ...args)
        {
            VariantVector argList;
            build_arguments(argList, args...);
            sendEvent(eventType, argList);
        }
#endif
        
    private:
        void dispatchEvent(IEventSender *sender, uint32 eventType, VariantVector & args);
        
        void addListener(uint32 eventType, IEventListener *listener);
        void delListener(uint32 eventType, IEventListener *listener);
        
        typedef std::map<uint32, std::set<IEventListener*>> ListenersMap;
        ListenersMap listeners_;
        
        friend class IEventListener;
    };
    
    
    /** 全局sender。
     *  所有的事件，最终都会转发给global sender，然后由global sender广播给所有的global listener。
     */
    class GlobalSender :
        public IEventSender,
        public Singleton<GlobalSender>
    {
    public:
        GlobalSender();
        ~GlobalSender();
        
    };
    
    /** 事件监听者。
     *  事件由sender发起，通过onEvent接口通知listener。
     *  如果要监听事件，可以派生IEventListener，实现onEvent接口；或者，调用newListener，传入函数指针。
     */
    class IEventListener
    {
    public:
        
        IEventListener();
        virtual ~IEventListener();
        
        virtual void onEvent(IEventSender *sender, uint32 eventType, VariantVector & args){}
        
        /** 监听sender。如果sender为null，将监听全局的sender。*/
        void subscribeEvent(uint32 eventType, IEventSender *sender=nullptr);
    
        /** 取消监听sender。如果sender为null，仅取消全局监听。*/
        void unsubscribeEvent(uint32 eventType, IEventSender *sender=nullptr);
        
    private:
        typedef std::map<uint32, std::set<IEventSender*>> SendersMap;
        SendersMap senders_;
    };
    
} // end namespace ora

#endif /* defined(__orange__event_dispatch__) */
