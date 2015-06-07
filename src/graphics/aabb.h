#ifndef _MATH_AABB_H
#define _MATH_AABB_H

#include "math/vector3.hpp"
#include "math/matrix.hpp"
#include "math/boundbox.hpp"

namespace ora
{
    class AABB
    {
    public:

        Vector3 min;
        Vector3 max;


        AABB();
        AABB(const Vector3 & min, const Vector3 & max);
        AABB(const BoundingBox & bb);

        const AABB & operator = (const BoundingBox & bb);

        void reset();

        //正规化。aabb经过变换后，min可能要大于max。
        void normalization();

        Vector3 getCenter() const;
        void getCenter(Vector3 & center) const;

        Vector3 getDiameter() const;
        void getDiameter(Vector3 & diameter) const;

        ///包围盒求交集
        void sub(const AABB & aabb);

        ///包围盒求并集
        void add(const AABB & aabb);

        ///并入点
        void addPoint(const Vector3 & pt);

        bool intersectsRay(const Vector3 & origin, const Vector3 & dir, float *pDist = nullptr) const;

        bool intersect(const AABB & aabb) const;

        bool clipLine(Vector3 & start, Vector3 & end) const;

        void applyMatrix(const Matrix & mat);
        void applyMatrix(AABB & out, const Matrix & mat) const;
    };




}//end namespace ora

#endif //_MATH_AABB_H