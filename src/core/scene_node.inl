
namespace ora
{
    INLINE
    void SceneNode::setTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
    {
        scale_ = scale;
        rotation_ = rotation;
        position_ = translation;

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }
    
    INLINE
    void SceneNode::setMatrix(const Matrix & mat)
    {
        matrix_ = mat;
        matrix_.decompose(rotation_, scale_, position_);

        dirty(DIRTY_SCALE | DIRTY_ROTATION | DIRTY_TRANSLATION);
    }

    INLINE
    void SceneNode::setTransformIdentity()
    {
        scale_.set(1.0f, 1.0f, 1.0f);
        rotation_.setIdentity();
        position_.set(0.0f, 0.0f, 0.0f);

        dirty(DIRTY_TRANSLATION | DIRTY_ROTATION | DIRTY_SCALE);
    }

    INLINE
    const Vector3& SceneNode::getScale() const
    {
        return scale_;
    }

    INLINE
    const Quaternion& SceneNode::getRotation() const
    {
        return rotation_;
    }

    INLINE
    const Vector3& SceneNode::getPosition() const
    {
        return position_;
    }

    INLINE
    Vector3 SceneNode::getForwardVector() const
    {
        return getMatrix().applyToUnitAxisVector(2);
    }

    INLINE
    Vector3 SceneNode::getUpVector() const
    {
        return getMatrix().applyToUnitAxisVector(1);
    }

    INLINE
    Vector3 SceneNode::getRightVector() const
    {
        return getMatrix().applyToUnitAxisVector(0);
    }

    INLINE
	float SceneNode::getYaw() const
	{
		Vector3 v = Vector3(0.f, 0.f, 1.f);
		Vector3 look = getForwardVector();
		look.y = 0.f;
		return acos(v.dotProduct(look) / (v.length() * look.length()));
	}

    INLINE
	float SceneNode::getPitch() const
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

    INLINE
	float SceneNode::getRoll() const
	{
		return 0.f;
	}

    INLINE
    void SceneNode::rotate(const Quaternion& rotation)
    {
        rotation_.postMultiply(rotation);

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::rotateX(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(1.0f, 0.0f, 0.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::rotateY(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(0.0f, 1.0f, 0.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::rotateZ(float angle)
    {
        Quaternion rotationQuat;
        rotationQuat.fromAngleAxis(angle, Vector3(0.0f, 0.0f, 1.0f));

        rotation_.postMultiply(rotationQuat);

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::setRotationX(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(1.0f, 0.0f, 0.0f));

        dirty(DIRTY_ROTATION);

    }

    INLINE
    void SceneNode::setRotationY(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(0.0f, 1.0f, 0.0f));

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::setRotationZ(float angle)
    {
        rotation_.fromAngleAxis(angle, Vector3(0.0f, 0.0f, 1.0f));

        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::scale(const Vector3& scale)
    {
        scale_.x *= scale.x;
        scale_.y *= scale.y;
        scale_.z *= scale.z;
        dirty(DIRTY_SCALE);
    }

    INLINE
    void SceneNode::setScale(const Vector3& scale)
    {
        scale_ = scale;

        dirty(DIRTY_SCALE);
    }

    INLINE
    void SceneNode::setRotation(const Quaternion& rotation)
    {
        rotation_ = rotation;
        dirty(DIRTY_ROTATION);
    }

    INLINE
    void SceneNode::setPosition(const Vector3 & position)
    {
        position_ = position;
        dirty(DIRTY_TRANSLATION);
    }

    INLINE
    void SceneNode::translate(const Vector3& translation)
    {
        position_.x += translation.x;
        position_.y += translation.y;
        position_.z += translation.z;
        dirty(DIRTY_TRANSLATION);
    }

    INLINE
    void SceneNode::translateRight(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 right = getRightVector();

        right.normalise();
        right *= amount;

        translate(right);
    }

    INLINE
    void SceneNode::translateUp(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 up = getUpVector();

        up.normalise();
        up *= amount;

        translate(up);
    }

    INLINE
    void SceneNode::translateForward(float amount)
    {
        // Force the current transform matrix to be updated.
        getMatrix();

        Vector3 forward = getForwardVector();

        forward.normalise();
        forward *= amount;

        translate(forward);
    }

    INLINE
    void SceneNode::removeFromParent()
    {
        if(parent_)
            parent_->delChild(this);
    }

    INLINE
    void SceneNode::delChildByName(const std::string & name)
    {
        SceneNode * child = getChild(name);
        if(child) delChild(child);
    }
} // end namespace ora
