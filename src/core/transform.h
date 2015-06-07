#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "math/vector3.hpp"
#include "math/quat.hpp"
#include "math/matrix.hpp"


namespace ora
{

    /**
     * Defines a 3-dimensional transformation.
     *
     * When using the scale, rotate, and translate methods, only the
     * transform's corresponding scale, rotation, or translation
     * component is updated (it is not as if the scale, rotate, or translate
     * is applied to the transform's matrix).
     *
     * Note: To construct a Transform from a transformation matrix stored as a Matrix,
     * first decompose the Matrix into its separate translation, scale, and rotation
     * components using matrix.decompose(Vector3, Quaternion, Vector3) and then pass
     * those arguments to the appropriate constructor or set methods of Transform.
     */
    class  Transform
    {
    public:

        Transform();
        virtual ~Transform();
        
        void setTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
        void setTransform(const Vector3& scale, const Matrix& rotation, const Vector3& translation);
        void setTransform(const Transform& transform);
        void setIdentity();

        /**
         * Sets the scale factor along all axes for this transform
         * to the specified value.
         *
         * @param scale The scale factor along all axes.
         */
        void setScale(float scale);
        void setScale(const Vector3& scale);
        void setScaleX(float sx);
        void setScaleY(float sy);
        void setScaleZ(float sz);

        /**
         * Sets the rotation component for this transform to the
         * specified values.
         *
         * @param rotation The rotation as a quaternion.
         */
        void setRotation(const Quaternion& rotation);
        void setRotation(const Matrix& rotation);
        void setRotation(const Vector3& axis, float angle);
        void setRotationAlong(const Vector3& point, const Vector3& axis, float angle);
        void setRotationX(float angle);
        void setRotationY(float angle);
        void setRotationZ(float angle);

        /**
         * Sets the translation component for this transform to the
         * specified translation vector.
         *
         * @param translation The translation vector.
         */
        void setPosition(const Vector3& translation);
        void setPositionX(float tx);
        void setPositionY(float ty);
        void setPositionZ(float tz);


        /**
         * Gets the matrix corresponding to this transform.
         *
         * The matrix returned from this method is mathematically equivalent
         * to this transform only as long as this transform is not changed
         * (i.e. by calling set(), setScale(), translate(), rotateX(), etc.).
         * Once the transform has been changed, the user must call getMatrix()
         * again to get the updated matrix. Also note that changing the matrix
         * returned from this method does not change this transform.
         *
         * @return The matrix of this transform.
         */
        const Matrix& getMatrix() const;
        void setMatrix(const Matrix & mat);

        /**
         * Returns the scale for this transform.
         */
        const Vector3& getScale() const;
        float getScaleX() const;
        float getScaleY() const;
        float getScaleZ() const;

        /**
         * Returns the rotation for this transform.
         */
        const Quaternion& getRotation() const;


        /**
         * Returns the translation for this transform.
         */
        const Vector3& getPosition() const;
        float getPositionX() const;
        float getPositionY() const;
        float getPositionZ() const;

        /**
         * Returns the forward vector for this Transform.
         */
        Vector3 getForwardVector() const;

        /**
         * Returns the up vector for this Transform.
         */
        Vector3 getUpVector() const;

		/**
		* Returns the right vector for this transform.
		*/
		Vector3 getRightVector() const;

		/**
		* Returns yaw.
		*/
		float getYaw() const;

		/**
		* Returns pitch.
		*/
		float getPitch() const;

		/**
		* Returns roll.
		*/
		float getRoll() const;


        /**
         * Rotates this transform's rotation component by the given rotation.
         *
         * @param qx The quaternion x value.
         * @param qy The quaternion y value.
         * @param qz The quaternion z value.
         * @param qw The quaternion w value.
         */
        void rotate(float qx, float qy, float qz, float qw);
        void rotate(const Quaternion& rotation);
        void rotate(const Vector3& axis, float angle);
        void rotate(const Matrix& rotation);
        
        /**
         * Rotate along a specific line
         * @param point a point at the line
         * @param axis direction of the line
         * @param angle angle to rotate
         */
        void rotateAlong(const Vector3& point, const Vector3& axis, float angle);

        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        /**
         * Scales this transform's scale component by the given factor along all axes.
         *
         * @param scale The factor to scale by along all axis.
         */
        void scale(float scale);
        void scale(float sx, float sy, float sz);
        void scale(const Vector3& scale);
        void scaleX(float sx);
        void scaleY(float sy);
        void scaleZ(float sz);

        /**
         * Translates this transform's translation component by the
         * given translation vector.
         *
         * @param translation The amount to translate.
         */
        void translate(const Vector3& translation);
        void translateX(float tx);
        void translateY(float ty);
        void translateZ(float tz);
        void translateRight(float amount);
        void translateUp(float amount);
        void translateForward(float amount);

        /**
         * Transforms the specified point and stores the
         * result in the original point.
         *
         * @param point The point to transform.
         */
        void transformPoint(Vector3 & point) const;
        void transformPoint(const Vector3& point, Vector3 & dst) const;

        /**
         * Transforms the specified vector and stores the
         * result in the original vector.
         *
         * @param vector The vector to transform.
         */
        void transformVector(Vector3 & vector) const;
        void transformVector(const Vector3& vector, Vector3 & dst) const;

        /**
         *
         *
         */
        void lookAt(const Vector3& up, const Vector3& target);
        void setLook(const Vector3 & look, const Vector3 & up);


    protected:

        /**
         * Defines the matrix dirty bits for marking the translation, scale and rotation
         * components of the Transform.
         */
        enum MatrixDirtyBits
        {
            DIRTY_TRANSLATION = 0x01,
            DIRTY_SCALE = 0x02,
            DIRTY_ROTATION = 0x04,
        };

        /**
         * Marks this transform as dirty and fires transformChanged().
         */
        virtual void dirty(int matrixDirtyBits);

        virtual void copyFrom(const Transform* other);

        /**
         * The scale component of the Transform.
         */
        Vector3 scale_;

        /**
         * The rotation component of the Transform.
         */
        Quaternion rotation_;

        /**
         * The translation component of the Transform.
         */
        Vector3 position_;

        /**
         * Matrix dirty bits flag.
         */
        mutable int matrixDirtyBits_;

    private:

        /**
         * The Matrix representation of the Transform.
         */
        mutable Matrix matrix_;

    };

} // end namespace ora

#endif
