//
//  Application.cpp
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#include "application.h"

#include "engine.h"

namespace ora
{
    IApplication::IApplication()
    {
        Engine::initInstance();
    }
    
    IApplication::~IApplication()
    {
        Engine::finiInstance();

#ifndef _RELEASE
        RefHeader::dumpAllLinkedObjects();
#endif
    }
    
    
    int IApplication::run()
    {
        if(!Engine::instance()->preInit())
            return -1;
        
        if(!onSetup())
            return -1;
        
        if (Engine::instance()->init())
        {
            if (onStart())
            {
                Engine::instance()->mainLoop();
            }
            onStop();
        }

        Engine::instance()->fini();
        return 0;
    }
    
} // end namespace ora
