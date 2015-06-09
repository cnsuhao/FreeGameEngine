//
//  application.h
//  client
//
//  Created by zhoubao on 14-6-11.
//
//

#ifndef __client__application__
#define __client__application__

#include "core/Application.h"

class Application : public ora::IApplication
{
public:
    
    Application();
    ~Application();
    
    virtual void onEvent(ora::IEventSender *sender, uint32 eventType, ora::VariantVector & args) override;
    
private:
    virtual bool onSetup() override;
    virtual bool onStart() override;
    virtual void onStop() override;
};

#endif /* defined(__client__application__) */
