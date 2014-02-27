//
//  TestSimpleDraw.h
//  test3d
//
//  Created by zhoubao on 14-2-27.
//
//

#ifndef __test3d__TestSimpleDraw__
#define __test3d__TestSimpleDraw__

#include "cocos3d.h"
#include "my3d/my3d.h"

//渲染一个立方体
class SimpleDrawDNode : public cocos2d::C3DNode
{
    my3d::VertexBufferPtr   m_vertexBuffer;
    my3d::IndexBufferPtr    m_indexBuffer;
    my3d::VertexDeclarationPtr m_vertexDecl;
    my3d::EffectPtr         m_effect;
    int                     m_nIndices;

public:
    
    SimpleDrawDNode();
    ~SimpleDrawDNode();
    
    static SimpleDrawDNode * create();
    
    bool initTest3D();
    
    virtual void draw() override;
    
};

#endif /* defined(__test3d__TestSimpleDraw__) */
