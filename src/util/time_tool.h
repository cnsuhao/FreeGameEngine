//
//  time_tool.h
//  orange
//
//  Created by zhoubao on 14-6-9.
//
//

#ifndef __orange__time_tool__
#define __orange__time_tool__

#include <ctime>
#include <string>
#include "util_config.h"

namespace ora
{
    /** 世界时间。即格林威治时间，单位为秒。*/
    time_t getWorldTime();
    
    /** 本地时间。即本机时间，单位为秒。*/
    time_t getLocalTime();
    
    /** 游戏时间。即游戏服务器的时间，单位为秒。*/
    time_t getGameTime();
    
    /** tick时间。进程启动到现在，所经过的时间。单位为秒。*/
	float getTickTime();

    /** tick时间。进程启动到现在，所经过的时间。单位为毫秒。*/
	uint32 getTickTime_ms();
    
    std::string time2desc(time_t t);
    
} // end namesapce ora

#endif /* defined(__orange__time_tool__) */
