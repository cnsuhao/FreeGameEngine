//
//  watch_time.h
//  liborange
//
//  Created by zhoubao on 14-5-9.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__watch_time__
#define __liborange__watch_time__

#ifndef _RELEASE

#include "util_config.h"
#include "singleton.h"

#include <map>

namespace ora
{
    class NumberNode
    {
    public:
        NumberNode();

        void watchValue(float value);

        float getAverage() const;
        float getMinElapse() const { return minElapse_; }
        float getMaxElapse() const { return maxElapse_; }
        float getAccumulate() const { return accumulate_; }
        int getCounter() const { return counter_; }

        void watchTimeBegin();
        void watchTimeEnd();

    private:
        float   minElapse_;
        float   maxElapse_;
        float   accumulate_;
        int     counter_;

        float startTime_;
    };

    class ScopedTimeNode
    {
        NumberNode & timeNode_;
    public:

        ScopedTimeNode(NumberNode & node);
        ~ScopedTimeNode();
    };


    class WatchTime : public Singleton<WatchTime>
    {
    public:
        typedef std::map<std::string, NumberNode> Pool;

        WatchTime();
        ~WatchTime();

        NumberNode & get(const std::string & name);
        void clear();

        const Pool & getPool() const { return pool_; }

    private:
        Pool pool_;
    };

}// end namespace ora

#define WATCH_TIME(NAME)            ::ora::ScopedTimeNode scoped##NAME(::ora::WatchTime::instance()->get(#NAME))
#define WATCH_TIME_START(NAME)      ::ora::WatchTime::instance()->get(#NAME).watchTimeBegin()
#define WATCH_TIME_STOP(NAME)       ::ora::WatchTime::instance()->get(#NAME).watchTimeEnd()
#define WATCH_NUMBER(NAME, VALUE)   ::ora::WatchTime::instance()->get(#NAME).watchValue(VALUE)

#else //_RELEASE

#define WATCH_TIME(NAME)
#define WATCH_TIME_START(NAME)
#define WATCH_TIME_STOP(NAME)

#endif //_RELEASE

#endif /* defined(__liborange__watch_time__) */
