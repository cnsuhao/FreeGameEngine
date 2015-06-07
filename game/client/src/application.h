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

//previous declare
namespace MyGUI
{
    class Gui;
    class OrangePlatform;
    class Widget;
    class EditBox;
}

class CStdMf;
class BWResource;
namespace Moo
{
    class RenderContext;
}

class Application : public ora::IApplication
{
public:
    
    Application();
    ~Application();
    
private:
    virtual bool onSetup() override;
    virtual bool onStart() override;
    virtual void onStop() override;
    
    void onEvent(IEventSender * sender, uint32 eventType, ora::VariantVector & args) override;
    
#ifndef _RELEASE
    void onBtnDebugWatch(MyGUI::Widget *sender);
    void onEdtTextChange(MyGUI::EditBox *sender);
    
    bool loadDebugForm();
    void updateDebugInfo();

    MyGUI::Widget *         debugWatch_;
    int                     watchInterval_;
#endif

private:
    MyGUI::OrangePlatform * myguiPlatform_;
    MyGUI::Gui *            mygui_;

    CStdMf *                pStdmf_;
    BWResource *            pBwresource_;
    Moo::RenderContext*     pRenderContext_;
};

#endif /* defined(__client__application__) */
