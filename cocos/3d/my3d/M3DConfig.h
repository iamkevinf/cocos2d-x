#ifndef H_M3D_CONFIG_H
#define H_M3D_CONFIG_H

namespace my3d
{
    typedef signed char     int8;
    typedef unsigned char   uint8;
    typedef short           int16;
    typedef unsigned short  uint16;
    typedef int             int32;
    typedef unsigned int    uint32;

    class RenderDevice;
    RenderDevice * renderDev();

}//end namepsace my3d


#endif