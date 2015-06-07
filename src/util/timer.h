//
//  call_back.h
//  orange
//
//  Created by Kevin on 14-6-10.
//
//

#ifndef __orange__call_back__
#define __orange__call_back__

#include "util_config.h"
#include "priority_queue.h"
#include "singleton.h"

#include <cmath>

namespace ora
{
    
    typedef std::function<void()> TimerFunction;
    typedef uint32 TimerHandle;
    
    const TimerHandle   INVALID_TIMER_HANDLE = 0;
    const TimerHandle   MAX_TIMER_HANDLE = 0xffffffff;

    extern uint32       MAX_TIMER_DEAD_SIZE;

    
    class TimerNode
    {
    public:

        uint64          cookie_;
        float           time_;
        float           interval_;
        TimerHandle     handle_;
        TimerFunction   function_;
        uint32          loop_;

    
        TimerNode(float interval, TimerFunction function, uint32 loop, TimerHandle handle);
        
        bool ifTimeExpire() const;
        bool ifLoop() const { return loop_ != 0; }
        bool ifCanceled() const { return handle_ == INVALID_TIMER_HANDLE; }
        
        void cancel(){ handle_ = INVALID_TIMER_HANDLE; }
        
        bool operator < (const TimerNode & right) const
        {
            if (std::abs(this->time_ - right.time_) < 0.00001f)
                return this->cookie_ > right.cookie_;

            return this->time_ > right.time_;
        }
    };
    
    class Timer : public Singleton<Timer>
    {
    private:
        
        typedef PriorityQueue<TimerNode> TimerQueue;
        typedef std::vector<TimerNode> TimerCache;

        TimerQueue  timeQueue_;
        TimerCache  timeCache_;
        TimerHandle handle_;
        uint32      deadTimers_;
        
        /// 同步cache到queue中
        void syncCache();
        
        TimerHandle allocateHandle();
        
    public:
        Timer();
        ~Timer();
        
        /// 添加一个timer到cache中
        TimerHandle addTimer(float delay, TimerFunction callback, bool isLoop=false);

        /// 从queue和cache中删除一个timer
        void delTimer(TimerHandle handle);

        void tick();
    };
    
}; // end of namespace ora;

#endif /* defined(__orange__call_back__) */
