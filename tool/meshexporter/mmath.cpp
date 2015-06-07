#include "pch.hpp"
#include "mmath.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

BoundingBox::BoundingBox()
    : min_(FLT_MAX, FLT_MAX, FLT_MAX)
    , max_(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{

}

void BoundingBox::addBounds(const Vector3 & v)
{
    for(int i = 0; i < 3; ++i)
    {
        min_[i] = std::min(min_[i], v[i]);
        max_[i] = std::max(max_[i], v[i]);
    }
}

void BoundingBox::addBounds(const BoundingBox & bb)
{
    for(int i = 0; i < 3; ++i)
    {
        min_[i] = std::min(min_[i], bb.min_[i]);
        max_[i] = std::max(max_[i], bb.max_[i]);
    }
}

void BoundingBox::setBounds( const Vector3 & min, const Vector3 & max )
{
    min_ = min;
    max_ = max;
}
