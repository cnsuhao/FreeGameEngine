#ifndef H__COLOR_H
#define H__COLOR_H

#include "pre_def.h"

namespace ora
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

        uint32 getARGB() const;
        uint32 getRGBA() const;

        operator uint32() const { return getRGBA(); }

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

    const ora::Color ColorZero(0.0f, 0.0f, 0.0f, 0.0f);
    const ora::Color ColorBlack(0.0f, 0.0f, 0.0f, 1.0f);
    const ora::Color ColorWhite(1.0f, 1.0f, 1.0f, 1.0f);
    const ora::Color ColorRed(1.0f, 0.0f, 0.0f, 1.0f);
    const ora::Color ColorGreen(0.0f, 1.0f, 0.0f, 1.0f);
    const ora::Color ColorBlue(0.0f, 0.0f, 1.0f, 1.0f);

    struct MaterialColor
    {
        Color ambient;
        Color diffuse;
        Color specular;
        Color emissive;
    };

}//end namespace ora

#endif //H__COLOR_H