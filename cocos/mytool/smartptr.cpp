//
//  smartptr.cpp
//  dydg_b
//
//  Created by zhoubao on 14-2-12.
//
//

#include "cocos2d.h"
#include "smartptr.h"
#include "testtool.h"

void testSmartPtr()
{
    CCLOG("testSmartPtr");
    
    SmartPtr<cocos2d::CCObject> ptr1 = new cocos2d::CCString("test SmartPtr");
    ptr1->release();
    
    {//测试释放
        //CCLOG("test release...");
        
        SmartPtr<cocos2d::CCObject> ptrx = ptr1;
        SmartPtr<cocos2d::CCObject> ptry = ptr1;
        ptry = NULL;
        
        SmartPtr<cocos2d::CCObject> ptrz;
    }
    
    CCLOG("test ==");
    
    //同类型相等比较
    SmartPtr<cocos2d::CCObject> ptr2 = ptr1;
    DO_TEST_CASE(ptr1 == ptr2, "ptr1 == ptr2");
    DO_TEST_CASE(ptr1 == ptr2.get(), "ptr1 == ptr2.get()");
    DO_TEST_CASE(ptr1.get() == ptr2, "ptr1.get() == ptr2");
    ptr2 = NULL;
    
    //同类型不相等比较
    SmartPtr<cocos2d::CCObject> ptr3 = new cocos2d::CCString("str");
    DO_TEST_CASE(ptr1 != ptr3, "ptr1 != ptr3");
    DO_TEST_CASE(ptr1 != ptr3.get(), "ptr1 != ptr3.get()");
    DO_TEST_CASE(ptr1.get() != ptr3, "ptr1.get() != ptr3");
    ptr3 = NULL;
    
    SmartPtr<cocos2d::CCString> ptr4 = ptr1;
    (*ptr4) = "test SmartPtr 2";
    CCLOG("str: %s, len: %d", ptr4->getCString(), ptr4->length());
    
    CCLOG("test ==");
    
    //不同类型相等比较
    DO_TEST_CASE(ptr1 == ptr4, "ptr1 == ptr4");
    DO_TEST_CASE(ptr1 == ptr4.get(), "ptr1 == ptr4.get()");
    DO_TEST_CASE(ptr1.get() == ptr4, "ptr1.get() == ptr4");
    ptr4 = NULL;
    
    //不同类型不相等比较
    SmartPtr<cocos2d::CCInteger> ptr5 = new cocos2d::CCInteger(2);
    DO_TEST_CASE(ptr1 != ptr5, "ptr1 != ptr5");
    DO_TEST_CASE(ptr1 != ptr5.get(), "ptr1 != ptr5.get()");
    DO_TEST_CASE(ptr1.get() != ptr5, "ptr1.get() != ptr5");
    ptr5 = NULL;
    
    DO_TEST_CASE(ptr1->isSingleReference(), "ptr1->isSingleReference");
}

