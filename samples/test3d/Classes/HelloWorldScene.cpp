#include "HelloWorldScene.h"
#include "cocos3d.h"
#include "Vertex.h"
#include "C3DVertexFormat.h"

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
: m_vertexbuffer(nullptr)
{
    
}

HelloWorld::~HelloWorld()
{
    delete m_vertexbuffer;
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

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
#if 1
    C3DLayer * pLayer = C3DLayer::create();
    this->addChild(pLayer);
    
    C3DCamera *pCamera = C3DCamera::createPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    C3DScene *pScene = pLayer->get3DScene();
    pScene->addNodeToRenderList(pCamera);
    pScene->setActiveCamera(0);
    
    const float size = 100.0f;
    const float pz = -10.0f;
    const int numVertices = 4;
    BBVertex vertices[numVertices];
    vertices[0].position.set(-size, -size, pz);
    vertices[1].position.set(-size, size, pz);
    vertices[2].position.set(size, -size, pz);
    vertices[3].position.set(size, size, pz);
    
    m_vertexbuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
                                            numVertices * sizeof(BBVertex), vertices);
    

    
#endif
    
    return true;
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::draw()
{
    if(m_vertexbuffer != nullptr)
    {
        m_vertexbuffer->bind();
        
        m_vertexbuffer->unbind();
    }
    
    Layer::draw();
}

