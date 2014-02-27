#include "HelloWorldScene.h"
#include "cocos3d.h"
#include "my3d/my3d.h"

#include "TestEGL.h"

USING_NS_CC;

namespace
{
    TestEGL::UserData g_userData;
}

//#define TEST_EGL

class Test3DNode : public C3DNode
{
    my3d::VertexBufferPtr  m_vertexBuffer;
    my3d::IndexBufferPtr   m_indexBuffer;
    my3d::VertexDeclarationPtr m_vertexDecl;
    my3d::EffectPtr     m_effect;
    my3d::TexturePtr    m_texture;
    
    my3d::MeshPtr m_mesh;
    my3d::MeshPtr m_ground;
    
public:
    
    Test3DNode()
        : m_vertexBuffer(nullptr)
        , m_vertexDecl(nullptr)
        , m_indexBuffer(nullptr)
    {
        
    }
    
    ~Test3DNode()
    {
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
        my3d::VertexXYZColor vertices[numVertices];
        
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
            numVertices * sizeof(my3d::VertexXYZColor), &vertices[0]);
        
        m_effect = my3d::EffectMgr::instance()->get("effect/test1.shader");
        m_vertexDecl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZColor::getType());
        
        return true;
    }
    
    bool initVertexBuffer_2()
    {
        const float size = 2.8f;
        
        const int numVertices = 8;
        my3d::VertexXYZUV vertices[numVertices];
        
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
             numVertices * sizeof(my3d::VertexXYZUV), &vertices[0]);
        
        m_vertexDecl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZUV::getType());
        m_effect = my3d::EffectMgr::instance()->get("effect/test2.shader");
        m_texture = my3d::TextureMgr::instance()->get("HelloWorld.png");
        
        return true;
    }
    
    bool initTest3D()
    {
#if 0
        initVertexBuffer_1();
#else
        initVertexBuffer_2();
#endif
        
        //create index buffer
        
        const int numIndices = 6 * 2 * 3;
        my3d::uint16 indices[numIndices] = {
            0, 2, 1,  0, 3, 2, //front
            3, 6, 2,  3, 7, 6, //right
            7, 5, 6,  7, 4, 5, //back
            4, 1, 5,  4, 0, 1, //left
            1, 6, 5,  1, 2, 6, //top
            4, 3, 0,  4, 7, 3, //bottom
        };
        
        m_indexBuffer = new my3d::IndexBuffer(my3d::BufferUsage::Static,
            my3d::IndexType::Index16, numIndices * sizeof(my3d::uint16), &indices[0]);

        //create material
        
        my3d::MaterialPtr mtl = new my3d::Material();
        mtl->setEffect(m_effect);
        
        my3d::TextureStage stage;
        stage.setTexture(m_texture);
        stage.setUWrap(my3d::TextureWrap::Repeat);
        stage.setVWrap(my3d::TextureWrap::Repeat);
        stage.setMinFilter(my3d::TextureFilter::Linear);
        stage.setMagFilter(my3d::TextureFilter::Linear);
        mtl->addTextureStage(stage);
        
        //create a sub mesh
        
        my3d::SubMeshPtr subMesh = new my3d::SubMesh();
        subMesh->setPrimitive(my3d::PrimitiveType::TriangleList, 0, numIndices);
        subMesh->setMaterial(mtl);
        
        //create mesh
        
        m_mesh = new my3d::Mesh();
        m_mesh->setVertexBuffer(m_vertexBuffer);
        m_mesh->setVertexDecl(m_vertexDecl);
        m_mesh->setIndexBuffer(m_indexBuffer);
        m_mesh->addSubMeshes(subMesh);
        
#ifdef TEST_EGL
        TestEGL::Init(&g_userData);
#endif
        
        initGround();
        
        return true;
    }
    
    void initGround()
    {
        const int nRows = 10;
        const int nCols = 8;
        
        const int nRowVertices = nRows + 1;
        const int nColVertices = nCols + 1;
        
        const int nVertices = nRowVertices * nColVertices;
        
        const int nFaces = nRows * nCols * 2;
        const int nIndices = nFaces * 3;
        
        const float gridWidth = 4.0f;
        const float gridHeight = 3.0f;
        const float halfWidth = nCols * gridWidth * 0.5f;
        const float halfHeight = nRows * gridHeight * 0.5f;
        const float y = -4.0f;
        
        //create vertex buffer
        
        my3d::VertexXYZUV vertices[nVertices];
        for (int r = 0; r < nRowVertices; ++r)
        {
            for(int c = 0; c < nColVertices; ++c)
            {
                int i = r * nColVertices + c;
                vertices[i].position.set(c * gridWidth - halfWidth, y, r * gridHeight - halfHeight);
                vertices[i].uv.set(c, r);
            }
        }
        
        my3d::VertexBufferPtr vb = new my3d::VertexBuffer(
              my3d::BufferUsage::Static, nVertices * sizeof(my3d::VertexXYZUV), vertices );
        
        //create index buffer
        
        my3d::uint16 indices[nIndices];
        my3d::uint16 *p = indices;
        for(int r = 0; r < nRows; ++r)
        {
            for(int c = 0; c < nCols; ++c)
            {
                int i = r * nColVertices + c;
                p[0] = i;
                p[1] = i + nColVertices;
                p[2] = i + 1;
                
                p[3] = p[2];
                p[4] = p[1];
                p[5] = p[4] + 1;
                
                p += 6;
            }
        }
        
        my3d::IndexBufferPtr ib = new my3d::IndexBuffer(my3d::BufferUsage::Static,
             my3d::IndexType::Index16, sizeof(my3d::uint16) * nIndices, indices);
        
        //create material
        
        my3d::MaterialPtr mtl = new my3d::Material();
        mtl->setEffect(my3d::EffectMgr::instance()->get("effect/test2.shader"));
        
        my3d::TextureStage stage;
        stage.setTexture(my3d::TextureMgr::instance()->get("HelloWorld.png"));
        stage.setUWrap(my3d::TextureWrap::Repeat);
        stage.setVWrap(my3d::TextureWrap::Mirror);
        stage.setMinFilter(my3d::TextureFilter::Linear);
        stage.setMagFilter(my3d::TextureFilter::Linear);
        mtl->addTextureStage(stage);
        
        //create a sub mesh
        
        my3d::SubMeshPtr sub = new my3d::SubMesh();
        sub->setMaterial(mtl);
        sub->setPrimitive(my3d::PrimitiveType::TriangleList, 0, nIndices);
        
        //create vertex declaration
        
        my3d::VertexDeclarationPtr decl = my3d::VertexDeclMgr::instance()->get(my3d::VertexXYZUV::getType());
        
        //create the ground mesh
        
        m_ground = new my3d::Mesh();
        m_ground->setVertexBuffer(vb);
        m_ground->setVertexDecl(decl);
        m_ground->setIndexBuffer(ib);
        m_ground->addSubMeshes(sub);
        
    }
    
    virtual void draw() override
    {
        if(m_ground)
        {
            m_ground->draw();
        }
        
        this->rotateY(0.01f);

        my3d::renderDev()->pushWorld(this->getWorldMatrix());

        C3DNode::draw();

#if 1
        my3d::renderDev()->setRenderState(my3d::RenderState::CullFace, true);
        my3d::renderDev()->setCullFace(my3d::CullFace::Back);
        
#if 0
        if(m_vertexBuffer)
        {
            m_vertexBuffer->bind();
            m_indexBuffer->bind();
            m_vertexDecl->bind();
            
            if(m_effect && m_effect->begin())
            {
                my3d::EffectConstant *pConst = m_effect->getConstant(my3d::EffectConstType::Texture);
                if(m_texture && pConst) pConst->bindValue(m_texture.get());
                
                my3d::renderDev()->drawIndexedPrimitive(my3d::PrimitiveType::TriangleList, 0, numIndices);
                
                m_effect->end();
            }
            
            m_indexBuffer->unbind();
            m_vertexBuffer->unbind();
        }
#else
        if(m_mesh)
        {
          m_mesh->draw();
        }
        
#endif
        
        
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
