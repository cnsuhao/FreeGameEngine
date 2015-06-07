#ifndef CAMERA_H_
#define CAMERA_H_

#include "util/smartptr.h"
#include "scene_node.h"

namespace ora
{
    typedef SmartPtr<class Camera> CameraPtr;
    
    class Camera : public SceneNode
    {
    public:

        Camera();
        virtual ~Camera();
        
        const Matrix& getView();
        const Matrix& getInvView();

        virtual void tick(float elapse);

        void setControled(bool controled){ controled_ = controled; }
        bool getControled() const { return controled_; }

        void doRotation(const Vector3 & axis, float speed);
        void doTranslation(const Vector3 & axis, float speed);

        void active();
        
    private:

        virtual void dirty(int matrixDirtyBits);
        
        mutable Matrix   view_;
        mutable Matrix   invView_;
        mutable int      dirtyBits_;
        bool             controled_;

        Vector3         rotationAxis_;
        float           rotationSpeed_;
        Vector3         translationAxis_;
        float           translationSpeed_;
    };
    
}// end namespace ora

#endif
