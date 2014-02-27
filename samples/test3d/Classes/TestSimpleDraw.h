//
//  TestSimpleDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestSimpleDraw__
#define __test3d__TestSimpleDraw__

#include "TestBase.h"

//渲染一个立方体
class TestSimpleDrawNode : public TestBaseNode
{
    my3d::VertexBufferPtr   m_vertexBuffer;
    my3d::IndexBufferPtr    m_indexBuffer;
    my3d::VertexDeclarationPtr m_vertexDecl;
    my3d::EffectPtr         m_effect;
    int                     m_nIndices;

public:
    
    TestSimpleDrawNode();
    ~TestSimpleDrawNode();
    
    CREATE_TEST_3D_NODE(TestSimpleDrawNode);
    
    virtual bool initTest3D() override;
    virtual void draw() override;
    
};

#endif /* defined(__test3d__TestSimpleDraw__) */
