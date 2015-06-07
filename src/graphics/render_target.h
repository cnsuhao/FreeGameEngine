#ifndef ORANGE_RENDER_TARGET_H
#define ORANGE_RENDER_TARGET_H

#include "texture.h"

namespace ora
{

    class RenderTarget : public IResource
    {
    public:

        RenderTarget();
        ~RenderTarget();

        bool        create(int width, int height);

        TexturePtr  getTexture(){ return texture_; }
        uint        getFBO() { return frameBufferObject_; }

    protected:

        virtual void onDeviceClose();
        TexturePtr  bindTextrue(TexturePtr tex, bool isFBOBinded=false);

    private:
        TexturePtr  texture_;
        int         width_;
        int         height_;

        uint        frameBufferObject_;
        uint        depthBufferObject_;
    };

    typedef SmartPtr<RenderTarget> RenderTargetPtr;

} // end namespace ora

#endif //ORANGE_RENDER_TARGET_H
