#include "logic_timer.h"
#include "watcher.h"

#if LOGIC_TIMER_USE_FIXED_TICK
#include "timer.h"
#else
#include "event_def.h"
#endif

#include <list>

namespace ora
{

static int s_numTimer = 0;

/*static*/ uint64 LogicTimer::Timer::lastcookie = 1;

void LogicTimer::checkTimers( LGTimeVal timeNow )
{
	// assert timeNow >= lastTime
	
	while(!timers_.empty() && timers_.top().time() < timeNow)
	{
        Timer node = timers_.top();
		timers_.pop();
        --s_numTimer;

        std::set< LGTimerHandle >::iterator found = cancelledTimers_.find(node.handle());

        TimerCallback * pCb = node.pCB();
        lastTime_ = node.time();

		if (found == cancelledTimers_.end())
		{
            // push it to queue agin, and then call the callback, will avoid the 'dead timer'.
            // because sometimes we need cancel the looped timer in callback.
            if (node.isLoopedTimer())
            {
                ++s_numTimer;
                timers_.push( Timer(time() + node.interval(), node.interval(), node.handle(), pCb) );
            }

            pCb->callback(*this, node.handle());

            if (!node.isLoopedTimer())
            {
                pCb->finished();
            }
		}
        else
        {
            cancelledTimers_.erase( found );
		    pCb->finished();
        }
	}

	lastTime_ = timeNow;
}

LGTimerHandle LogicTimer::addTimer(LGTimeVal start, TimerCallback * cb, LGTimeVal interval)
{
	// assert time >= lastTime
	LGTimerHandle newHandle = lasthandle_++;
    timers_.push(Timer(lastTime_ + start, interval, newHandle, cb));
    ++s_numTimer;
	return newHandle;
}

void LogicTimer::cancelTimer( LGTimerHandle handle )
{
	if (handle >= lasthandle_)
		return;

	// Mark this one to be skipped
	cancelledTimers_.insert( handle );

	// If there are too damn many cancelled timers, deal with it.
	if (cancelledTimers_.size() * 2 > timers_.size() + 4)
	{
		purgeCancelled();
	}
	else if (!timers_.empty() && timers_.top().handle() == handle)
	{
		// Otherwise, just ensure the next timer is valid
		while( !timers_.empty())
		{
            std::set< LGTimerHandle >::iterator found = cancelledTimers_.find(timers_.top().handle());
			if (found == cancelledTimers_.end())
				break;

			cancelledTimers_.erase( found );
			timers_.top().pCB()->finished();
			timers_.pop();
            --s_numTimer;
		}
	}
}

void LogicTimer::purgeCancelled()
{
	std::list< Timer > newTimers;

	while(!timers_.empty())
	{
		if (cancelledTimers_.count( timers_.top().handle() ) )
			timers_.top().pCB()->finished();
		else
			newTimers.push_back( timers_.top() );
		
        timers_.pop();
        --s_numTimer;
	}

	cancelledTimers_.clear();
	for (Timer & node : newTimers)
	{
		timers_.push( node );
        ++s_numTimer;
	}
}

LogicTimer::LogicTimer(LGTimeVal interval)
    : lasthandle_( 0 )
    , lastTime_( 0 )
    , realTime_(0)

{
    if(s_numTimer == 0)
        ADD_WATCHER("time/logic_timer", s_numTimer);

 #if LOGIC_TIMER_USE_FIXED_TICK
    tickInterval_ = interval;
    tickHandle_ = 0;
#else
    tickListener_ = ora::newListener(this, &LogicTimer::onTickEvent);
#endif
}

LogicTimer::~LogicTimer()
{
    clear();
    stop();
}

void LogicTimer::play()
{
#if LOGIC_TIMER_USE_FIXED_TICK
    if (tickHandle_ == 0)
    {
        auto callback = std::bind(&LogicTimer::onTick, this);
        tickHandle_ = ora::Timer::instance()->addTimer(tickInterval_ * 0.001f, callback, true);
    }
#else
    tickListener_->subscribeEvent(ora::ET::TickBeg);
#endif
}

void LogicTimer::stop()
{
#if LOGIC_TIMER_USE_FIXED_TICK
    if (tickHandle_ != 0 && ora::Timer::hasInstance())
    {
        ora::Timer::instance()->delTimer(tickHandle_);
    }
    tickHandle_ = 0;
#else
    tickListener_->unsubscribeEvent(ora::ET::TickBeg);
#endif
}

void LogicTimer::clear()
{
    while (!timers_.empty())
    {
        timers_.top().pCB()->finished();
        timers_.pop();
        --s_numTimer;
    }
}

#if LOGIC_TIMER_USE_FIXED_TICK
void LogicTimer::onTick()
{
    realTime_ += tickInterval_;
    checkTimers(realTime_);
}
#else
void LogicTimer::onTickEvent(ora::IEventSender *sender, uint32 eventType, ora::VariantVector & args)
{
    uint32 elapse_ms;
    args[1].getv(elapse_ms);

    realTime_ += elapse_ms;
    checkTimers(realTime_);
}
#endif

}
