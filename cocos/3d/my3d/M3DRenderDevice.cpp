#include "M3DRenderDevice.h"


namespace my3d
{
    namespace DirtyFlag
    {
        const uint32 InvView = 1 << 0;
        const uint32 ViewProj = 1 << 1;
        const uint32 WorldViewProj = 1 << 2;
    }

    namespace
    {
        std::map<RenderState, GLenum> stateMap;
    }

    GLenum renderState2GL(RenderState state)
    {
        if (stateMap.empty())
        {
#define REG_STATE_MAP(RS_STATE, GL_STATE) stateMap.insert(std::pair<RenderState, GLenum>(RS_STATE, GL_STATE))

            REG_STATE_MAP(RenderState::CullFace, GL_CULL_FACE);

#undef REG_STATE_MAP
        }

        auto it = stateMap.find(state);
        if (it != stateMap.end()) return it->second;

        return 0;
    }


    RenderDevice::RenderDevice()
        : m_dirty(0)
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
        GLenum glstate = renderState2GL(state);

        if (enable) glEnable(glstate);
        else glDisable(glstate);
    }

}//end namespace my3d