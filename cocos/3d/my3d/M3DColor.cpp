#include "M3DColor.h"
#include <cassert>

namespace my3d
{


    Color::Color()
        : r(0.0f), g(0.0f), b(0.0f), a(0.0f)
    {}

    Color::~Color()
    {}

    Color::Color(uint32 argb)
    {
        setARGB(argb);
    }

    Color::Color(float r_, float g_, float b_, float a_)
        : r(r_) , g(g_) , b(b_) , a(a_)
    {}

    void Color::setARGB(uint32 cr)
    {
        a = (cr & BitMask::HH) * BitMask::Inv255;
        r = (cr & BitMask::HL) * BitMask::Inv255;
        g = (cr & BitMask::LH) * BitMask::Inv255;
        b = (cr & BitMask::HH) * BitMask::Inv255;
    }

    void Color::setXRGB(uint32 cr)
    {
        a = 1.0f;
        r = (cr & BitMask::HL) * BitMask::Inv255;
        g = (cr & BitMask::LH) * BitMask::Inv255;
        b = (cr & BitMask::HH) * BitMask::Inv255;
    }

    void Color::setRGBA(uint32 cr)
    {
        r = (cr & BitMask::HH) * BitMask::Inv255;
        g = (cr & BitMask::HL) * BitMask::Inv255;
        b = (cr & BitMask::LH) * BitMask::Inv255;
        a = (cr & BitMask::HH) * BitMask::Inv255;
    }

    void Color::set(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    float & Color::operator[](int index)
    {
        assert(index >= 0 && index < 4);

        return ((float*)this)[index];
    }

    float Color::operator[](int index) const
    {
        assert(index >= 0 && index < 4);

        return ((float*)this)[index];
    }

    const Color & Color::operator += (const Color & v)
    {
        r += v.r;
        g += v.g;
        v += v.b;
        a += v.a;
    }

    const Color & Color::operator -= (const Color & v)
    {
        r -= v.r;
        g -= v.g;
        b -= v.b;
        a -= v.a;
    }

    const Color & Color::operator *= (float v)
    {
        r *= v;
        g *= v;
        b *= v;
        a *= v;
    }

    const Color & Color::operator /= (float v)
    {
        r /= v;
        g /= v;
        b /= v;
        a /= v;
    }

    Color Color::operator + (const Color & v) const
    {
        return Color(r + v.r, g + v.g, b + v.b, a + v.a);
    }

    Color Color::operator - (const Color & v) const
    {
        return Color(r - v.r, g - v.g, b - v.b, a - v.a);
    }
    Color Color::operator * (float v) const
    {
        return Color(r * v, g * v, b * v, a * v);
    }

    Color Color::operator / (float v) const
    {
        return Color(r / v, g / v, b / v, a / v);
    }

}//end namespace my3d