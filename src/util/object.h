//
//  object.h
//  liborange
//
//  Created by zhoubao on 14-6-5.
//
//

#ifndef __liborange__object__
#define __liborange__object__

#include "event_dispatch.h"

namespace ora
{
    //TODO 减少多继承。导出脚本的时候，多继承会带来麻烦。
    
    class Object :
        public IReferenceCount,
        public IEventListener,
        public IEventSender
    {
    public:
        Object();
        ~Object();
        
    };
    
} // end namespace ora

#endif /* defined(__liborange__object__) */
