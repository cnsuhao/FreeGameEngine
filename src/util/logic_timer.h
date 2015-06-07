#ifndef ORA_STABLE_TIMER_H
#define ORA_STABLE_TIMER_H

#define LOGIC_TIMER_USE_FIXED_TICK 1

#if LOGIC_TIMER_USE_FIXED_TICK
#include "util_config.h"
#else
#include "event_listener.h"
#endif

#include <queue>
#include <set>

namespace ora
{

class LogicTimer;

typedef uint64 LGTimeVal;
typedef uint64 LGTimerHandle;


class TimerCallback
{
public:
    virtual void callback(LogicTimer & system, LGTimerHandle handle) = 0;
    virtual void finished() = 0;
};


class LogicTimer
{
public:

    LogicTimer(LGTimeVal interval);
    virtual ~LogicTimer();

    void checkTimers( LGTimeVal timeNow );
    LGTimerHandle addTimer(LGTimeVal start, TimerCallback * cb, LGTimeVal interval = 0);
    void cancelTimer( LGTimerHandle handle );

    LGTimeVal time() { return lastTime_; }

    void play();
    void stop();
    void clear();

private:

    class Timer
    {
        LGTimeVal       time_;
        LGTimeVal       interval_;
        LGTimerHandle   handle_;
        TimerCallback * pCallback_;
        uint64          cookie_;

        static uint64  lastcookie;
    public:
        Timer(LGTimeVal time, LGTimeVal interval, LGTimerHandle handle, TimerCallback * pCB)
            : time_(time), interval_(interval), handle_(handle), pCallback_(pCB)
        {
            cookie_ = ++lastcookie;
        }

        bool operator<( const Timer & other ) const
        {
            if (time_ != other.time_)
                return time_ > other.time_;

            return cookie_ > other.cookie_;
        }

        LGTimeVal time() const { return time_; }
        LGTimeVal interval() const { return interval_; }
        bool isLoopedTimer() const { return interval_ != 0; }
        TimerCallback * pCB() const { return pCallback_; }
        LGTimerHandle handle() const { return handle_; }
    };

    void purgeCancelled();

#if LOGIC_TIMER_USE_FIXED_TICK
    void onTick();

    LGTimeVal                   tickInterval_;
    uint32                      tickHandle_;
#else
    void onTickEvent(ora::IEventSender *sender, uint32 eventType, ora::VariantVector & args);

    ora::EventListenerWraperPtr tickListener_;
#endif
    
    std::priority_queue<Timer>  timers_;
    std::set<LGTimerHandle>     cancelledTimers_;
    LGTimerHandle               lasthandle_;
    LGTimeVal                   lastTime_;
    LGTimeVal                   realTime_;
};

}

#endif //ORA_STABLE_TIMER_H
