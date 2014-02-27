#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
: m_pTestController(nullptr)
{
    
}

HelloWorld::~HelloWorld()
{
    
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto prevItem = MenuItemImage::create("gui/b1.png", "gui/b2.png", CC_CALLBACK_1(HelloWorld::menuPrevTestCallback, this));
    auto restartItem = MenuItemImage::create("gui/r1.png", "gui/r2.png", CC_CALLBACK_1(HelloWorld::menuRestartTestCallback, this));
    auto nextItem = MenuItemImage::create("gui/f1.png", "gui/f2.png", CC_CALLBACK_1(HelloWorld::menuNextTestCallback, this));
    
    int start = 150;
    prevItem->setPosition(start, prevItem->getContentSize().height);
    start += prevItem->getContentSize().width;
    
    restartItem->setPosition(start, restartItem->getContentSize().height);
    start += restartItem->getContentSize().width + 30;
    
    nextItem->setPosition(start, nextItem->getContentSize().height);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, prevItem, restartItem, nextItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    m_pTitle = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    m_pTitle->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - m_pTitle->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(m_pTitle, 1);
    

    C3DLayer * pLayer = C3DLayer::create();
    this->addChild(pLayer);
    
    C3DCamera *pCamera = C3DCamera::createPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    pCamera->setPosition(0, 2.0, 6.0f);
    pCamera->rotateX(-3.14f / 8);
    C3DScene *pScene = pLayer->get3DScene();
    pScene->addNodeToRenderList(pCamera);
    pScene->setActiveCamera(0);

    my3d::renderDev()->setView(pCamera->getViewMatrix());
    my3d::renderDev()->setProjection(pCamera->getProjectionMatrix());
    
    m_pTestController = TestController::create();
    pScene->addChild(m_pTestController);
    
    m_pTitle->setString(m_pTestController->getCurTestDesc());
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::menuPrevTestCallback(Object *pSender)
{
    m_pTestController->goPrevTest();
    m_pTitle->setString(m_pTestController->getCurTestDesc());
}

void HelloWorld::menuNextTestCallback(Object *pSender)
{
    m_pTestController->goNextTest();
    m_pTitle->setString(m_pTestController->getCurTestDesc());
}

void HelloWorld::menuRestartTestCallback(Object *pSender)
{
    m_pTestController->restartTest();
    m_pTitle->setString(m_pTestController->getCurTestDesc());
}


//////////////////////////////////////////////////////////
