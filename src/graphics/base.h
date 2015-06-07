#ifndef H__BASE_H
#define H__BASE_H

#include "util/util_config.h"

// Graphics (OpenGL)
#if defined(ANDROID)

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define glClearDepth glClearDepthf
#define OPENGL_ES
//#define USE_ETC

extern PFNGLBINDVERTEXARRAYOESPROC      glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSOESPROC   glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSOESPROC      glGenVertexArrays;
extern PFNGLISVERTEXARRAYOESPROC        glIsVertexArray;

#elif defined(WIN32)

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#ifndef GLEW_NO_GLU
#define GLEW_NO_GLU
#endif

#define WIN32_LEAN_AND_MEAN
#include <glew/glew.h>
#define USE_VAO

#elif defined(__APPLE__)

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define glBindVertexArray           glBindVertexArrayOES
#define glDeleteVertexArrays        glDeleteVertexArraysOES
#define glGenVertexArrays           glGenVertexArraysOES
#define glIsVertexArray             glIsVertexArrayOES
#define glMapBuffer					glMapBufferOES
#define glUnmapBuffer				glUnmapBufferOES
#define glClearDepth                glClearDepthf

#define OPENGL_ES
//#define USE_PVRTC
#define USE_VAO

#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES

#elif TARGET_OS_MAC

#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#define glBindVertexArray           glBindVertexArrayAPPLE
#define glDeleteVertexArrays        glDeleteVertexArraysAPPLE
#define glGenVertexArrays           glGenVertexArraysAPPLE
#define glIsVertexArray             glIsVertexArrayAPPLE
#define USE_VAO
#define GL_MAX_VERTEX_UNIFORM_VECTORS GL_MAX_VERTEX_UNIFORM_COMPONENTS

#else
#error "Unsupported Apple Device"
#endif

#endif


/**
* GL assertion that can be used for any OpenGL function call.
*
* This macro will assert if an error is detected when executing
* the specified GL code. This macro will do nothing in release
* mode and is therefore safe to use for realtime/per-frame GL
* function calls.
*/
#ifdef _RELEASE
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) \
do{ \
	glGetError();\
    gl_code; \
    __gl_error_code = glGetError(); \
    if (__gl_error_code != GL_NO_ERROR) \
    { \
        ORA_LOG_FATAL("OpenGL Error: " #gl_code ": 0x%x", (int) __gl_error_code); \
    } \
} while (0)
#endif

/**
* Executes the specified GL code and checks the GL error afterwards
* to ensure it succeeded.
*
* This macro should be used instead of GL_ASSERT for code that must
* be checked in both debug and release builds. The GL_LAST_ERROR
* macro can be used afterwards to check whether a GL error was
* encountered executing the specified code.
*/
#define GL_CHECK( gl_code ) \
do{ \
    while (glGetError() != GL_NO_ERROR); \
    gl_code; \
    __gl_error_code = glGetError(); \
    if (__gl_error_code != GL_NO_ERROR) \
    { \
        ORA_LOG_ERROR("OpenGL Error: " #gl_code ": %d", (int) __gl_error_code); \
    } \
} while (0)

// Global variable to hold GL errors
extern GLenum __gl_error_code;

/**
* Accesses the most recently set global GL error.
*/
#define GL_LAST_ERROR() __gl_error_code


namespace ora
{
    typedef GLint  VertexAttribute;
    typedef GLuint VertexBufferHandle;
    typedef GLuint IndexBufferHandle;
    typedef GLuint TextureHandle;
    typedef GLuint FrameBufferHandle;
    typedef GLuint RenderBufferHandle;
    typedef GLuint ProgramHandle;
}

#include "pre_def.h"

#include "math/vector2.hpp"
#include "math/vector3.hpp"
#include "math/vector4.hpp"
#include "math/matrix.hpp"
#include "math/quat.hpp"

#endif //H__BASE_H
