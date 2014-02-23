#ifndef H_M3D_RENDER_DEVICE_H
#define H_M3D_RENDER_DEVICE_H

#include "M3DBase.h"

namespace my3d
{
    enum class RenderState
    {
        ZEnable,
        CullFace,
    };

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

    private:
        std::vector<cocos2d::Matrix> m_matWorlds;
        cocos2d::Matrix m_matView;
        cocos2d::Matrix m_matProj;
        mutable cocos2d::Matrix m_matInvView;
        mutable cocos2d::Matrix m_matViewProj;
        mutable cocos2d::Matrix m_matWorldViewProj;
        mutable uint32  m_dirty;
    };

}//end namespace m3d


#endif //H_M3D_RENDER_DEVICE_H