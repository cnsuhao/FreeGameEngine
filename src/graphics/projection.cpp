#include "projection.h"
#include "util/assert_tool.h"

namespace ora
{
    ProjCamera::ProjCamera()
        : type_(PERSPECTIVE)
        , fov_(PI_QUAR)
        , aspect_(1.0)
        , near_(1.0f)
        , far_(1000.0f)
        , dirty_(true)
    {
    }
    
    ProjCamera::ProjCamera(float fov, float aspect, float znear, float zfar)
        : type_(PERSPECTIVE)
        , fov_(fov)
        , aspect_(aspect)
        , near_(znear)
        , far_(zfar)
        , dirty_(true)
    {
    }
    
    ProjCamera::ProjCamera(float left, float right, float bottom, float top, float znear, float zfar)
        : type_(ORTHOGRAPHIC)
        , near_(znear)
        , far_(zfar)
        , dirty_(true)
    {
        rect_.left = left;
        rect_.right = right;
        rect_.bottom = bottom;
        rect_.top = top;
    }
    
    void ProjCamera::setPerspective(float fov, float aspect, float znear, float zfar)
    {
        fov_ = fov;
        aspect_ = aspect;
        near_ = znear;
        far_ = zfar;
        type_ = PERSPECTIVE;
        dirty_ = true;
    }
    
    void ProjCamera::setOrthographic(float left, float right, float bottom, float top, float znear, float zfar)
    {
        rect_.left = left;
        rect_.right = right;
        rect_.bottom = bottom;
        rect_.top = top;

        near_ = znear;
        far_ = zfar;
        type_ = ORTHOGRAPHIC;
        dirty_ = true;
    }
    
    void ProjCamera::setFov(float fov)
    {
        fov_ = fov;
        dirty_ = true;
    }
    
    void ProjCamera::setRect(const CameraRect & rc)
    {
        rect_ = rc;
        dirty_ = true;
    }
    
    void ProjCamera::setAspect(float aspect)
    {
        aspect_ = aspect;
        dirty_ = true;
    }
    
    void ProjCamera::setNear(float znear)
    {
        near_ = znear;
        dirty_ = true;
    }
    
    void ProjCamera::setFar(float zfar)
    {
        far_ = zfar;
       dirty_ = true;
    }
    
    const Matrix& ProjCamera::getMatrix() const
    {
        if (dirty_)
        {
            dirty_ = false;
            if (type_ == PERSPECTIVE)
            {
                matrix_.perspectiveProjectionGL(fov_, aspect_, near_, far_);
            }
            else
            {
                matrix_.orthogonalProjectionOffCenterGL(rect_.left, rect_.right, rect_.bottom, rect_.top, near_, far_);
            }
        }
        
        return matrix_;
    }
} // end namespace ora
