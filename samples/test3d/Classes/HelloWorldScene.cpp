#include "HelloWorldScene.h"
#include "cocos3d.h"
#include "my3d/my3d.h"

#include "TestEGLDraw.h"
#include "TestSimpleDraw.h"
#include "TestTextureDraw.h"
#include "TestMeshDraw.h"
#include "TestGroundDraw.h"



USING_NS_CC;


//#define TEST_EGL

typedef TestBaseNode* (*CreateTestFun)();

class Test3DNode : public TestBaseNode
{
    
    std::vector<std::pair<std::string, CreateTestFun>> m_testFactory;
    int m_curTestIndex;
    TestBaseNode * m_curTestNode;
    
    
    void addTestFactoryMethod(const std::string & desc, CreateTestFun method)
    {
        m_testFactory.push_back(std::make_pair(desc, method));
    }
    
    TestBaseNode * createTestNode(int index)
    {
        return (m_testFactory[index].second)();
    }
    
    
public:
    
    Test3DNode()
    : m_curTestNode(nullptr)
    , m_curTestIndex(0)
    {
        
    }
    
    ~Test3DNode()
    {
    }
    
    CREATE_TEST_3D_NODE(Test3DNode);
    
    bool initTest3D()
    {
        m_testFactory.clear();
        
        addTestFactoryMethod("opengl es", CreateTestFun(TestEGLDrawNode::create));
        addTestFactoryMethod("simple cube", CreateTestFun(TestSimpleDrawNode::create));
        addTestFactoryMethod("texture cube", CreateTestFun(TestTextureDrawNode::create));
        addTestFactoryMethod("mesh", CreateTestFun(TestMeshDrawNode::create));
        addTestFactoryMethod("ground", CreateTestFun(TestGroundDrawNode::create));
        addTestFactoryMethod("complex scene", CreateTestFun(TestGroundDrawNode::create));
        
        restartTest();
        
        return true;
    }
    
    virtual void draw() override
    {
        my3d::renderDev()->setRenderState(my3d::RenderState::CullFace, true);
        my3d::renderDev()->setCullFace(my3d::CullFace::Back);
        my3d::renderDev()->pushWorld(this->getWorldMatrix());

        TestBaseNode::draw();
      
        my3d::renderDev()->popWorld();
    }
    
    void goNextTest()
    {
        ++m_curTestIndex;
        m_curTestIndex %= m_testFactory.size();
        
        restartTest();
    }
    
    void goPrevTest()
    {
        --m_curTestIndex;
        if(m_curTestIndex < 0)
            m_curTestIndex = m_testFactory.size() - 1;
        
        restartTest();
    }
    
    void restartTest()
    {
        if(m_curTestNode)
            this->removeChild(m_curTestNode);
        
        m_curTestNode = createTestNode(m_curTestIndex);
        this->addChild(m_curTestNode);
    }
    
    const size_t getNumTotalTest() const
    {
        return m_testFactory.size();
    }
    
    const std::string & getTestDesc(int index) const
    {
        return m_testFactory[index].first;
    }
    
    const std::string & getCurTestDesc() const
    {
        return m_testFactory[m_curTestIndex].first;
    }
    
};


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

#if 0
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
#endif
    
#if 1
    C3DLayer * pLayer = C3DLayer::create();
    this->addChild(pLayer);
    
    C3DCamera *pCamera = C3DCamera::createPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
    pCamera->setPosition(0, 4.0, 20.0f);
    pCamera->rotateX(-3.14f / 8);
    C3DScene *pScene = pLayer->get3DScene();
    pScene->addNodeToRenderList(pCamera);
    pScene->setActiveCamera(0);

    my3d::renderDev()->setView(pCamera->getViewMatrix());
    my3d::renderDev()->setProjection(pCamera->getProjectionMatrix());
    
    Test3DNode * pNode = Test3DNode::create();
    pScene->addChild(pNode);
    
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
    
    Layer::draw();
}

//////////////////////////////////////////////////////////
