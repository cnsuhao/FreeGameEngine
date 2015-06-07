//
//  testtool.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef __dydg_b__testtool__
#define __dydg_b__testtool__

#ifndef _RELEASE

namespace ora
{

    void doTestCase(bool cond, const char * desc, const char *file, int line);

}//end namespace ora

#define DO_TEST_CASE(cond, desc) ora::doTestCase(cond, desc, __FILE__, __LINE__)

#else

#define DO_TEST_CASE(cond, desc)

#endif

#endif /* defined(__dydg_b__testtool__) */
