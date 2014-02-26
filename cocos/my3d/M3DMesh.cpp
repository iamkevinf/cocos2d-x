#include "M3DMesh.h"
#include "M3DRenderDevice.h"
#include "M3DEffectConstant.h"

namespace my3d
{
    SubMesh::SubMesh()
        : m_indexCount(0)
        , m_indexStart(0)
    {
        
    }
    
    SubMesh::~SubMesh()
    {
        
    }
    
    void SubMesh::setTexture(TexturePtr tex)
    {
        m_texture = tex;
    }
    
    void SubMesh::setEffect(EffectPtr eff)
    {
        m_effect = eff;
    }
    
    void SubMesh::setPrimitive(PrimitiveType pt, uint32 start, uint32 count)
    {
        m_primitiveType = pt;
        m_indexStart = start;
        m_indexCount = count;
    }
    
    void SubMesh::draw()
    {
        if(m_indexCount == 0) return;
        
        if (m_effect && m_effect->begin())
        {
            if(m_texture)
            {
                m_texture->setFilterMode(cocos2d::Texture_Filter_LINEAR, cocos2d::Texture_Filter_LINEAR);
                m_texture->setWrapMode(cocos2d::Texture_Wrap_REPEAT, cocos2d::Texture_Wrap_REPEAT);
                
                EffectConstant *pConst = m_effect->getConstant(EffectConstType::Texture);
                if(pConst) pConst->bindValue(m_texture.get());
            }
            
            renderDev()->drawIndexedPrimitive(m_primitiveType, m_indexStart, m_indexCount);
            m_effect->end();
        }
    }
    
    /////////////////////////////////////////////////////////////
    
    Mesh::Mesh()
    {
        
    }
    
    Mesh::~Mesh()
    {
        
    }
    
    
    void Mesh::draw()
    {
        if(!m_vertexBuffer || !m_indexBuffer || !m_vertexDecl) return;
        
        m_vertexBuffer->bind();
        m_indexBuffer->bind();
        m_vertexDecl->bind();
        
        for(SubMeshPtr ptr : m_subMeshs)
        {
            ptr->draw();
        }
        
        m_vertexDecl->unbind();
        m_indexBuffer->unbind();
        m_vertexBuffer->unbind();
    }
    
    void Mesh::setVertexBuffer(VertexBufferPtr vertex)
    {
        m_vertexBuffer = vertex;
    }
    
    void Mesh::setIndexBuffer(IndexBufferPtr index)
    {
        m_indexBuffer = index;
    }
    
    void Mesh::setVertexDecl(VertexDeclarationPtr decl)
    {
        m_vertexDecl = decl;
    }
    
    void Mesh::setSubMeshes(const std::vector<SubMeshPtr> & subMeshes)
    {
        m_subMeshs = subMeshes;
    }
    
    void Mesh::addSubMeshes(SubMeshPtr subMesh)
    {
        m_subMeshs.push_back(subMesh);
    }
    
}//end namespace my3d