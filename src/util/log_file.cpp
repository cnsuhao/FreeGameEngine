//
//  log_to_file.cpp
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#include "assert_tool.h"

#include "log_file.h"
#include "time_tool.h"
#include "event_def.h"

#include "resmgr/bwresource.hpp"
#include "resmgr/multi_file_system.hpp"

namespace ora
{
    LogFile::LogFile()
        : stream_(nullptr)
    {
    }
    
    LogFile::~LogFile()
    {
    }
    
    bool LogFile::init(const std::string & filename)
    {
        ASSERT_1(!stream_);
        
        stream_ = BWResource::instance().fileSystem()->posixFileOpen(filename, "w");
        if(stream_)
        {
            subscribeEvent(ET::Log);
            
            write("----logfile system start----");
        }
        
        return stream_ != NULL;
    }
    
    void LogFile::fini()
    {
        if(stream_)
        {
            write("----logfile system stop----");
            fclose(stream_);
            stream_ = nullptr;
        }
    }
    
    void LogFile::onEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        if(eventType == ET::Log)
        {
            int lvl;
            std::string msg;
            if (parse_arguments(args, &lvl, &msg))
            {
                log(lvl, msg);
            }
        }
    }
    
    void LogFile::log(int lvl, const std::string & msg)
    {
        write( time2desc(getWorldTime()) );
        write( getLogPrefix(lvl) );
        write( msg );
        write( "\n" );
        
        //stream_->flush();
    }
    
    void LogFile::write(const std::string & text)
    {
        if(!text.empty())
        {
            fwrite(text.c_str(), text.length(), 1, stream_);
        }
    }
    
} // end namespace ora