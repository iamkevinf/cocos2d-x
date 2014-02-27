//
//  TestController.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestController__
#define __test3d__TestController__

#include "TestBase.h"

typedef TestBaseNode* (*CreateTestFun)();

class TestController : public TestBaseNode
{
    
    std::vector<std::pair<std::string, CreateTestFun>> m_testFactory;
    int m_curTestIndex;
    TestBaseNode * m_curTestNode;
    
    
    void addTestFactoryMethod(const std::string & desc, CreateTestFun method);
    TestBaseNode * createTestNode(int index);
    
public:
    
    TestController();
    ~TestController();
    
    CREATE_TEST_3D_NODE(TestController);
    
    bool initTest3D();
    
    virtual void draw() override;
    
    void goNextTest();
    
    void goPrevTest();
    
    void restartTest();
    
    const size_t getNumTotalTest() const;
    
    const std::string & getTestDesc(int index) const;
    
    const std::string & getCurTestDesc() const;
    
};

#endif /* defined(__test3d__TestController__) */
