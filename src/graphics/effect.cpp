//
//  effect.cpp
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "effect.h"
#include "effect_mgr.h"
#include "shader.h"
#include "effect_constant.h"
#include "shader_parser.h"

#include "util/log_tool.h"
#include "util/file_tool.h"

namespace ora
{
    /*static*/ Effect * Effect::s_pActiveEffect = nullptr;

    /*static*/ Effect * Effect::getActiveEffect()
    {
        return s_pActiveEffect;
    }
    
    int g_effect_counter = 0;

    
    struct EffectLoadingInfo
    {
        ShaderCodePtr code;
        std::string macros;
    };

    //////////////////////////////////////////////////////////
    Effect::Effect(const std::string & resource)
        : resouce_(resource)
        , program_(0)
        , pConstRoot_(nullptr)
        , pLoadingInfo_(nullptr)
    {
        ++g_effect_counter;

        for(int i = 0; i < VertexUsageMax; ++i)
        {
            attributes_[i] = -1;
        }
    }

    Effect::~Effect()
    {
        --g_effect_counter;
        
        if (this == s_pActiveEffect)
        {
            s_pActiveEffect = nullptr;
        }

        if(pConstRoot_ != nullptr)
        {
            delete pConstRoot_;
        }

        if(pLoadingInfo_ != nullptr)
        {
            delete pLoadingInfo_;
        }

        onDeviceClose();
    }

    void Effect::onDeviceClose()
    {
        if(program_ != 0)
        {
#ifndef _RELEASE
            GLint curProgram;
            glGetIntegerv(GL_CURRENT_PROGRAM, &curProgram);
            ASSERT_2(program_ != curProgram, "the program is already in use.");
#endif
            glDeleteProgram(program_);
            program_ = 0;
        }
    }

    bool Effect::isValid() const
    {
        return program_ != 0;
    }

    bool Effect::load(const std::string & resouce)
    {
        resouce_ = resouce;

        ShaderCodePtr shader = ShaderParser::instance()->loadShader(resouce);
        if (!shader)
        {
            ORA_LOG_ERROR("Failed to load shader '%s' !", resouce.c_str());
            return false;
        }

        if (!load(shader, ""))
        {
            ORA_LOG_ERROR("Failed to load effect '%s' !", resouce_.c_str());
            return false;
        }

        return true;
    }

    bool Effect::load(ShaderCodePtr code, const std::string & macros)
    {
        ORA_LOG_TRACE("Load Effect: %s", resouce_.c_str());
        ASSERT_2(pConstRoot_ == nullptr, "the effect has been loaded.");

        pConstRoot_ = new EffectConstant("root");
        pConstRoot_->pEffect_ = this;

        std::string codes = macros + code->getCode();

#ifndef _RELEASE
        {
            std::string tempname = resouce_ + ".temp";
            std::replace(tempname.begin(), tempname.end(), '/', ' ');
            writeFile(codes, FileSystemMgr::fileSystem()->getWritablePath() + tempname, true);
        }
#endif

        EffectLoadingInfo *pInfo = new EffectLoadingInfo();
        pInfo->code = code;
        pInfo->macros = macros;

        pLoadingInfo_ = pInfo;
        return true;
    }

    void Effect::doLoading()
    {
        ASSERT_1(pLoadingInfo_ != nullptr);

        std::string codes = pLoadingInfo_->macros;
        codes += pLoadingInfo_->code->getCode();
        
        do
        {
            ShaderPtr vshPtr = new Shader(ShaderType::Vertex, resouce_);
            if (!vshPtr->load(codes))
            {
                break;
            }

            ShaderPtr pshPtr = new Shader(ShaderType::Pixel, resouce_);
            if (!pshPtr->load(codes))
            {
                break;
            }

            program_ = glCreateProgram();
            if(0 == program_)
            {
                break;
            }

            glAttachShader(program_, vshPtr->getShader());
            glAttachShader(program_, pshPtr->getShader());

            bool ret = linkProgram();
        
            glDetachShader(program_, vshPtr->getShader());
            glDetachShader(program_, pshPtr->getShader());
        
            if(ret)
            {   
                parseAttributes(pLoadingInfo_->code);
                parseUniforms();
            }
            else
            {
                glDeleteProgram(program_);
                program_ = 0;
            }
        }while(0);

        delete pLoadingInfo_;
        pLoadingInfo_ = nullptr;
    }
    
    bool Effect::linkProgram()
    {
        glLinkProgram(program_);
        
        int result;
        glGetProgramiv(program_, GL_LINK_STATUS, &result);
        
        int logLength;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
        if(logLength > 1)
        {
            std::string error(logLength, '\0');
            glGetProgramInfoLog(program_, logLength, nullptr, &error[0]);

            if(result == GL_FALSE)
                ORA_LOG_ERROR("link effect failed: '%s'", error.c_str());
            else
                ORA_LOG_INFO("link effect log: '%s'", error.c_str());
        }
        
        return result != GL_FALSE;
    }
    
    void Effect::parseAttributes(ShaderCodePtr code)
    {
        GLint numAttributes;
        GL_ASSERT( glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &numAttributes) );
        if (numAttributes <= 0) return;
        
        int maxLength;
        GL_ASSERT( glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength) );
        if (maxLength <= 0) return;
        
        std::string attribName;
        GLint   attribSize;
        GLenum  attribType;
        GLint   attribLocation;
        GLint   length;
        for (GLint i = 0; i < numAttributes; ++i)
        {
            attribName.resize(maxLength);
            glGetActiveAttrib(program_, i, maxLength, &length, &attribSize, &attribType, &attribName[0]);
            attribName.erase(length);
            
            attribLocation = glGetAttribLocation(program_, attribName.c_str());
            
            int usage = code->findSemantics(attribName);
            if(VertexUsage(usage) == VertexUsage::NONE)
            {
                ORA_LOG_ERROR("can't get usage for attribute '%s' in '%s'.",
                    attribName.c_str(), resouce_.c_str());
                continue;
            }

            attributes_[usage] = attribLocation;
        }
    }
    
    void Effect::parseUniforms()
    {
        GLint nUniforms;
        glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &nUniforms);

        if(nUniforms == 0) return;

        GLint nMaxNameLen;
        glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nMaxNameLen);

        std::string uniformName(nMaxNameLen + 1, '\0');
        unsigned int samplerIndex = 0;
        for(int i = 0; i < nUniforms; ++i)
        {
            uniformName.resize(nMaxNameLen);
            GLsizei len;
            GLint num;
            GLenum type;
            glGetActiveUniform(program_, i, nMaxNameLen, &len, &num, &type, &uniformName[0]);
            uniformName.erase(len);

            GLint location = glGetUniformLocation(program_, uniformName.c_str());

            size_t iBracket = uniformName.find('[');
            if(iBracket != std::string::npos)
                uniformName.erase(iBracket);

            EffectConstant* uniform = pConstRoot_->getChildren(uniformName, true);
            uniform->pEffect_ = this;
            uniform->location_ = location;
            uniform->type_ = type;
            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
                uniform->index_ = samplerIndex++;
            else
                uniform->index_ = 0;
        }

        for(auto it : pConstRoot_->children_)
        {
            EffectAutoConstant *pAutoConst = EffectAutoConstant::get(it.first);
            if (pAutoConst)
            {
                autoConsts_.push_back(std::make_pair(pAutoConst, it.second));
            }
        }
    }

    EffectConstant * Effect::getConstant(const std::string & name)
    {
        if(pConstRoot_ != nullptr )
        {
            return pConstRoot_->getChildren(name);
        }
        return nullptr;
    }

    bool Effect::begin()
    {
        ASSERT_2(s_pActiveEffect == nullptr, "Effect::begin - invalid operation!");

        if(pLoadingInfo_ != nullptr)
        {
            doLoading();
        }

        if(0 == program_)
        {
            return false;
        }

        if(VertexDeclaration::getActiveDecl() == nullptr)
        {
            ASSERT_2(0, "Effect::begin - failed! please set vertex declaration first.");
            return false;
        }

        glUseProgram(program_);
        s_pActiveEffect = this;

        bindAttributes();
        bindUniforms();
        return true;
    }

    void Effect::end()
    {
        ASSERT_2(s_pActiveEffect == this, "Effect::end invalied");

        unbindAttributes();

        s_pActiveEffect = nullptr;
        glUseProgram(0);
    }

    void Effect::bindUniforms()
    {
        for (auto it : autoConsts_)
        {
            it.first->apply(it.second);
        }
    }

    void Effect::bindAttributes()
    {
        VertexDeclaration *decl = VertexDeclaration::getActiveDecl();

        GLsizei vertexSize = GLsizei(decl->getVertexSize());
        size_t offset = 0;
        for(size_t i = 0; i < decl->getNumElement(); ++i)
        {
            const VertexElement & e = decl->getElement(i);

            GLint slot = attributes_[int(e.usage)];
            if(slot >= 0)
            {
                GL_ASSERT( glEnableVertexAttribArray(slot) );
                GL_ASSERT( glVertexAttribPointer(slot, e.nComponent, e.type, e.normalized, vertexSize, (GLvoid*)(offset)) );
            }
            offset += e.size();
        }
    }

    void Effect::unbindAttributes()
    {
        VertexDeclaration *decl = VertexDeclaration::getActiveDecl();

        for(size_t i = 0; i < decl->getNumElement(); ++i)
        {
            const VertexElement & e = decl->getElement(i);
            GLint slot = attributes_[int(e.usage)];
            if(slot >= 0)
            {
                GL_ASSERT( glDisableVertexAttribArray(slot) );
            }
        }
    }

}//end namespace ora
