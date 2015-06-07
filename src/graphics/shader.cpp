//
//  shader.cpp
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "shader.h"

#include "util/log_tool.h"

namespace ora
{
    Shader::Shader(ShaderType type, const std::string & name)
        : name_(name)
        , type_(type)
        , shader_(0)
    {
    }
    
    Shader::~Shader()
    {
        deleteShader();
    }

    void Shader::onDeviceClose()
    {
        deleteShader();
    }
    
    bool Shader::load(const std::string & code)
    {
        if(code.empty())
        {
            return false;
        }
        
        if(!createShader())
        {
            ORA_LOG_ERROR("Failed to create shader!");
            return false;
        }
        
        std::string temp;
        if (type_ == ShaderType::Vertex)
        {
            temp = "#define AS_VS\r\n";
        }
        else
        {
            temp = "#define AS_PS\r\n";
        }
        temp += code;
        temp += '\n'; // this '\n' is needed for android.

        const char * pcode [] = { temp.c_str(), };
        GLint plength [] = {GLint(temp.size()), };
        glShaderSource(shader_, 1, pcode, plength);
        
        return compile();
    }

    bool Shader::createShader()
    {
        ASSERT_1(shader_ == 0);
        
        shader_ = glCreateShader(type_ == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
        return shader_ != 0;
    }

    void Shader::deleteShader()
    {
        if(shader_ != 0)
        {
            glDeleteShader(shader_);
            shader_ = 0;
        }
    }

    bool Shader::compile()
    {
        ASSERT_1(shader_ != 0);

        glCompileShader(shader_);

        GLint result;
        glGetShaderiv(shader_, GL_COMPILE_STATUS, &result);

        int logLength;
        glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &logLength);
        if(logLength > 1)
        {
            std::string error(logLength, '\0');
            glGetShaderInfoLog(shader_, logLength, nullptr, &error[0]);

            if(result == GL_FALSE)
                ORA_LOG_ERROR("Failed to compile shader '%s': %s", name_.c_str(), error.c_str());
            else
                ORA_LOG_INFO("compile shader '%s': %s", name_.c_str(), error.c_str());
        }
        
        return result != GL_FALSE;
    }

}//end namespace ora