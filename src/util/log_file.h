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
        
        void log(int lvl, const std::string & msg);
        
    private:
        void write(const std::string & text);
    
        FILE *stream_;
    };
    
}

#endif /* defined(__orange__log_to_file__) */
