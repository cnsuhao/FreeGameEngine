
#include "camera.h"
#include "world.h"
#include "math/math.hpp"
#include <SDL/include/SDL.h>

// Camera dirty bits
const int CAMERA_DIRTY_VIEW = 1;
const int CAMERA_DIRTY_INV_VIEW = 2;

const int CAMERA_DIRTY_ALL = \
    CAMERA_DIRTY_VIEW | \
    CAMERA_DIRTY_INV_VIEW;

namespace ora
{
    Camera::Camera()
        : dirtyBits_(CAMERA_DIRTY_ALL)
        , controled_(false)
        , rotationSpeed_(0)
        , translationSpeed_(0)
    {
    }
    
    Camera::~Camera()
    {
    }
    
    const Matrix& Camera::getView()
    {
        if (dirtyBits_ & CAMERA_DIRTY_VIEW || matrixDirtyBits_)
        {
            view_.invert(getWorldMatrix());
            
            dirtyBits_ &= ~CAMERA_DIRTY_VIEW;
        }
        
        return view_;
    }
    
    const Matrix& Camera::getInvView()
    {
        if (dirtyBits_ & CAMERA_DIRTY_INV_VIEW || matrixDirtyBits_)
        {
            invView_ = getWorldMatrix();
            
            dirtyBits_ &= ~CAMERA_DIRTY_INV_VIEW;
        }
        
        return invView_;
    }
    
    void Camera::dirty(int flag)
    {
        Transform::dirty(flag);
        dirtyBits_ |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_INV_VIEW;
    }

    void Camera::tick(float elapse)
    {
        SceneNode::tick(elapse);

        if (!controled_)
            return;

        if (!almostZero(rotationSpeed_))
        {
            this->rotate(rotationAxis_, rotationSpeed_ * elapse);
        }

        if (!almostZero(translationSpeed_))
        {
            this->translate( translationAxis_ * (translationSpeed_ * elapse) );
        }
    }

    void Camera::doRotation(const Vector3 & axis, float speed)
    {
        rotationAxis_ = axis;
        rotationSpeed_ = speed;
    }

    void Camera::doTranslation(const Vector3 & axis, float speed)
    {
        translationAxis_ = axis;
        translationSpeed_ = speed;
    }

    void Camera::active()
    {
        World::instance()->setCamera(this);
    }
}
