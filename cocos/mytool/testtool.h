//
//  testtool.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef __dydg_b__testtool__
#define __dydg_b__testtool__

void doTestCase(bool exp, const char * desc, const char * file, int line);

#define DO_TEST_CASE(exp, desc) doTestCase(exp, desc, __FILE__, __LINE__)


#endif /* defined(__dydg_b__testtool__) */
