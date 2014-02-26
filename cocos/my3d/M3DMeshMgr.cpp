//
//  M3DMeshMgr.cpp
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#include "M3DMeshMgr.h"

namespace my3d
{
    MeshMgr::MeshMgr()
    {
        
    }
    
    MeshMgr::~MeshMgr()
    {
        
    }
    
    MeshPtr MeshMgr::get(const std::string & resouce, bool createIfMiss)
    {
        MeshPtr mesh;
        
        auto it = m_meshes.find(resouce);
        if(it != m_meshes.end())
        {
            mesh = it->second;
        }
        else if(createIfMiss)
        {
            mesh = new Mesh();
            if(!mesh->load(resouce))
            {
                mesh = nullptr;
            }
            
            m_meshes.insert(std::make_pair(resouce, mesh.get()));
        }
        return mesh;
    }
    
    void MeshMgr::del(Mesh *pMesh)
    {
        for(auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
        {
            if(pMesh == it->second)
            {
                m_meshes.erase(it);
                return;
            }
        }
    }
    
    
   
    
}//end namespace my3d