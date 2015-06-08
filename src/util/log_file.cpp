//
//  log_to_file.cpp
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#include "log_file.h"
#include "assert_tool.h"
#include "time_tool.h"
#include "event_def.h"

namespace ora
{
    LogFile::LogFile()
    {
    }
    
    LogFile::~LogFile()
    {
    }
    
    bool LogFile::init(const std::string & filename)
    {
        ASSERT_1(!_stream);
        
        _stream = FileSystemMgr::instance()->openFile(filename, IFile::MD_CREATE);
        if(_stream)
        {
            subscribeEvent(ET::Log);
            
            write("----logfile system start----");
        }
        
        return bool(_stream);
    }
    
    void LogFile::fini()
    {
        if(_stream)
        {
            write("----logfile system stop----");
            _stream->close();
            _stream = nullptr;
        }
    }
    
    void LogFile::onEvent(IEventSender *sender, uint32 eventType, VariantVector & args)
    {
        if(eventType == ET::Log)
        {
            std::string msg;
            if(args[1].getv(msg))
            {
                log(msg);
            }
        }
    }
    
    void LogFile::log(const std::string & msg)
    {
        write( time2desc(getWorldTime()) );
        write( msg );
        write( "\n" );
        
        _stream->flush();
    }
    
    void LogFile::write(const std::string & text)
    {
        if(!text.empty())
        {
            _stream->write(text.c_str(), text.length());
        }
    }
    
} // end namespace ora