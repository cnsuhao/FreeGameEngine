#include "transform.h"

namespace ora
{

    Transform::Transform()
        : scale_(1.0f, 1.0f, 1.0f)
        , matrixDirtyBits_(0)
    {
    }

    Transform::~Transform()
    {
    }
    
    void Transform::setTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
    {
        scale_ = scale;
        rotation_ = rotation;
        position_ = translation;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }

    void Transform::setTransform(const Vector3& scale, const Matrix& rotation, const Vector3& translation)
    {
        scale_ = scale;
        rotation_.fromMatrix(rotation);
        position_ = translation;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }

    void Transform::setTransform(const Transform& transform)
    {
        scale_ = transform.scale_;
        rotation_ = transform.rotation_;
        position_ = transform.position_;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }
    
    void Transform::setMatrix(const Matrix & mat)
    {
        matrix_ = mat;
        matrix_.decompose(rotation_, scale_, position_);

        dirty(DIRTY_SCALE | DIRTY_ROTATION | DIRTY_TRANSLATION);
    }

    const Matrix& Transform::getMatrix() const
    {
        if (matrixDirtyBits_)
        {
            matrixDirtyBits_ = 0;
            
            matrix_.setScale(scale_);
        
            Matrix temp;
            temp.setRotate(rotation_);
            matrix_.postMultiply(temp);
            
            matrix_.translation(position_);
        }
        return matrix_;
    }

    void Transform::setIdentity()
    {
        scale_.set(1.0f, 1.0f, 1.0f);
        rotation_.setIdentity();
        position_.set(0.0f, 0.0f, 0.0f);

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }

    const Vector3& Transform::getScale() const
    {
        return scale_;
    }


    float Transform::getScaleX() const
    {
        return scale_.x;
    }

    float Transform::getScaleY() const
    {
        return scale_.y;
    }

    float Transform::getScaleZ() const
    {
        return scale_.z;
    }

    const Quaternion& Transform::getRotation() const
    {
        return rotation_;
    }


    const Vector3& Transform::getPosition() const
    {
        return position_;
    }

    float Transform::getPositionX() const
    {
        return position_.x;
    }

    float Transform::getPositionY() const
    {
        return position_.y;
    }

    float Transform::getPositionZ() const
    {
        return position_.z;
    }

    Vector3 Transform::getForwardVector() const
    {
        return getMatrix().applyToUnitAxisVector(2);
    }

    Vector3 Transform::getUpVector() const
    {
        return getMatrix().applyToUnitAxisVector(1);
    }

    Vector3 Transform::getRightVector() const
    {
        return getMatrix().applyToUnitAxisVector(0);
    }

	float Transform::getYaw() const
	{
		Vector3 v = Vector3(0.f, 0.f, 1.f);
		Vector3 look = getForwardVector();
		look.y = 0.f;
		return acos(v.dotProduct(look) / (v.length() * look.length()));
	}

	float Transform::getPitch() const
	{
#if 0
		Vector3 v = Vector3(0.f, 0.f, 1.f);
		Vector3 look = getForwardVector();
		look.x = 0.f;
		return acos(v.dotProduct(look) / (v.length() * look.length()));
#else
		return 0.f;
#endif
	}

	float Transform::getRoll() const
	{
		return 0.f;
	}

    void Transform::rotate(float qx, float qy, float qz, float qw)
    {
        Quaternion q(qx, qy, qz, qw);
        rotation_.postMultiply(q);

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotate(const Quaternion& rotation)
    {
        rotation_.postMultiply(rotation);

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotate(const Vector3& axis, float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, axis);

        rotation_.postMultiply(rotationQuat);
        rotation_.normalise();

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotate(const Matrix& rotation)
    {
        Quaternion rotationQuat;
        rotationQuat.fromMatrix(rotation);

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotateX(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(1.0f, 0.0f, 0.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotateY(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(0.0f, 1.0f, 0.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    void Transform::rotateZ(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(0.0f, 0.0f, 1.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    void Transform::setRotationX(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(1.0f, 0.0f, 0.0f));

        dirty(DIRTY_ROTATION);

    }
    void Transform::setRotationY(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(0.0f, 1.0f, 0.0f));

        dirty(DIRTY_ROTATION);
    }


    void Transform::setRotationZ(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(0.0f, 0.0f, 1.0f));

        dirty(DIRTY_ROTATION);
    }

    void Transform::scale(float scale)
    {
        scale_ *= scale;

        dirty(DIRTY_SCALE);
    }

    void Transform::scale(float sx, float sy, float sz)
    {
        scale_.x *= sx;
        scale_.y *= sy;
        scale_.z *= sz;

        dirty(DIRTY_SCALE);
    }

    void Transform::scale(const Vector3& scale)
    {
        scale_.x *= scale.x;
        scale_.y *= scale.y;
        scale_.z *= scale.z;
        dirty(DIRTY_SCALE);
    }

    void Transform::scaleX(float sx)
    {
        scale_.x *= sx;
        dirty(DIRTY_SCALE);
    }

    void Transform::scaleY(float sy)
    {
        scale_.y *= sy;
        dirty(DIRTY_SCALE);
    }

    void Transform::scaleZ(float sz)
    {
        scale_.z *= sz;
        dirty(DIRTY_SCALE);
    }

    void Transform::setScale(float scale)
    {
        scale_.set(scale, scale, scale);
        dirty(DIRTY_SCALE);
    }

    void Transform::setScale(const Vector3& scale)
    {
        scale_ = scale;

        dirty(DIRTY_SCALE);
    }

    void Transform::setScaleX(float sx)
    {
        scale_.x = sx;
        dirty(DIRTY_SCALE);
    }

    void Transform::setScaleY(float sy)
    {
        scale_.y = sy;
        dirty(DIRTY_SCALE);
    }

    void Transform::setScaleZ(float sz)
    {
        scale_.z = sz;
        dirty(DIRTY_SCALE);
    }

    void Transform::setRotation(const Quaternion& rotation)
    {
        rotation_ = rotation;
        dirty(DIRTY_ROTATION);
    }

    void Transform::setRotation(const Matrix& rotation)
    {
        rotation_.fromMatrix(rotation);
        dirty(DIRTY_ROTATION);
    }

    void Transform::setRotation(const Vector3& axis, float angle)
    {
        rotation_.fromAngleAxis(angle, axis);
        dirty(DIRTY_ROTATION);
    }

    void Transform::setPosition(const Vector3 & position)
    {
        position_ = position;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::setPositionX(float tx)
    {
        position_.x = tx;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::setPositionY(float ty)
    {
        position_.y = ty;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::setPositionZ(float tz)
    {
        position_.z = tz;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::translate(const Vector3& translation)
    {
        position_.x += translation.x;
        position_.y += translation.y;
        position_.z += translation.z;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::translateX(float tx)
    {
        position_.x += tx;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::translateY(float ty)
    {
        position_.y += ty;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::translateZ(float tz)
    {
        position_.z += tz;
        dirty(DIRTY_TRANSLATION);
    }

    void Transform::translateRight(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 right = getRightVector();

        right.normalise();
        right *= amount;

        translate(right);
    }

    void Transform::translateUp(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 up = getUpVector();

        up.normalise();
        up *= amount;

        translate(up);
    }

    void Transform::translateForward(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 forward = getForwardVector();

        forward.normalise();
        forward *= amount;

        translate(forward);
    }

    void Transform::transformPoint(Vector3 & point) const
    {
        point = getMatrix().applyPoint(point);
    }

    void Transform::transformPoint(const Vector3& point, Vector3 & dst) const
    {
        getMatrix().applyPoint(dst, point);
    }

    void Transform::transformVector(Vector3 & normal) const
    {
        normal = getMatrix().applyVector(normal);
    }

    void Transform::transformVector(const Vector3& normal, Vector3 & dst) const
    {
        getMatrix().applyVector(dst, normal);
    }


    void Transform::dirty(int matrixDirtyBits)
    {
        matrixDirtyBits_ |= matrixDirtyBits;
    }


    void Transform::lookAt(const Vector3& up, const Vector3& target)
    {
        Vector3 Up;
        Vector3 Look(target - position_);
	    Vector3 Right;

	    Look.normalise( );
	    Right.crossProduct( up, Look );
	    Right.normalise( );
	    Up.crossProduct( Look, Right );
        Up.normalise();

        Matrix rot;
	    rot.m00 = Right.x;
	    rot.m01 = Right.y;
	    rot.m02 = Right.z;
	    rot.m03 = 0.f;

	    rot.m10 = Up.x;
	    rot.m11 = Up.y;
	    rot.m12 = Up.z;
	    rot.m13 = 0.f;

	    rot.m20 = Look.x;
	    rot.m21 = Look.y;
	    rot.m22 = Look.z;
	    rot.m23 = 0.f;

	    rot.m30 = 0;
	    rot.m31 = 0;
	    rot.m32 = 0;
	    rot.m33 = 1.f;

        setRotation(rot);
    }

    void Transform::setLook(const Vector3 & look, const Vector3 & up)
    {
        Vector3 Look = look;
        Vector3 Up = up;
        Vector3 Right;

	    Right.crossProduct( up, Look );
	    Right.normalise( );
	    Up.crossProduct( Look, Right );
        Up.normalise();

        Matrix rot;
	    rot.m00 = Right.x;
	    rot.m01 = Right.y;
	    rot.m02 = Right.z;
	    rot.m03 = 0.f;

	    rot.m10 = Up.x;
	    rot.m11 = Up.y;
	    rot.m12 = Up.z;
	    rot.m13 = 0.f;

	    rot.m20 = Look.x;
	    rot.m21 = Look.y;
	    rot.m22 = Look.z;
	    rot.m23 = 0.f;

	    rot.m30 = 0;
	    rot.m31 = 0;
	    rot.m32 = 0;
	    rot.m33 = 1.f;

        setRotation(rot);
    }

    void Transform::copyFrom(const Transform* other)
    {
        scale_ = other->scale_;
        position_ = other->position_;
        rotation_ = other->rotation_;
        matrix_ = other->matrix_;

        matrixDirtyBits_ = 0;
        dirty(other->matrixDirtyBits_);
    }

    void Transform::setRotationAlong(const Vector3& point, const Vector3& axis, float angle)
    {
        rotation_.fromAngleAxis(angle, axis);

        Matrix mat;
        mat.setRotate(rotation_);
        mat.applyPoint(position_, point);
        position_ = point - position_;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION);
    }

    void Transform::rotateAlong(const Vector3& point, const Vector3& axis, float angle)
    {
        Quaternion rot;
        rot.fromAngleAxis(angle, axis);

        rotation_.postMultiply(rot);

        Matrix mat;
        mat.setRotate(rot);
        Vector3 translation = position_ - point;
        translation = mat.applyPoint(translation);

        position_ = translation + point;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION);
    }


} // end namespace ora
