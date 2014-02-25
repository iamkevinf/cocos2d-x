#include "M3DMaterial.h"

namespace my3d
{

    Material::Material()
    {
    }


    Material::~Material()
    {
    }

    Material::Material(Color a, Color d, Color s, Color e)
        : ambient(a)
        , diffuse(d)
        , specular(s)
        , emissive(e)
    {
    }

    void Material::set(Color a, Color d, Color s, Color e)
    {
        ambient = a;
        diffuse = d;
        specular = s;
        emissive = e;
    }

}//end namespace my3d