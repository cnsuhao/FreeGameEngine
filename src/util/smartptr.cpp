//
//  smartptr.cpp
//  freegame
//
//  Created by C218-pc on 15/6/9.
//
//

#include "smartptr.h"

#include "script/script.h"

namespace ora
{
    int g_ref_counter = 0;
    
    IReferenceCount::IReferenceCount()
        : referenceCount_(0)
        , scriptID_(0)
    {
        ++g_ref_counter;
    }
    
    IReferenceCount::~IReferenceCount()
    {
        --g_ref_counter;
        
        if(scriptID_ != 0)
        {
            ScriptMgr::instance()->removeScriptObjecrt(scriptID_);
        }
    }
    
} // end namespace ora
