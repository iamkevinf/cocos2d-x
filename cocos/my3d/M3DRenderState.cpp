#include "M3DRenderState.h"

namespace my3d
{

    namespace
    {
        std::map<RenderState, GLenum> stateMap;
        std::map<BlendFun, GLenum> blendMap;
    }

    GLenum renderState2Sys(RenderState state)
    {
        if (stateMap.empty())
        {
#define REG_STATE_MAP(RS_STATE, GL_STATE) stateMap.insert(std::make_pair(RS_STATE, GL_STATE))

            REG_STATE_MAP(RenderState::CullFace, GL_CULL_FACE);
            REG_STATE_MAP(RenderState::Blend, GL_BLEND);
            REG_STATE_MAP(RenderState::DepthTest, GL_DEPTH_TEST);
            REG_STATE_MAP(RenderState::LineSmooth, GL_LINE_SMOOTH);
            REG_STATE_MAP(RenderState::MutiSample, GL_MULTISAMPLE);
            REG_STATE_MAP(RenderState::ScissorTest, GL_SCISSOR_TEST);
            REG_STATE_MAP(RenderState::StencilTest, GL_STENCIL_TEST);
            REG_STATE_MAP(RenderState::PointSize, GL_PROGRAM_POINT_SIZE_EXT);

#undef REG_STATE_MAP
        }

        auto it = stateMap.find(state);
        if (it != stateMap.end()) return it->second;

        return 0;
    }

    GLenum blendFun2Sys(BlendFun state)
    {
        if (blendMap.empty())
        {
#define REG_BLEND_MAP(STATE, GL_STATE) blendMap.insert(std::make_pair(STATE, GL_STATE))

            REG_BLEND_MAP(BlendFun::One, GL_ONE);
            REG_BLEND_MAP(BlendFun::Zero, GL_ZERO);

            REG_BLEND_MAP(BlendFun::SrcColor, GL_SRC_COLOR);
            REG_BLEND_MAP(BlendFun::InvSrcColor, GL_ONE_MINUS_SRC_COLOR);

            REG_BLEND_MAP(BlendFun::SrcAlpha, GL_SRC_ALPHA);
            REG_BLEND_MAP(BlendFun::InvSrcAlpha, GL_ONE_MINUS_SRC_ALPHA);

            REG_BLEND_MAP(BlendFun::DstColor, GL_DST_COLOR);
            REG_BLEND_MAP(BlendFun::InvDstColor, GL_ONE_MINUS_DST_COLOR);

            REG_BLEND_MAP(BlendFun::DstAlpha, GL_DST_ALPHA);
            REG_BLEND_MAP(BlendFun::InvDstAlpha, GL_ONE_MINUS_DST_ALPHA);

            REG_BLEND_MAP(BlendFun::ConstantColor, GL_CONSTANT_COLOR);
            REG_BLEND_MAP(BlendFun::InvConstantColor, GL_ONE_MINUS_CONSTANT_COLOR);

            REG_BLEND_MAP(BlendFun::ConstantAlpha, GL_CONSTANT_ALPHA);
            REG_BLEND_MAP(BlendFun::InvConstantAlpha, GL_ONE_MINUS_CONSTANT_ALPHA);

#undef REG_BLEND_MAP
        }

        auto it = blendMap.find(state);
        if (it != blendMap.end()) return it->second;

        return 0;
    }

    GLenum cullFace2Sys(CullFace mode)
    {
        switch (mode)
        {
            case CullFace::Front:
                return GL_FRONT;

            case CullFace::Back:
                return GL_BACK;
                
            case CullFace::FrontBack:
                return GL_FRONT_AND_BACK;

            default:
                CCAssert(0, "Invalid CullFace!");
                return 0;
        }
    }

    GLenum compareFun2Sys(CompareFun fun)
    {
        switch (fun)
        {
            case CompareFun::Never:
                return GL_NEVER;

            case CompareFun::Less:
                return GL_LESS;

            case CompareFun::LessEqual:
                return GL_LEQUAL;

            case CompareFun::Equal:
                return GL_EQUAL;

            case CompareFun::Greater:
                return GL_GREATER;

            case CompareFun::GreaterEqual:
                return GL_GEQUAL;

            case CompareFun::Always:
                return GL_ALWAYS;

            default:
                CCAssert(0, "Invalid CompareFun!");
                return 0;
        }
    }

    GLenum stencilOp2Sys(StencilOp op)
    {
        switch (op)
        {
            case StencilOp::Keep:
                return GL_KEEP;

            case StencilOp::Zero:
                return GL_ZERO;

            case StencilOp::Incr:
                return GL_INCR;

            case StencilOp::IncrWrap:
                return GL_INCR_WRAP;

            case StencilOp::Decr:
                return GL_DECR;

            case StencilOp::DecrWrap:
                return GL_DECR_WRAP;

            case StencilOp::Relpace:
                return GL_REPLACE;

            case StencilOp::Invert:
                return GL_INVERT;

            default:
                CCAssert(0, "Invalid StencilOp");
                return 0;
        }
    }
    
    GLenum textureTarget2Sys(TextureTarget tex)
    {
        switch(tex)
        {
            case TextureTarget::Tex2D:
                return GL_TEXTURE_2D;
            
            case TextureTarget::TexCubeMap:
                return GL_TEXTURE_CUBE_MAP;
            
            default:
                CCAssert(0, "Invalid TextureTarget");
                return 0;
        };
    }
    
    GLenum textureParam2Sys(TextureParam tex)
    {
        switch(tex)
        {
            case TextureParam::MinFilter:
                return GL_TEXTURE_MIN_FILTER;
            
            case TextureParam::MagFilter:
                return GL_TEXTURE_MAG_FILTER;
            
            case TextureParam::WrapU:
                return GL_TEXTURE_WRAP_S;
            
            case TextureParam::WrapV:
                return GL_TEXTURE_WRAP_R;
            
            default:
                CCAssert(0, "Invalid TextureParam");
                return 0;
        };
    }
    
    GLenum textureFilter2Sys(TextureFilter tex)
    {
        switch (tex)
        {
            case TextureFilter::Near:
                return GL_NEAREST;
            
            case TextureFilter::Linear:
                return GL_LINEAR;
            
            case TextureFilter::NearMipmapNear:
                return GL_NEAREST_MIPMAP_LINEAR;
            
            case TextureFilter::NearMipmapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            
            case TextureFilter::LinearMipmapNear:
                return GL_LINEAR_MIPMAP_NEAREST;
            
            case TextureFilter::LinearMipmapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
            
            default:
                CCAssert(0, "Invalid TextureFilter");
                return 0;
        }
    }
    
    GLenum primitiveType2Sys(PrimitiveType type)
    {
        switch(type)
        {
            case PrimitiveType::PointList:
                return GL_POINTS;
                
            case PrimitiveType::LineList:
                return GL_LINES;
                
            case PrimitiveType::LineStrip:
                return GL_LINE_STRIP;
                
            case PrimitiveType::LineLoop:
                return GL_LINE_LOOP;
                
            case PrimitiveType::TriangleList:
                return GL_TRIANGLES;
                
            case PrimitiveType::TriangleStrip:
                return GL_TRIANGLE_STRIP;
                
            case PrimitiveType::TriangleFan:
                return GL_TRIANGLE_FAN;
                
            default:
                CCAssert(0, "Invalid PrimitiveType");
                return 0;
        };
    }
    
    GLenum textureWrap2Sys(TextureWrap tex)
    {
        switch(tex)
        {
            case TextureWrap::Repeat:
                return GL_REPEAT;
                
            case TextureWrap::Mirror:
                return GL_MIRRORED_REPEAT;
                
            case TextureWrap::Clamp:
                return GL_CLAMP_TO_EDGE;
                
            default:
                CCAssert(0, "Invalid TextureWrap");
                return 0;
        }
    }
}