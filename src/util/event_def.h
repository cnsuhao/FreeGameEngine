//
//  event_def.h
//  orange
//
//  Created by zhoubao on 14-6-10.
//
//

#ifndef orange_event_def_h
#define orange_event_def_h

namespace ora
{
    namespace ET
    {
        const uint32 None = 0;
        const uint32 Log = 1;
        const uint32 Exit = 2;
        const uint32 Destroy = 3;
        const uint32 SystemEvent = 4;
        
        const uint32 FrameBeg = 5;
        const uint32 TickBeg = 6;
        const uint32 TickEnd = 7;
        const uint32 RenderBeg = 8;
        const uint32 RenderEnd = 9;
        const uint32 FrameEnd = 10;
    
        const uint32 Keyboard           = 101;
        const uint32 Touch              = 102;
        const uint32 MouseButton        = 106;
        const uint32 MouseMove          = 107;
        const uint32 MouseWheel         = 108;
        const uint32 TextEditing        = 1010;
        const uint32 TextInput          = 1011;
        const uint32 Window             = 1012;
        
    }// end namespace ET
    
} // end namespace ora


#endif
