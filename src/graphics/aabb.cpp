
#include "aabb.h"

namespace ora
{

    AABB::AABB()
    {

    }

    AABB::AABB(const Vector3 & min_, const Vector3 & max_)
        : min(min_)
        , max(max_)
    {

    }

    AABB::AABB(const BoundingBox & bb)
        : min(bb.minBounds())
        , max(bb.maxBounds())
    {
    }

    const AABB & AABB::operator = (const BoundingBox & bb)
    {
        min = bb.minBounds();
        max = bb.maxBounds();
        return *this;
    }

    void AABB::reset()
    {
        min.set(FloatMax, FloatMax, FloatMax);
        max.set(FloatMin, FloatMin, FloatMin);
    }

    bool AABB::intersectsRay(const Vector3 & origin, const Vector3 & dir, float *pDist/*=nullptr*/) const
    {
        if(pDist) *pDist = FloatMax;

        Vector3 base(0.0f, 0.0f, 0.0f);
        int couter = 0;

        for (int i = 0; i < 3; ++i)
        {
            if (origin[i] <= min[i])
            {
                base[i] = min[i];
            }
            else if (origin[i] >= max[i])
            {
                base[i] = max[i];
            }
            else //该方向在包围盒里
            {
                couter++;
                float offset_min_v = origin[i] - min[i];
                float offset_max_v = max[i] - origin[i];
                base[i] = (offset_min_v < offset_max_v) ? min[i] : max[i];
            }
        }

        if (couter == 3) //起点在包围盒里
        {
            if(pDist) *pDist = 0.0f;
            return true;
        }

        Vector3 ptOnPlane;
        for (int axis = 0; axis < 3; axis++)
        {
            if (dir[axis] != 0.f)
            {
                float t = (base[axis] - origin[axis]) / dir[axis];

                if (t > 0.f)
                {
                    ptOnPlane = origin + t * dir;

                    int nextAxis = (axis + 1) % 3;
                    int prevAxis = (axis + 2) % 3;

                    if (min[nextAxis] < ptOnPlane[nextAxis] && ptOnPlane[nextAxis] < max[nextAxis] &&
                            min[prevAxis] < ptOnPlane[prevAxis] && ptOnPlane[prevAxis] < max[prevAxis])
                    {
                        if(pDist) *pDist = t;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool AABB::clipLine(Vector3 & start, Vector3 & end) const
    {
        float sMin = 0.0f;
        float eMax = 1.0f;

        Vector3 delta = end - start;

        for (int i = 0; i < 3; ++i)
        {
            if (delta[i] != 0)
            {
                float dMin = (min[i] - start[i]) / delta[i];
                float dMax = (max[i] - start[i]) / delta[i];

                if (delta[i] > 0)//射线方向从min指向max
                {
                    sMin = std::max(sMin, dMin);
                    eMax = std::min(eMax, dMax);
                }
                else//射线方向从max指向min，dMax应该和dMin交换
                {
                    sMin = std::max(sMin, dMax);
                    eMax = std::min(eMax, dMin);
                }
            }
            else if (start[i] < min[i] || start[i] > max[i]) //起点不在包围盒范围。
                return false;
        }

        end = start + eMax * delta;
        start = start + sMin * delta;

        return true;
    }

    bool AABB::intersect(const AABB & aabb) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (max[i] < aabb.min[i] || min[i] > aabb.max[i])
            {
                return false;
            }
        }
        return true;
    }

    void AABB::normalization()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (min[i] > max[i]) std::swap(min[i], max[i]);
        }
    }

    void AABB::applyMatrix(const Matrix & mat)
    {
        Vector3 corners[8];
        // Near face, specified counter-clockwise
        // Left-top-front.
        corners[0].set(min.x, max.y, max.z);
        // Left-bottom-front.
        corners[1].set(min.x, min.y, max.z);
        // Right-bottom-front.
        corners[2].set(max.x, min.y, max.z);
        // Right-top-front.
        corners[3].set(max.x, max.y, max.z);

        // Far face, specified clockwise
        // Right-top-back.
        corners[4].set(max.x, max.y, min.z);
        // Right-bottom-back.
        corners[5].set(max.x, min.y, min.z);
        // Left-bottom-back.
        corners[6].set(min.x, min.y, min.z);
        // Left-top-back.
        corners[7].set(min.x, max.y, min.z);

        reset();
        for(int i = 0; i < 8; ++i)
        {
            corners[i] = mat.applyPoint(corners[i]);
            addPoint(corners[i]);
        }
    }

    void AABB::applyMatrix(AABB & out, const Matrix & matrix) const
    {
        out = *this;
        out.applyMatrix(matrix);
    }

    void AABB::add(const AABB & aabb)
    {
        min.x = std::min(min.x, aabb.min.x);
        min.y = std::min(min.y, aabb.min.y);
        min.z = std::min(min.z, aabb.min.z);

        max.x = std::max(max.x, aabb.max.x);
        max.y = std::max(max.y, aabb.max.y);
        max.z = std::max(max.z, aabb.max.z);
    }

    void AABB::addPoint(const Vector3 & pt)
    {
        min.x = std::min(min.x, pt.x);
        min.y = std::min(min.y, pt.y);
        min.z = std::min(min.z, pt.z);

        max.x = std::max(max.x, pt.x);
        max.y = std::max(max.y, pt.y);
        max.z = std::max(max.z, pt.z);
    }

    void AABB::sub(const AABB & aabb)
    {
        min.x = std::max(min.x, aabb.min.x);
        min.y = std::max(min.y, aabb.min.y);
        min.z = std::max(min.z, aabb.min.z);

        max.x = std::min(max.x, aabb.max.x);
        max.y = std::min(max.y, aabb.max.y);
        max.z = std::min(max.z, aabb.max.z);
    }

    Vector3 AABB::getCenter() const
    {
        Vector3 v;
        getCenter(v);
        return v;
    }

    void AABB::getCenter(Vector3 & center) const
    {
        center = (max + min) * 0.5f;
    }

    Vector3 AABB::getDiameter() const
    {
        Vector3 v;
        getDiameter(v);
        return v;
    }

    void AABB::getDiameter(Vector3 & diameter) const
    {
        diameter = max - min;
    }


}//end namespace ora
