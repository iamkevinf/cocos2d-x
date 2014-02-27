//
//  TestTextureDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestTextureDraw__
#define __test3d__TestTextureDraw__

#include "TestBase.h"

//渲染一个带纹理的立方体
class TestTextureDrawNode : public TestBaseNode
{
    my3d::VertexBufferPtr   m_vertexBuffer;
    my3d::IndexBufferPtr    m_indexBuffer;
    my3d::VertexDeclarationPtr m_vertexDecl;
    my3d::EffectPtr         m_effect;
    my3d::TexturePtr        m_texture;
    int                     m_nIndices;
public:
    
    TestTextureDrawNode();
    ~TestTextureDrawNode();
    
    CREATE_TEST_3D_NODE(TestTextureDrawNode)
    
    virtual bool initTest3D() override;
    virtual void draw() override;
    
};

#endif /* defined(__test3d__TestTextureDraw__) */
