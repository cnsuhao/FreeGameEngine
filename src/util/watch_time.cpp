//
//  watch_time.cpp
//  liborange
//
//  Created by zhoubao on 14-5-9.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef _RELEASE

#include "watch_time.h"
#include "log_tool.h"
#include "assert_tool.h"
#include "time_tool.h"

#include "math/math.hpp"

namespace ora
{

    NumberNode::NumberNode()
        : minElapse_(FloatMax)
        , maxElapse_(FloatMin)
        , accumulate_(0.0f)
        , counter_(0)
        , startTime_(-1.0f)
    {
    }

    void NumberNode::watchValue(float value)
    {
        ++counter_;

        if (minElapse_ > value)
            minElapse_ = value;

        if(maxElapse_ < value)
            maxElapse_ = value;
        
        accumulate_ += value;
    }

	float NumberNode::getAverage() const
    {
        if(counter_ == 0) return accumulate_;
        
        return accumulate_ / counter_;
    }
    
    void NumberNode::watchTimeBegin()
    {
        ASSERT_1(startTime_ < 0.0f);
        
        startTime_ = getTickTime();
    }
    
    void NumberNode::watchTimeEnd()
    {
        ASSERT_1(startTime_ >= 0.0f);
        
        watchValue(getTickTime() - startTime_);
        
        startTime_ = -1.0f;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    ScopedTimeNode::ScopedTimeNode(NumberNode & node)
        : timeNode_(node)
    {
        timeNode_.watchTimeBegin();
    }
    
    ScopedTimeNode::~ScopedTimeNode()
    {
        timeNode_.watchTimeEnd();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(WatchTime);
    
    WatchTime::WatchTime()
    {
    }
    
    WatchTime::~WatchTime()
    {
    }

    NumberNode & WatchTime::get(const std::string & name)
    {
        return pool_[name];
    }
    
    void WatchTime::clear()
    {
        pool_.clear();
    }
    
}// end namespace ora

#endif
