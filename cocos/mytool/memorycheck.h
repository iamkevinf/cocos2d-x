//
//  memorycheck.h
//  cocos2d_libs
//
//  Created by zhoubao on 14-2-24.
//
//

#ifndef __cocos2d_libs__memorycheck__
#define __cocos2d_libs__memorycheck__

#include <string>

void incObjectCounter(const std::string & name);
void decObjectCounter(const std::string & name);
void dumpAllObjectCounter();

#if COCOS2D_DEBUG > 0
#define CHECK_OBJECT_CONS(p) incObjectCounter(typeid(*p).name())
#define CHECK_OBJECT_DEST(p) decObjectCounter(typeid(*p).name())
#else
#define CHECK_OBJECT_CONS(p)
#define CHECK_OBJECT_DEST(p)
#endif

#endif /* defined(__cocos2d_libs__memorycheck__) */
