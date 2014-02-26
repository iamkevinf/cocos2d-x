#ifndef H_M3D_COLOR_H
#define H_M3D_COLOR_H

#include "M3DConfig.h"

namespace my3d
{

    namespace BitMask
    {
        const uint32 HH = 0xff << 24;
        const uint32 HL = 0xff << 16;
        const uint32 LH = 0xff << 8;
        const uint32 LL = 0xff << 0;
        const float Inv255 = 1.0f / 255.0f;
    }

    class Color
    {
    public:
        Color();
        Color(uint32 argb);
        Color(float r, float g, float b, float a);
        ~Color();

        void setARGB(uint32 cr);
        void setXRGB(uint32 cr);
        void setRGBA(uint32 cr);
        void set(float r, float g, float b, float a);

        float & operator[](int index);
        float operator[](int index) const;
        
        const Color & operator += (const Color & v);
        const Color & operator -= (const Color & v);
        const Color & operator *= (float v);
        const Color & operator /= (float v);

        Color operator + (const Color & v) const;
        Color operator - (const Color & v) const;
        Color operator * (float v) const;
        Color operator / (float v) const;

        float r;
        float g;
        float b;
        float a;

    };

    const my3d::Color ColorZero(0.0f, 0.0f, 0.0f, 0.0f);
    const my3d::Color ColorBlack(0.0f, 0.0f, 0.0f, 1.0f);
    const my3d::Color ColorWhite(1.0f, 1.0f, 1.0f, 1.0f);
    const my3d::Color ColorRed(1.0f, 0.0f, 0.0f, 1.0f);
    const my3d::Color ColorGreen(0.0f, 1.0f, 0.0f, 1.0f);
    const my3d::Color ColorBlue(0.0f, 0.0f, 1.0f, 1.0f);
    
    struct MaterialColor
    {
        Color ambient;
        Color diffuse;
        Color specular;
        Color emissive;
    };

}//end namespace my3d

#endif //H_M3D_COLOR_H