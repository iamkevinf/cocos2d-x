#include "HelloWorldScene.h"
#include "cocos3d.h"
#include "EffectMgr.h"
#include "EffectConstant.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Effect.h"
#include "VertexDeclaration.h"

#include "TestEGL.h"

USING_NS_CC;

TestEGL::UserData g_userData;

class Test3DNode : public C3DNode
{
    my3d::VertexBuffer * m_vertexBuffer;
    my3d::EffectPtr     m_effect;
    my3d::VertexDeclaration *m_vertexDecl;
    BBVertex m_vertices[4];
    
    static const int s_numVertices = 4;
    
public:
    
    Test3DNode()
    : m_vertexBuffer(nullptr)
    , m_vertexDecl(nullptr)
    {
        
    }
    
    ~Test3DNode()
    {
        delete m_vertexDecl;
        delete m_vertexBuffer;
    }
    
    static Test3DNode * create()
    {
        Test3DNode * p = new Test3DNode();
        if(!p->initTest3D())
        {
            delete p;
            return nullptr;
        }
        
        p->autorelease();
        return p;
    }
    
    bool initTest3D()
    {
        const float size = 2.8f;
        const float pz = 0.0f;
        
        m_vertices[0].position.set(-size, -size, pz);
        m_vertices[0].color.set(0.0f, 0.0f, 1.0f, 1.0f);
        
        m_vertices[1].position.set(-size, size, pz);
        m_vertices[1].color.set(0.0f, 1.0f, 0.0f, 1.0f);
        
        m_vertices[2].position.set(size, -size, pz);
        m_vertices[2].color.set(1.0f, 0.0f, 0.0f, 1.0f);
        
        m_vertices[3].position.set(size, size, pz);
        m_vertices[3].color.set(0.0f, 1.0f, 1.0f, 1.0f);
        

        
        m_vertexBuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
                                                s_numVertices * sizeof(BBVertex), m_vertices);
        
        m_effect = my3d::EffectMgr::instance()->get("effect/test.fx");
        
        m_vertexDecl = new my3d::VertexDeclaration();
        m_vertexDecl->addElement(my3d::VertexUsage::POSITION, 3);
        m_vertexDecl->addElement(my3d::VertexUsage::COLOR, 4);
        
        TestEGL::Init(&g_userData);
        
        return true;
    }
    
    virtual void draw() override
    {
        C3DNode::draw();

#if 1
        C3DLayer *pMainLayer = C3DLayer::getMainLayer();
        C3DScene *pScene = pMainLayer->get3DScene();
        C3DCamera *pCamera = pScene->getActiveCamera();
        
        Matrix matViewProj = pCamera->getViewProjectionMatrix();
        Vector3 pos;
        for(int i = 0; i<s_numVertices; ++i)
        {
            pos = m_vertices[i].position;
            matViewProj.transformPoint(&pos);
        }
        
        if(m_effect && m_effect->begin())
        {
            my3d::EffectConstant *pConst = m_effect->getConstant("u_matViewProj");
            if(pConst)
            {
                pConst->bindValue(matViewProj);
            }
            
            if(m_vertexBuffer != nullptr)
            {
                m_vertexBuffer->bind();
                m_vertexDecl->bind(m_effect.get());
                
                
                
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                m_vertexBuffer->unbind();
            }
            
            m_effect->end();
        }
#else
        TestEGL::Draw(&g_userData);
        
#endif
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
    pCamera->setPosition(0, 2, 0);
    C3DScene *pScene = pLayer->get3DScene();
    pScene->addNodeToRenderList(pCamera);
    pScene->setActiveCamera(0);
    
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
