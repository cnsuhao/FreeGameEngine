#pragma once

#include <d3dx9.h>


typedef signed char     int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef __int64         int64;
typedef unsigned __int64 uint64;

typedef uint32          uint;

#define FLT_MAX         3.402823466e+38F        /* max value */
#define FLT_MIN         1.175494351e-38F        /* min positive value */

namespace Math
{
    template <class TYPE>
    inline TYPE clamp( TYPE minValue, TYPE value, TYPE maxValue )
    {
        return value < minValue ? minValue :
            ( value > maxValue ? maxValue : value);
    }
}

class Vector2 : public D3DXVECTOR2
{
public:
    Vector2()
    {
    }

    Vector2(float x_, float y_)
        : D3DXVECTOR2(x_, y_)
    {
    }

    inline void set(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

};

class Vector3 : public D3DXVECTOR3
{
public:
    Vector3()
        : D3DXVECTOR3(0, 0, 0)
    {
    }

    Vector3(float x_, float y_, float z_)
        : D3DXVECTOR3(x_, y_, z_)
    {
    }

    inline void set(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    void setZero()
    {
        memset(this, 0, sizeof(Vector3));
    }

    void normalise()
    {
        D3DXVec3Normalize(this, this);
    }

    const Vector3 & operator = (const Vector3 & v)
    {
        set(v.x, v.y, v.z);
        return *this;
    }

    const Vector3 & operator *= (float v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    Vector3 operator * (float v) const
    {
        return Vector3(x * v, y * v, z * v);
    }

    Vector3 operator / (float v) const
    {
        v = 1.0f / v;
        return *this * v;
    }
};

class Vector4 : public D3DXVECTOR4
{
public:
    Vector4()
    {

    }

    Vector4(float x_, float y_, float z_, float w_)
        : D3DXVECTOR4(x_, y_, z_, w_)
    {
    }

    inline void set(float x_, float y_, float z_, float w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }
};

class BoundingBox
{
public:
    BoundingBox();

    void addBounds(const Vector3 & pt);
    void addBounds(const BoundingBox & bb);

    void setBounds( const Vector3 & min, const Vector3 & max );

    Vector3 min_;
    Vector3 max_;
};


typedef D3DXCOLOR				Colour;
typedef D3DCOLOR				PackedColour;

/**
	* Unpack compressed normal into a three float vector.
	*/
inline Vector3 unpackNormal( uint32 packed )
{
	int32 z = int32(packed) >> 22;
	int32 y = int32( packed << 10 ) >> 21;
	int32 x = int32( packed << 21 ) >> 21;

	return Vector3( float( x ) / 1023.f, float( y ) / 1023.f, float( z ) / 511.f );
}

/**
	* Pack three float normal (each component clamped to [-1,1]) into a single
	* unsigned 32bit word ( 11 bits x, 11 bits y, 10 bits z )
	*/
inline uint32 packNormal( const Vector3& nn )
{
	Vector3 n = nn;
	n.normalise();

	n.x = Math::clamp(-1.f, n.x, 1.f);
	n.y = Math::clamp(-1.f, n.y, 1.f);
	n.z = Math::clamp(-1.f, n.z, 1.f);


	return	( ( ( (uint32)(n.z * 511.0f) )  & 0x3ff ) << 22L ) |
			( ( ( (uint32)(n.y * 1023.0f) ) & 0x7ff ) << 11L ) |
			( ( ( (uint32)(n.x * 1023.0f) ) & 0x7ff ) <<  0L );
}
