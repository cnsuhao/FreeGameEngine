//
//  _math.hpp
//  liborange
//
//  Created by zhoubao on 14-6-4.
//
//

#ifndef liborange__math_h
#define liborange__math_h

#include "util/util_config.h"

#include <iostream>
#include <cmath>
#include <cassert>

#ifndef MF_ASSERT_DEBUG
#define MF_ASSERT_DEBUG assert
#endif

namespace ora
{
    const float FloatMax = 1e30f;
    const float FloatMin = -FloatMax;
    const float DefaultEpsilon = 0.000001f;

    const float MATH_FLOAT_SMALL  =          1.0e-37f;
    const float MATH_TOLERANCE    =          2e-37f;
    const float MATH_EPSILON      =          0.00001f;

    const float PI_FULL = 3.14159265f;
    const float PI_HALF = PI_FULL * 0.5f;
    const float PI_QUAR = PI_HALF * 0.5f;


    enum AXIS_INDEX
    {
        AXIS_X = 0,
        AXIS_Y = 1,
        AXIS_Z = 2,
    };

    template<typename T>
    inline const T & min2(const T & a, const T & b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    inline const T & min3(const T & a, const T & b, const T & c)
    {
        return (a < b) ? min2(a, c) : min2(b, c);
    }

    template<typename T>
    inline const T & max2(const T & a, const T & b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    inline const T & max3(const T & a, const T & b, const T & c)
    {
        return (a > b) ? max2(a, c) : max2(b, c);
    }

    inline bool almostZero(float d, float epsilon = DefaultEpsilon)
    {
        return d > -epsilon && d < epsilon;
    }

    inline bool almostEqual(float a, float b, float epsilon = DefaultEpsilon)
    {
        return fabs(a - b) < epsilon;
    }

    template<typename T>
    const T & clamp(const T & low, const T & v, const T & hig)
    {
        if (v < low) return low;
        else if (v > hig) return hig;

        return v;
    }

    template<typename T>
    void limit_in_range(T & v, const T & low, const T & high)
    {
        if (v < low) v = low;
        else if (v > high) v = high;
    }

    template<typename T>
    void limit_in_order(T & a, T & b)
    {
        if (a > b) std::swap(a, b);
    }
}

#endif
