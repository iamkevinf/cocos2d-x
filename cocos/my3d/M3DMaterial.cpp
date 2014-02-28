#include "M3DMaterial.h"
#include "M3DEffectConstant.h"
#include "M3DRenderDevice.h"

namespace my3d
{
    Material::Material()
    : m_bLight(false)
    {
    }


    Material::~Material()
    {
    }
    
    bool Material::load(const std::string & resouce)
    {
        m_resouce = resouce;
        return false;
    }
    
    bool Material::begin()
    {
        if(!m_effect || !m_effect->begin()) return false;
        
        EffectConstant *pConst;
        
        for (size_t i = 0; i < m_texStage.size(); ++i)
        {
            TextureStage & st = m_texStage[i];
            
            std::stringstream ss;
            ss << EffectConstType::Texture << i;
            pConst = m_effect->getConstant(ss.str());
            if(pConst)
            {
                pConst->bindValue(st.getTexture());
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter2Sys(st.getMinFilter()));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter2Sys(st.getMagFilter()));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap2Sys(st.getUWrap()));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap2Sys(st.getVWrap()));
            }
        }
        
        pConst = m_effect->getConstant(EffectConstType::Material);
        if(pConst) pConst->bindValue(m_mtlColor);
        
        return true;
    }
    
    void Material::end()
    {
        m_effect->end();
    }
    
    void Material::setTextureStage(int i, const TextureStage & stage)
    {
        m_texStage[i] = stage;
    }
    
    void Material::addTextureStage(const TextureStage & stage)
    {
        m_texStage.push_back(stage);
    }
    
    void Material::clearTextureStage()
    {
        m_texStage.clear();
    }

}//end namespace my3d