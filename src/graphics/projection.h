#ifndef H_ORANGE_PROJECTION_H
#define H_ORANGE_PROJECTION_H

#include "math/matrix.hpp"

namespace ora
{

    struct CameraRect
    {
        float left;
        float right;
        float bottom;
        float top;
    };

    ///the camera for projection
    class ProjCamera
    {
    public:
        
        enum CameraType
        {
            PERSPECTIVE = 1,
            ORTHOGRAPHIC = 2
        };

        ProjCamera();
        ProjCamera(float fov, float aspect, float znear, float zfar);
        ProjCamera(float left, float right, float bottom, float top, float znear, float zfar);

        void setPerspective(float fov, float aspect, float znear, float zfar);
        void setOrthographic(float left, float right, float bottom, float top, float znear, float zfar);
        
        CameraType getCameraType() const{ return type_; }
        
        float getFov() const{ return fov_; }
        void setFov(float fov);
        
        float getAspect() const{ return aspect_; }
        void setAspect(float aspect);
        
        const CameraRect getRect() const { return rect_; }
        void setRect(const CameraRect & rc);
        
        float getNear() const{ return near_; }
        void setNear(float znear);
        
        float getFar() const{ return far_; }
        void setFar(float zfar);

        const Matrix& getMatrix() const;

    private:
        CameraType       type_;
        float		     fov_; // y方向上的视野范围
        float		     aspect_; // 屏幕宽高比
        CameraRect		 rect_; // 正交矩阵使用
        float		     near_;
        float		     far_;
        mutable Matrix   matrix_;
        mutable bool     dirty_;
    };
}

#endif //H_ORANGE_PROJECTION_H