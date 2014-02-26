//
//  testtool.h
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#ifndef __dydg_b__testtool__
#define __dydg_b__testtool__

void doTestCase(bool cond, const char * desc, const char *file, int line);
void doAssert(bool cond, const char * desc, const char *file, int line);

#define DO_TEST_CASE(cond, desc) doTestCase(cond, desc, __FILE__, __LINE__)

#if COCOS2D_DEBUG > 0
#define DO_ASSERT(cond, desc) \
do{\
    doAssert(bool(cond), desc, __FILE__, __LINE__);\
    CC_ASSERT(cond);\
}while(0)
#else
#define DO_ASSERT(cond, desc)
#endif

#endif /* defined(__dydg_b__testtool__) */
