
#include "render_target.h"
#include "util/log_tool.h"

namespace ora
{
    RenderTarget::RenderTarget()
        : width_(0)
        , height_(0)
        , frameBufferObject_(0)
        , depthBufferObject_(0)
    {

    }

    RenderTarget::~RenderTarget()
    {
        onDeviceClose();
    }

    bool RenderTarget::create(int width, int height)
    {
        onDeviceClose();

        width_ = width;
        height_ = height;
        TextureFormat format = TextureFormat::RGBA;
        int miplevel = 0;

        GLuint hTexture;
        glGenTextures(1, &hTexture);
        GL_ASSERT(glBindTexture(GL_TEXTURE_2D, hTexture));
        GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, miplevel, (GLenum) format, width_, height_, 0, (GLenum) format, GL_UNSIGNED_BYTE, nullptr));
        glBindTexture(GL_TEXTURE_2D, 0);

        texture_ = new Texture();
        texture_->create(hTexture, width_, height_, format);
        texture_->setQuality(ora::TextureQuality::TwoLinear);
    
        glGenRenderbuffers(1, &depthBufferObject_);
        GL_ASSERT(glBindRenderbuffer(GL_RENDERBUFFER, depthBufferObject_));
        GL_ASSERT(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width_, height_));
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        GLint oldFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

        glGenFramebuffers(1, &frameBufferObject_);
        GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));

        // attach a texture to FBO color attachement point
        GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hTexture, 0));

        // attach a renderbuffer to depth attachment point
        GL_ASSERT(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferObject_));

        glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

        return true;
    }

    TexturePtr RenderTarget::bindTextrue(TexturePtr tex, bool isFBOBinded)
    {
        TexturePtr oldTexture = texture_;

        GLint oldFBO = 0;
        if (!isFBOBinded)
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
            GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
        }

        // attach a texture to FBO color attachement point
        GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->getHandle(), 0));
        texture_ = tex;

        if (!isFBOBinded)
            glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

        return oldTexture;
    }

    void RenderTarget::onDeviceClose()
    {
        if (frameBufferObject_ != 0)
        {
            GLint oldFBO;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

            // detach texture and depth buffer from frame buffer
            GL_ASSERT(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
            GL_ASSERT(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0));
            GL_ASSERT(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0));
            glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

            // delete the frame buffer and depth buffer
            glDeleteFramebuffers(1, &frameBufferObject_);
            frameBufferObject_ = 0;

            glDeleteRenderbuffers(1, &depthBufferObject_);
            depthBufferObject_ = 0;
        }

        texture_ = nullptr;
    }

} // namespace MyGUI
