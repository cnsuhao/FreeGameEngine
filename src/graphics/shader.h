//
//  shader.h
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__Shader__
#define __liborange__Shader__

#include "base.h"
#include "render_resource.h"

namespace ora
{
    enum class ShaderType
    {
        Vertex = 0,
        Pixel = 1,
    };

    class Shader : public IReferenceCount
    {
        MAKE_UNCOPYABLE(Shader);
    public:
        Shader(ShaderType type, const std::string & name);
        virtual ~Shader();
        virtual void onDeviceClose();

        bool load(const std::string & code);
        GLuint getShader() const { return shader_; }
        const std::string & getName() const { return name_; }

    private:
        bool compile();
        
        bool createShader();
        void deleteShader();

        std::string     name_;
        ShaderType      type_;
        GLuint          shader_;
    };
    typedef SmartPtr<Shader> ShaderPtr;

}//end namespace ora

#endif /* defined(__liborange__Shader__) */
