#ifndef H_M3D_RENDER_STATE
#define H_M3D_RENDER_STATE

#include "M3DBase.h"

namespace my3d
{

    enum class RenderState
    {
        ZEnable,
        CullFace,
        Blend,
        DepthTest,
        LineSmooth,
        MutiSample,
        ScissorTest,
        StencilTest,
        PointSize,
    };

    enum class BlendFun
    {
        Zero,
        One,

        SrcColor,
        InvSrcColor,

        DstColor,
        InvDstColor,

        SrcAlpha,
        InvSrcAlpha,

        DstAlpha,
        InvDstAlpha,

        ConstantColor,
        InvConstantColor,

        ConstantAlpha,
        InvConstantAlpha,
    };

    enum class CullFace
    {
        Front,
        Back,
        FrontBack
    };

    enum class CompareFun
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        GreaterEqual,
        NotEqual,
        Always,
    };

    enum class StencilOp
    {
        Keep,
        Zero,
        Relpace,
        Incr,
        IncrWrap,
        Decr,
        DecrWrap,
        Invert,
    };


    namespace ColorMask
    {
        const uint32 A = 0xff << 24;
        const uint32 R = 0xff << 16;
        const uint32 G = 0xff << 8;
        const uint32 B = 0xff << 0;
    }

    GLenum renderState2Sys(RenderState state);
    GLenum blendFun2Sys(BlendFun state);
    GLenum cullFace2Sys(CullFace mode);
    GLenum compareFun2Sys(CompareFun fun);
    GLenum stencilOp2Sys(StencilOp op);


}//end namespace my3d

#endif //H_M3D_RENDER_STATE