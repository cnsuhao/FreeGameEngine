//
//  Application.h
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#ifndef __liborange__Application__
#define __liborange__Application__

#include "util/object.h"
#include "main_def.h"

namespace ora
{
    class IApplication : public Object
    {
    public:
        
        IApplication();
        virtual ~IApplication();
        
        /** 引擎初始化之前回调 */
        virtual bool onSetup() { return true; }
        
        /** 引擎初始化完毕后回调 */
        virtual bool onStart() { return true; }
        
        virtual void onStop() {}
        
        int run();
    };
}

#define DEFINE_APPLICATION_MAIN(className) \
int runApplication() \
{ \
    SmartPtr<className> application(new className()); \
    return application->run(); \
} \
DEFINE_MAIN(runApplication);

#endif /* defined(__liborange__Application__) */
