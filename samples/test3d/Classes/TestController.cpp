//
//  TestController.cpp
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#include "TestController.h"

#include "TestEGLDraw.h"
#include "TestSimpleDraw.h"
#include "TestTextureDraw.h"
#include "TestMeshDraw.h"
#include "TestGroundDraw.h"
#include "TestLightDraw.h"
#include "TestMeshFromFile.h"


TestController::TestController()
: m_curTestNode(nullptr)
, m_curTestIndex(0)
{
    
}

TestController::~TestController()
{
}


bool TestController::initTest3D()
{
    m_testFactory.clear();
    
    addTestFactoryMethod("opengl es", CreateTestFun(TestEGLDrawNode::create));
    addTestFactoryMethod("simple cube", CreateTestFun(TestSimpleDrawNode::create));
    addTestFactoryMethod("texture cube", CreateTestFun(TestTextureDrawNode::create));
    addTestFactoryMethod("mesh", CreateTestFun(TestMeshDrawNode::create));
    addTestFactoryMethod("ground", CreateTestFun(TestGroundDrawNode::create));
    addTestFactoryMethod("lighting", CreateTestFun(TestLightDrawNode::create));
    addTestFactoryMethod("mesh auto generate", CreateTestFun(TestMeshFileNode::create));
    
    m_curTestIndex = 6;
    
    restartTest();
    
    return true;
}

void TestController::draw()
{
    my3d::renderDev()->setRenderState(my3d::RenderState::CullFace, true);
    my3d::renderDev()->setCullFace(my3d::CullFace::Back);
    my3d::renderDev()->setRenderState(my3d::RenderState::DepthTest, true);
    
    my3d::renderDev()->pushWorld(this->getWorldMatrix());
    
    TestBaseNode::draw();
    
    my3d::renderDev()->popWorld();
    
    my3d::renderDev()->setRenderState(my3d::RenderState::DepthTest, false);
}

void TestController::addTestFactoryMethod(const std::string & desc, CreateTestFun method)
{
    m_testFactory.push_back(std::make_pair(desc, method));
}

TestBaseNode * TestController::createTestNode(int index)
{
    return (m_testFactory[index].second)();
}

void TestController::goNextTest()
{
    ++m_curTestIndex;
    m_curTestIndex %= m_testFactory.size();
    
    restartTest();
}

void TestController::goPrevTest()
{
    --m_curTestIndex;
    if(m_curTestIndex < 0)
        m_curTestIndex = m_testFactory.size() - 1;
    
    restartTest();
}

void TestController::restartTest()
{
    cocos2d::C3DScene *pScene = cocos2d::C3DLayer::getMainLayer()->get3DScene();
    pScene->getActiveCamera()->setPosition(0, 2.0, 8.0f);
    my3d::renderDev()->updateCamera();
    
    if(m_curTestNode)
        this->removeChild(m_curTestNode);
    
    m_curTestNode = createTestNode(m_curTestIndex);
    CCASSERT(m_curTestNode != nullptr, "create test failed!");
    
    this->addChild(m_curTestNode);
}

const size_t TestController::getNumTotalTest() const
{
    return m_testFactory.size();
}

const std::string & TestController::getTestDesc(int index) const
{
    return m_testFactory[index].first;
}

const std::string & TestController::getCurTestDesc() const
{
    return m_testFactory[m_curTestIndex].first;
}
