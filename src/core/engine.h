//
//  engine.h
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#ifndef __orange__engine__
#define __orange__engine__

#include "util/object.h"
#include "util/log_file.h"

namespace ora
{
    class Engine :
        public Object,
        public Singleton<Engine>
    {
    public:
    
        Engine();
        ~Engine();
        
        /** must called before init.*/
        bool preInit();
        
        bool init();
        void fini();
        
        void mainLoop();
        
        void onEvent(IEventSender *sender, uint32 eventType, VariantVector & args) override;
        
        void quit(){ exit_ = true; }
        
    private:
        bool                exit_;
        SmartPtr<LogFile>   logfile_;
    };
    
} // end namespace ora

#endif /* defined(__orange__engine__) */
