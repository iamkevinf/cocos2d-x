#ifndef H_M3D_MESH_H
#define H_M3D_MESH_H

#include "mytool/smartptr.h"
#include "M3DVertexBuffer.h"
#include "M3DVertexDeclaration.h"
#include "M3DTexture.h"
#include "M3DEffect.h"

namespace my3d
{
    class SubMesh : public cocos2d::ISmartObject
    {
    public:
        SubMesh();
        ~SubMesh();
        
        void draw();
        
    private:
        TexturePtr  m_texture;
        EffectPtr   m_effect;
    };
    
    class Mesh : public cocos2d::ISmartObject
    {
    public:
        Mesh();
        ~Mesh();
        
        void setVertices(void *data, size_t size);
        void setIndices(void *data, size_t size);
        void setVertexDecl(SmartPtr<VertexDeclaration> decl);
        
        void draw();
        
    private:
        SmartPtr<VertexDeclaration> m_vertexDecl;
        SmartPtr<VertexBuffer>      m_vertexBuffer;
        SmartPtr<IndexBuffer>       m_indexBuffer;
    };
    
}//end namespace my3d

#endif //H_M3D_MESH_H