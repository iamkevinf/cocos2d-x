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

//#define TEST_EGL

class Test3DNode : public C3DNode
{
    my3d::VertexBuffer  *m_vertexBuffer;
    my3d::IndexBuffer   *m_indexBuffer;
    my3d::VertexDeclaration *m_vertexDecl;
    my3d::EffectPtr     m_effect;
    SamplerPtr  m_sampler;
    
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
    
    bool initVertexBuffer_1()
    {
        const float size = 2.8f;
        
        const int numVertices = 8;
        BBVertex vertices[numVertices];
        
        vertices[0].position.set(-size, -size, size);
        vertices[0].color.set(0.0f, 0.0f, 1.0f, 1.0f);
        
        vertices[1].position.set(-size, size, size);
        vertices[1].color.set(0.0f, 1.0f, 0.0f, 1.0f);
        
        vertices[2].position.set(size, size, size);
        vertices[2].color.set(0.0f, 1.0f, 1.0f, 1.0f);
        
        vertices[3].position.set(size, -size, size);
        vertices[3].color.set(1.0f, 0.0f, 0.0f, 1.0f);
        
        for (int i = 0; i < 4; ++i)
        {
            vertices[i + 4] = vertices[i];
            vertices[i + 4].position.z = -size;
        }
        
        m_vertexBuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
                                                numVertices * sizeof(BBVertex), &vertices[0]);
        
        m_effect = my3d::EffectMgr::instance()->get("effect/test1.shader");
        
        m_vertexDecl = new my3d::VertexDeclaration();
        m_vertexDecl->addElement(my3d::VertexUsage::POSITION, 3);
        m_vertexDecl->addElement(my3d::VertexUsage::COLOR, 4);
        
        return true;
    }
    
    bool initVertexBuffer_2()
    {
        const float size = 2.8f;
        
        const int numVertices = 8;
        VertexPositionUV vertices[numVertices];
        
        vertices[0].position.set(-size, -size, size);
        vertices[0].uv.set(0.0f, 1.0f);
        
        vertices[1].position.set(-size, size, size);
        vertices[1].uv.set(0.0f, 0.0f);
        
        vertices[2].position.set(size, size, size);
        vertices[2].uv.set(1.0f, 0.0f);
        
        vertices[3].position.set(size, -size, size);
        vertices[3].uv.set(1.0f, 1.0f);
        
        for (int i = 0; i < 4; ++i)
        {
            vertices[i + 4] = vertices[i];
            vertices[i + 4].position.z = -size;
        }
        
        vertices[4].uv.set(1, 1);
        vertices[5].uv.set(1, 0);
        vertices[6].uv.set(0, 0);
        vertices[7].uv.set(0, 1);
        
        m_vertexBuffer = new my3d::VertexBuffer(my3d::BufferUsage::Static,
                                                numVertices * sizeof(BBVertex), &vertices[0]);
        
        m_vertexDecl = new my3d::VertexDeclaration();
        m_vertexDecl->addElement(my3d::VertexUsage::POSITION, 3);
        m_vertexDecl->addElement(my3d::VertexUsage::TEXCOORD0, 2);

        m_effect = my3d::EffectMgr::instance()->get("effect/test2.shader");
        m_sampler = C3DSampler::create("HelloWorld.png");
        
        return true;
    }
    
    bool initTest3D()
    {
#if 0
        initVertexBuffer_1();
#else
        initVertexBuffer_2();
#endif
        
        unsigned short indices[numIndices] = {
            0, 2, 1,  0, 3, 2, //front
            3, 6, 2,  3, 7, 6, //right
            7, 5, 6,  7, 4, 5, //back
            4, 1, 5,  4, 0, 1, //left
            1, 6, 5,  1, 2, 6, //top
            4, 3, 0,  4, 7, 3, //bottom
        };
        
        m_indexBuffer = new my3d::IndexBuffer(my3d::BufferUsage::Static,
            numIndices * sizeof(unsigned short), &indices[0]);

#ifdef TEST_EGL
        TestEGL::Init(&g_userData);
#endif
        
        return true;
    }
    
    virtual void draw() override
    {
        this->rotateY(0.01f);

        my3d::renderDev()->pushWorld(this->getWorldMatrix());

        C3DNode::draw();

#if 1
        my3d::renderDev()->setRenderState(my3d::RenderState::CullFace, true);
        my3d::renderDev()->setCullFace(my3d::CullFace::Back);
        
        if(m_effect && m_effect->begin())
        {
            my3d::EffectConstant *pConst = m_effect->getConstant(my3d::EffectConstType::Sampler);
            if(m_sampler && pConst)
            {
                pConst->bindValue(m_sampler.get());
            }
            
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
#endif
        
#ifdef TEST_EGL
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
