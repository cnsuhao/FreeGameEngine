#ifndef H__LIGHT_DIR_H
#define H__LIGHT_DIR_H

#include "base.h"
#include "color.h"

namespace ora
{

    struct DirLightInfo
    {
        Vector4 direction;
        Color   color;
        Vector3 halfVector;
    };

    class DirLight : public IReferenceCount
    {
    public:
        DirLight();
        ~DirLight();

        void setColor(const Color & cr)
        {
            color_ = cr;
        }
        const Color & getColor() const
        {
            return color_;
        }

        void setDirection(const Vector3 & dir)
        {
            direction_ = dir;
            direction_.normalise();
        }
        
        const Vector3 & getDirection() const
        {
            return direction_;
        }

        void getEffectData(DirLightInfo & info);
    private:
        Vector3 direction_;
        Color   color_;
    };

    typedef SmartPtr<DirLight> DirLightPtr;

}//end namespace ora

#endif //H__LIGHT_DIR_H