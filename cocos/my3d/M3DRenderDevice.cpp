﻿#include "M3DRenderDevice.h"
#include "M3DVertexBuffer.h"
#include "3d/C3DCamera.h"

namespace my3d
{
    namespace DirtyFlag
    {
        const uint32 InvView = 1 << 0;
        const uint32 ViewProj = 1 << 1;
        const uint32 WorldViewProj = 1 << 2;
    }

    RenderDevice::RenderDevice()
        : m_dirty(0)
        , m_pCamera(nullptr)
        , m_vertexBuffer(nullptr)
        , m_indexBuffer(nullptr)
    {
        m_matWorlds.push_back(cocos2d::Matrix::identity());
        m_matInvView = m_matView = cocos2d::Matrix::identity();
        m_matWorldViewProj = m_matViewProj = m_matProj = cocos2d::Matrix::identity();
    }


    RenderDevice::~RenderDevice()
    {
    }

    void RenderDevice::pushWorld(const cocos2d::Matrix & world)
    {
        m_matWorlds.push_back(world);
        m_dirty |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::popWorld()
    {
        m_matWorlds.pop_back();
        m_dirty |= DirtyFlag::WorldViewProj;
    }

    const cocos2d::Matrix & RenderDevice::getWorld() const
    {
        return m_matWorlds.back();
    }

    void RenderDevice::setWorld(const cocos2d::Matrix & world)
    {
        m_matWorlds.back() = world;
        m_dirty |= DirtyFlag::WorldViewProj;
    }

    void RenderDevice::setView(const cocos2d::Matrix & view)
    {
        m_matView = view;
        m_dirty |= DirtyFlag::InvView | DirtyFlag::ViewProj | DirtyFlag::WorldViewProj;
    }

    const cocos2d::Matrix & RenderDevice::getView() const
    {
        return m_matView;
    }

    void RenderDevice::setProjection(const cocos2d::Matrix & proj)
    {
        m_matProj = proj;
        m_dirty |= DirtyFlag::ViewProj | DirtyFlag::WorldViewProj;
    }

    const cocos2d::Matrix & RenderDevice::getProjection() const
    {
        return m_matProj;
    }

    const cocos2d::Matrix & RenderDevice::getInvView() const
    {
        if (m_dirty & DirtyFlag::InvView)
        {
            m_matView.invert(&m_matInvView);
            m_dirty &= ~DirtyFlag::InvView;
        }
        return m_matInvView;
    }

    const cocos2d::Matrix & RenderDevice::getViewProjection()
    {
        if (m_dirty & DirtyFlag::ViewProj)
        {
            m_matViewProj = m_matProj * m_matView;
            m_dirty &= ~DirtyFlag::ViewProj;
        }
        return m_matViewProj;
    }

    const cocos2d::Matrix & RenderDevice::getWorldViewProjection() const
    {
        if (m_dirty & DirtyFlag::WorldViewProj)
        {
            m_matWorldViewProj = m_matProj *  m_matView * m_matWorlds.back();
            m_dirty &= ~DirtyFlag::WorldViewProj;
        }
        return m_matWorldViewProj;
    }

    void RenderDevice::setRenderState(RenderState state, bool enable)
    {
        GLenum glstate = renderState2Sys(state);

        if (enable) glEnable(glstate);
        else glDisable(glstate);
    }

    void RenderDevice::setBlendFun(BlendFun src, BlendFun dst)
    {
        glBlendFunc(blendFun2Sys(src), blendFun2Sys(dst));
    }

    void RenderDevice::setCullFace(CullFace mode)
    {
        glCullFace(cullFace2Sys(mode));
    }

    void RenderDevice::setFrontFace(bool isCW)
    {
        glFrontFace(isCW ? GL_CW : GL_CCW);
    }

    void RenderDevice::setDepthFun(CompareFun fun)
    {
        glDepthFunc(compareFun2Sys(fun));
    }

    void RenderDevice::setScissor(int32 x, int32 y, uint32 width, uint32 height)
    {
        glScissor(x, y, width, height);
    }

    void RenderDevice::setStencilMask(uint32 mask)
    {
        glStencilMask(mask);
    }

    void RenderDevice::setStencilFun(CompareFun fun, int32 ref, uint32 mask)
    {
        glStencilFunc(compareFun2Sys(fun), ref, mask);
    }

    void RenderDevice::setStencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass)
    {
        glStencilOp(
            stencilOp2Sys(stencilFail),
            stencilOp2Sys(depthFail),
            stencilOp2Sys(depthPass)
            );
    }

    void RenderDevice::setStencilOpSeparate(CullFace face, StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass)
    {
        glStencilOpSeparate(
            cullFace2Sys(face),
            stencilOp2Sys(stencilFail),
            stencilOp2Sys(depthFail),
            stencilOp2Sys(depthPass)
            );
    }

    void RenderDevice::setColorMask(uint32 color)
    {
        
        GLboolean r = color & ColorMask::R ? GL_TRUE : GL_FALSE;
        GLboolean g = color & ColorMask::G ? GL_TRUE : GL_FALSE;
        GLboolean b = color & ColorMask::B ? GL_TRUE : GL_FALSE;
        GLboolean a = color & ColorMask::A ? GL_TRUE : GL_FALSE;
        glColorMask(r, g, b, a);
    }
    
    void RenderDevice::setTextureActive(GLenum tex)
    {
        glActiveTexture(tex);
    }
    
    void RenderDevice::setTextureFilter(TextureTarget target, TextureParam param, TextureFilter filter)
    {
        glTexParameteri(
                        textureTarget2Sys(target),
                        textureParam2Sys(param),
                        textureFilter2Sys(filter)
                        );
    }
    
    void RenderDevice::setTextureWrap(TextureTarget target, TextureParam param, TextureWrap wrap)
    {
        glTexParameteri(
                        textureTarget2Sys(target),
                        textureParam2Sys(param),
                        textureWrap2Sys(wrap)
                        );
    }

    void RenderDevice::setShadeMode()
    {
        //glShadeModel();
    }
    
    void RenderDevice::drawPrimitive(PrimitiveType pt, uint32 start, uint32 count)
    {
        glDrawArrays(primitiveType2Sys(pt), start, count);
    }
    
    void RenderDevice::drawIndexedPrimitive(PrimitiveType pt, uint32 start,  uint32 count)
    {
        CCAssert(m_indexBuffer, "Please bind the index buffer first!");
        CCAssert(m_indexBuffer->getIndexType() != IndexType::IndexError, "Please set the IndexType first!");
        
        IndexType itype = m_indexBuffer->getIndexType();
        size_t isize = indexType2Size(itype);
        GLenum type = indexType2Sys(itype);
        GL_ASSERT( glDrawElements(primitiveType2Sys(pt), count, type, (GLvoid*)(start*isize)) );
    }
    
    void RenderDevice::setVertexBuffer(VertexBuffer *p)
    {
        m_vertexBuffer = p;
    }
    
    void RenderDevice::unsetVertexBuffer(VertexBuffer *p)
    {
        if(m_vertexBuffer == p) m_vertexBuffer = nullptr;
    }
    
    void RenderDevice::setIndexBuffer(IndexBuffer *p)
    {
        m_indexBuffer = p;
    }
    
    void RenderDevice::unsetIndexBuffer(IndexBuffer *p)
    {
        if(m_indexBuffer == p) m_indexBuffer = nullptr;
    }
    
    void RenderDevice::setLightContainer(LightContainerPtr container)
    {
        m_lightContainer = container;
    }
    
    LightContainerPtr RenderDevice::getLightContainer()
    {
        return m_lightContainer;
    }
    
    void RenderDevice::setCamera(cocos2d::C3DCamera *pCamera)
    {
        m_pCamera = pCamera;
    }
    
    cocos2d::C3DCamera *RenderDevice::getCamera()
    {
        return m_pCamera;
    }
    
    void RenderDevice::updateCamera()
    {
        setView(m_pCamera->getViewMatrix());
        setProjection(m_pCamera->getProjectionMatrix());
    }

}//end namespace my3d