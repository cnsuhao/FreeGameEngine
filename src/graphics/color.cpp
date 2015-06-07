#include "color.h"
#include "util/assert_tool.h"

namespace ora
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
        a = ((cr >> 24) & 0xff) * BitMask::Inv255;
        r = ((cr >> 16) & 0xff) * BitMask::Inv255;
        g = ((cr >> 8) & 0xff) * BitMask::Inv255;
        b = ((cr >> 0) & 0xff) * BitMask::Inv255;
    }

    void Color::setXRGB(uint32 cr)
    {
        a = 1.0f;
        r = ((cr >> 16) & 0xff) * BitMask::Inv255;
        g = ((cr >> 8) & 0xff) * BitMask::Inv255;
        b = ((cr >> 0) & 0xff) * BitMask::Inv255;
    }

    void Color::setRGBA(uint32 cr)
    {
        r = ((cr >> 24) & 0xff) * BitMask::Inv255;
        g = ((cr >> 16) & 0xff) * BitMask::Inv255;
        b = ((cr >> 8) & 0xff) * BitMask::Inv255;
        a = ((cr >> 0) & 0xff) * BitMask::Inv255;
    }

    void Color::set(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    uint32 Color::getARGB() const
    {
        return (uint32(a * 255) << 24) || (uint32(r * 255) << 16) || \
            (uint32(g * 255) << 8) || (uint32(b * 255)) ;
    }

    uint32 Color::getRGBA() const
    {
        return (uint32(r * 255) << 24) || (uint32(g * 255) << 16) || \
            (uint32(b * 255) << 8) || (uint32(a * 255)) ;
    }

    float & Color::operator[](int index)
    {
        ASSERT_1(index >= 0 && index < 4);

        return ((float*)this)[index];
    }

    float Color::operator[](int index) const
    {
        ASSERT_1(index >= 0 && index < 4);

        return ((float*)this)[index];
    }

    const Color & Color::operator += (const Color & v)
    {
        r += v.r;
        g += v.g;
        b += v.b;
        a += v.a;
        return *this;
    }

    const Color & Color::operator -= (const Color & v)
    {
        r -= v.r;
        g -= v.g;
        b -= v.b;
        a -= v.a;
        return *this;
    }

    const Color & Color::operator *= (float v)
    {
        r *= v;
        g *= v;
        b *= v;
        a *= v;
        return *this;
    }

    const Color & Color::operator /= (float v)
    {
        r /= v;
        g /= v;
        b /= v;
        a /= v;
        return *this;
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

}//end namespace ora