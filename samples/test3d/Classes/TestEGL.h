//
//  TestEGL.h
//  test3d
//
//  Created by zhoubao on 14-2-21.
//
//

#ifndef __test3d__TestEGL__
#define __test3d__TestEGL__

#include "EnumDef.h"

namespace TestEGL
{

    typedef struct
    {
        // Handle to a program object
        GLuint programObject;
        
    } UserData;

    int Init(UserData *userData);
    void Draw(UserData *userData);

}//end namespace TestEGL

#endif /* defined(__test3d__TestEGL__) */
