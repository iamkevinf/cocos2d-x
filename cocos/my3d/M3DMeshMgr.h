//
//  M3DMeshMgr.h
//  cocos3d_libs
//
//  Created by zhoubao on 14-2-26.
//  Copyright (c) 2014年 zhoubao. All rights reserved.
//

#ifndef __cocos3d_libs__M3DMeshMgr__
#define __cocos3d_libs__M3DMeshMgr__

#include "M3DMesh.h"
#include "mytool/singleton.h"

namespace my3d
{
    class MeshMgr : public Singleton<MeshMgr>
    {
      
    public:
        MeshMgr();
        ~MeshMgr();
        
        MeshPtr get(const std::string & resouce, bool createIfMiss=true);
        void del(Mesh *pMesh);
        
    private:
        
        std::map<std::string, Mesh*> m_meshes;
    };
    
}//end namespace my3d

#endif /* defined(__cocos3d_libs__M3DMeshMgr__) */
