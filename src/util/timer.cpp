//
//  call_back.cpp
//  orange
//
//  Created by Kevin on 14-6-10.
//
//

#include "timer.h"
#include "assert_tool.h"
#include "log_tool.h"
#include "time_tool.h"
#include "watcher.h"

namespace ora
{
    uint32  MAX_TIMER_DEAD_SIZE = 100;
    static uint64 g_timerCookie = 0;

    struct FindTimerNode
    {
        TimerHandle handle_;
        
        FindTimerNode(TimerHandle handle)
            : handle_(handle)
        {}
        
        inline bool operator()(const TimerNode & node) const
        {
            return handle_ == node.handle_;
        }
    };


    TimerNode::TimerNode(float interval, TimerFunction function, uint32 loop, TimerHandle handle):
        interval_(interval),
        handle_(handle),
        function_(function),
        loop_(loop)
    {
        time_ = getTickTime() + interval_;
        cookie_ = g_timerCookie++;
    }
    
    bool TimerNode::ifTimeExpire() const
    {
        return time_ <= getTickTime();
    }
    
    //////////////////////////////////////////////////////////////////////
    /// Timer
    //////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(Timer);
    
    Timer::Timer()
        : handle_(INVALID_TIMER_HANDLE)
        , deadTimers_(0)
    {
        timeQueue_.getContainer().reserve(256);

        ADD_WATCHER("time/timers", &timeQueue_, &TimerQueue::size);
    }
    
    Timer::~Timer()
    {
        timeCache_.clear();
        DEL_WATCHER("time/timers");
    }
    
    TimerHandle Timer::addTimer(float delay, TimerFunction callback, bool isLoop/*=false*/)
    {
        TimerHandle handle = allocateHandle();
        TimerNode node(delay, callback, isLoop, handle);
        timeCache_.push_back(node);
        return handle;
    }
    
    void Timer::delTimer(TimerHandle handle)
    {
        ++deadTimers_;
        
        TimerCache::iterator it1 = std::find_if(timeCache_.begin(), timeCache_.end(), FindTimerNode(handle));
        if (it1 != timeCache_.end())
        {
            it1->cancel();
            return; //直接返回。在syncCache()中会跳过无效的数据。
        }

        //!!!这里必须是引用
        TimerQueue::container_type & nodes = timeQueue_.getContainer();
        TimerQueue::container_type::iterator it2 = std::find_if(nodes.begin(), nodes.end(), FindTimerNode(handle));
        if (it2 != nodes.end())
        {
            it2->cancel();
            return;
        }
        
        --deadTimers_; //revert the last ++dead_;
        ORA_LOG_WARN("Cannot delTimer handle is %d\n", handle);
    }
    
    void Timer::syncCache()
    {
        if ( !timeCache_.empty() )
        {
            for (TimerCache::value_type &iter : timeCache_)
            {
                if (!iter.ifCanceled())
                    timeQueue_.push(iter);
            }
            timeCache_.clear();
        }
        
        if (deadTimers_ >= MAX_TIMER_DEAD_SIZE)
        {
            deadTimers_ = 0;
            
            TimerQueue::container_type & nodes = timeQueue_.getContainer();
            auto iterStart = std::remove_if(nodes.begin(), nodes.end(), FindTimerNode(INVALID_TIMER_HANDLE));
            nodes.erase(iterStart, nodes.end());
            
            timeQueue_.make();
        }
    }
    
    TimerHandle Timer::allocateHandle()
    {
        TimerHandle handle = ++handle_;
        
        if (handle == INVALID_TIMER_HANDLE)
        {
            ORA_LOG_WARN("Timer: the handle was overflow");
            handle = ++handle_;
        }
        
        return handle;
    }
    
    void Timer::tick()
    {
        syncCache();
        
        while ( !timeQueue_.empty() )
        {
            if ( timeQueue_.top().ifCanceled() )
            {
                timeQueue_.pop();
            }
            else if ( timeQueue_.top().ifTimeExpire() )
            {
                TimerNode node = timeQueue_.top();
                timeQueue_.pop();

                if (node.ifLoop())
                {
                    TimerNode newNode(node.interval_, node.function_, node.loop_, node.handle_);
                    timeCache_.push_back(newNode);//添加到缓存池，以防死循环
                }
                
                node.function_();
            }
            else
            {
                break;
            }
        }
    }
    
}; // end of namepsace ora
