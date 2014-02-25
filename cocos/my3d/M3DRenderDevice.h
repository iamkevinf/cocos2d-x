#ifndef H_M3D_RENDER_DEVICE_H
#define H_M3D_RENDER_DEVICE_H

#include "M3DRenderState.h"
#include "M3DMaterial.h"

namespace my3d
{
    class VertexBuffer;
    class IndexBuffer;

    class RenderDevice
    {
    public:
        RenderDevice();
        ~RenderDevice();

        void pushWorld(const cocos2d::Matrix & world);
        void popWorld();

        const cocos2d::Matrix & getWorld() const;
        void setWorld(const cocos2d::Matrix & world);

        void setView(const cocos2d::Matrix & view);
        const cocos2d::Matrix & getView() const;

        void setProjection(const cocos2d::Matrix & proj);
        const cocos2d::Matrix & getProjection() const;

        const cocos2d::Matrix & getInvView() const;
        const cocos2d::Matrix & getViewProjection();
        const cocos2d::Matrix & getWorldViewProjection() const;

        void setRenderState(RenderState state, bool enable);
        void setBlendFun(BlendFun src, BlendFun dst);
        void setCullFace(CullFace mode);
        void setFrontFace(bool isCW);
        void setDepthFun(CompareFun fun);
        void setScissor(int32 x, int32 y, uint32 width, uint32 height);
        
        void setStencilMask(uint32 mask);
        void setStencilFun(CompareFun fun, int32 ref, uint32 mask);
        void setStencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass);
        void setStencilOpSeparate(CullFace face, StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass);
        
        void setColorMask(uint32 color);
        void setShadeMode();
        
        void setTextureActive(GLenum tex);
        void setTextureFilter(TextureTarget target, TextureParam param, TextureFilter filter);

        void drawPrimitive(PrimitiveType pt, uint32 start, uint32 count);
        void drawIndexedPrimitive(PrimitiveType pt, uint32 start, uint32 count);
        
        void setVertexBuffer(VertexBuffer *p);
        void unsetVertexBuffer(VertexBuffer *p);
        
        void setIndexBuffer(IndexBuffer *p);
        void unsetIndexBuffer(IndexBuffer *p);
        
    private:
        std::vector<cocos2d::Matrix> m_matWorlds;
        cocos2d::Matrix m_matView;
        cocos2d::Matrix m_matProj;
        mutable cocos2d::Matrix m_matInvView;
        mutable cocos2d::Matrix m_matViewProj;
        mutable cocos2d::Matrix m_matWorldViewProj;
        mutable uint32  m_dirty;
        
        VertexBuffer    *m_vertexBuffer;
        IndexBuffer     *m_indexBuffer;
    };

}//end namespace m3d


#endif //H_M3D_RENDER_DEVICE_H