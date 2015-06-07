//
//  log_to_file.h
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#ifndef __orange__log_to_file__
#define __orange__log_to_file__

#include "object.h"
#include "file_tool.h"

namespace ora
{
    class LogFile : public Object
    {
    public:
        LogFile();
        ~LogFile();
        
        bool init(const std::string & filename);
        void fini();
        
        void onEvent(IEventSender *sender, uint32 eventType, VariantVector & args) override;
        
        void log(const std::string & msg);
        
    private:
        void write(const std::string & text);
    
        FilePtr _stream;
    };
    
}

#endif /* defined(__orange__log_to_file__) */
