#ifndef H_M3D_MESH_H
#define H_M3D_MESH_H

#include "M3DVertexBuffer.h"
#include "M3DVertexDeclaration.h"
#include "M3DTexture.h"
#include "M3DEffect.h"
#include "M3DRenderState.h"

namespace my3d
{
    class SubMesh : public cocos2d::ISmartObject
    {
    public:
        SubMesh();
        ~SubMesh();
        
        void draw();
        
        void setTexture(TexturePtr tex);
        void setEffect(EffectPtr eff);
        void setPrimitive(PrimitiveType pt, uint32 start, uint32 count);
        
    private:
        uint32          m_indexStart;
        uint32          m_indexCount;
        PrimitiveType   m_primitiveType;
        TexturePtr      m_texture;
        EffectPtr       m_effect;
    };
    typedef SmartPtr<SubMesh> SubMeshPtr;
    
    
    class Mesh : public cocos2d::ISmartObject
    {
    public:
        Mesh();
        ~Mesh();
        
        void draw();
        
        void setVertexBuffer(VertexBufferPtr vertex);
        void setIndexBuffer(IndexBufferPtr index);
        void setVertexDecl(VertexDeclarationPtr decl);
        void setSubMeshes(const std::vector<SubMeshPtr> & subMeshes);
        void addSubMeshes(SubMeshPtr subMesh);
        
    private:
        VertexDeclarationPtr m_vertexDecl;
        VertexBufferPtr      m_vertexBuffer;
        IndexBufferPtr       m_indexBuffer;
        std::vector<SubMeshPtr>     m_subMeshs;
    };
    
    typedef SmartPtr<Mesh> MeshPtr;
    
}//end namespace my3d

#endif //H_M3D_MESH_H