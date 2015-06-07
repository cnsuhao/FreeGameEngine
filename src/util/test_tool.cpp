//
//  testtool.cpp
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef _RELEASE

#include "test_tool.h"
#include "log_tool.h"

namespace ora
{

    void doTestCase(bool cond, const char * desc, const char * file, int line)
    {
        if(cond) ORA_LOG_DEBUG("test succed: %s", desc);

        else ORA_LOG_DEBUG("test failed: %s. file:%s, line:%d", desc, file, line);
    }


}// end namespace ora

#endif
