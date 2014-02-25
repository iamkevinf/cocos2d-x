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

    enum class TexType
    {
        Tex2D,
        Tex3D,
        TexCubeMap,
    };

#if 0
    enum class TexOp
    {
        GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL,
        GL_TEXTURE_COMPARE_FUNC,
        GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_LOD_BIAS,
        GL_TEXTURE_MIN_FILTER,
        GL_TEXTURE_MAG_FILTER,
        GL_TEXTURE_MIN_LOD,
        GL_TEXTURE_MAX_LOD,
        GL_TEXTURE_MAX_LEVEL,
        GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G,
        GL_TEXTURE_SWIZZLE_B,
        GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S,
        GL_TEXTURE_WRAP_T,
        GL_TEXTURE_WRAP_R,
        GL_TEXTURE_BORDER_COLOR,
        GL_TEXTURE_SWIZZLE_RGBA
    };
#endif

    namespace ColorMask
    {
        const uint32 A = 0xff << 24;
        const uint32 R = 0xff << 16;
        const uint32 G = 0xff << 8;
        const uint32 B = 0xff << 0;
    }
    
    enum class TextureTarget
    {
        Tex2D,
        TexCubeMap,
    };
    
    enum class TextureParam
    {
        MinFilter,
        MagFilter,
        WrapU,
        WrapV,
    };
    
    enum class TextureFilter
    {
        Near,
        Linear,
        NearMipmapNear,
        NearMipmapLinear,
        LinearMipmapNear,
        LinearMipmapLinear,
    };
    
    enum class PrimitiveType
    {
        PointList,
        LineList,
        LineStrip,
        LineLoop,
        TriangleList,
        TriangleStrip,
        TriangleFan,
    };

    GLenum renderState2Sys(RenderState state);
    GLenum blendFun2Sys(BlendFun state);
    GLenum cullFace2Sys(CullFace mode);
    GLenum compareFun2Sys(CompareFun fun);
    GLenum stencilOp2Sys(StencilOp op);
    GLenum textureTarget2Sys(TextureTarget tex);
    GLenum textureParam2Sys(TextureParam tex);
    GLint textureFilter2Sys(TextureFilter tex);
    GLenum primitiveType2Sys(PrimitiveType type);


}//end namespace my3d

#endif //H_M3D_RENDER_STATE