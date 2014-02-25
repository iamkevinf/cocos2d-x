#ifndef H_M3D_MATERIAL_H
#define H_M3D_MATERIAL_H

#include "M3DColor.h"

namespace my3d
{

    class Material
    {
    public:
        Material();
        Material(Color ambient, Color diffuse, Color specular, Color emissive);
        ~Material();

        void set(Color ambient, Color diffuse, Color specular, Color emissive);

        Color ambient;
        Color diffuse;
        Color specular;
        Color emissive;
    };

}//end namespace my3d

#endif //H_M3D_MATERIAL_H