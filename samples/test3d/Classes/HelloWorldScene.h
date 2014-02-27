#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "TestController.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    HelloWorld();
    ~HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void menuPrevTestCallback(Object *pSender);
    void menuNextTestCallback(Object *pSender);
    void menuRestartTestCallback(Object *pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    cocos2d::LabelTTF *m_pTitle;
    TestController *m_pTestController;
    
};

#endif // __HELLOWORLD_SCENE_H__
