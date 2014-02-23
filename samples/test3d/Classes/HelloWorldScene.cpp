#include "HelloWorldScene.h"
#include "cocos3d.h"
#include "Vertex.h"

#include "my3d/my3d.h"

#include "TestEGL.h"

USING_NS_CC;

namespace
{
    TestEGL::UserData g_userData;
    const int numIndices = 6 * 2 * 3;
}

class Test3DNode : public C3DNode
{
    my3d::VertexBuffer  *m_vertexBuffer;
    my3d::IndexBuffer   *m_indexBuffer;
    my3d::EffectPtr     m_effect;
    my3d::VertexDeclaration *m_vertexDecl;
    std::vector<BBVertex> m_vertices;
    
public:
    
    Test3DNode()
        : m_vertexBuffer(nullptr)
        , m_vertexDecl(nullptr)
        , m_indexBuffer(nullptr)
    {
        
    }
    
    ~Test3DNode()
    {
        delete m_vertexDecl;
        delete m_vertexBuffer;
        delete m_indexBuffer;
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
        const float pz = -0.0f;

        m_vertices.resize(8);
        
        m_vertices[0].position.set(-size, -size, size);
        m_vertices[0].color.set(0.0f, 0.0f, 1.0f, 1.0f);
        
        m_vertices[1].position.set(-size, size, size);
        m_vertices[1].color.set(0.0f, 1.0f, 0.0f, 1.0f);
        
        m_vertices[2].position.set(size, size, size);
        m_vertices[2].color.set(0.0f, 1.0f, 1.0f, 1.0f);
        
        m_vertices[3].position.set(size, -size, size);
        m_vertices[3].color.set(1.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < 4; ++i)
        {
            m_vertices[i + 4] = m_vertices[i];
            m_vertices[i + 4].position.z = -size;
        }

        
        unsigned short indices[numIndices] = {
            0, 1, 2,  0, 2, 3, //front
            3, 2, 6,  3, 6, 7, //right
            7, 6, 5,  7, 5, 4, //back
            4, 5, 1,  4, 1, 0, //left
            1, 5, 6,  1, 6, 2, //top
            4, 0, 3,  4, 3, 7, //bottom
        };

        m_vertexBuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
            8 * sizeof(BBVertex), &m_vertices[0]);
        
        m_indexBuffer = new my3d::IndexBuffer(my3d::BufferUsage::Static,
            numIndices * sizeof(unsigned short), &indices[0]);

        m_effect = my3d::EffectMgr::instance()->get("effect/test.fx");
        
        m_vertexDecl = new my3d::VertexDeclaration();
        m_vertexDecl->addElement(my3d::VertexUsage::POSITION, 3);
        m_vertexDecl->addElement(my3d::VertexUsage::COLOR, 4);
        
        TestEGL::Init(&g_userData);
        
        return true;
    }
    
    virtual void draw() override
    {
        this->rotateY(0.01f);

        my3d::renderDev()->pushWorld(this->getWorldMatrix());

        C3DNode::draw();

#if 1
        glEnable(GL_CULL_FACE);
        
        if(m_effect && m_effect->begin())
        {
            if(m_vertexBuffer != nullptr)
            {
                m_vertexBuffer->bind();
                m_indexBuffer->bind();
                m_vertexDecl->bind();

                glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

                m_indexBuffer->unbind();
                m_vertexBuffer->unbind();
            }
            
            m_effect->end();
        }
#else
        TestEGL::Draw(&g_userData);
        
#endif

        my3d::renderDev()->popWorld();
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
    pCamera->setPosition(0, 0, 12.0f);
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
