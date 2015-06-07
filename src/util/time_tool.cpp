//
//  time_tool.cpp
//  orange
//
//  Created by zhoubao on 14-6-9.
//
//

#include "time_tool.h"

#include <SDL/include/SDL.h>

namespace ora
{
    
    time_t getWorldTime()
    {
        return time(nullptr);
    }
    
    time_t getLocalTime()
    {
        return time(nullptr) - timezone;
    }
    
    time_t getGameTime()
    {
        //TODO make this time sync with server.
        return getWorldTime();
    }
    
	float getTickTime()
    {
		return getTickTime_ms() * 0.001f;
    }
    
	uint32 getTickTime_ms()
	{
		return SDL_GetTicks();
	}

    std::string time2desc(time_t t)
    {
        return ctime(&t);
    }
    
} // end namespace ora