//
//  effect_constant.cpp
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "effect_constant.h"
#include "color.h"
#include "effect.h"

#include "util/assert_tool.h"
#include "util/log_tool.h"

namespace ora
{

    EffectConstant::EffectConstant(const std::string & name)
        : name_(name)
        , location_(-1)
        , type_(0)
        , index_(0)
        , pEffect_(nullptr)
    {
    }

    EffectConstant::~EffectConstant()
    {
        for(auto it : children_)
        {
            delete it.second;
        }
    }

    Effect* EffectConstant::getEffect() const
    {
        return pEffect_;
    }

    const std::string& EffectConstant::getName() const
    {
        return name_;
    }

    const GLenum EffectConstant::getType() const
    {
        return type_;
    }

    EffectConstant *EffectConstant::getChild(const std::string & name, bool createIfMiss)
    {
        auto it = children_.find(name);
        if(it != children_.end()) return it->second;

        if(createIfMiss)
        {
            EffectConstant *p = new EffectConstant(name);
            p->pEffect_ = pEffect_;

            children_[name] = p;
            return p;
        }

        return nullptr;
    }

    EffectConstant *EffectConstant::getChildren(const std::string & name, bool createIfMiss)
    {
        ASSERT_2(!name.empty(), "EffectConstant::getChild");

        size_t pos = name.find('.');
        if(pos == name.npos)
        {
            return getChild(name, createIfMiss);
        }

        EffectConstant *pChild = getChild(name.substr(0, pos), createIfMiss);
        if(!pChild) return nullptr;

        return pChild->getChildren(name.substr(pos + 1), createIfMiss);
    }


    void EffectConstant::bindValue(float value)
    {
        GL_ASSERT( glUniform1f(location_, value) );
    }

    void EffectConstant::bindValue(const float* values, uint32 count)
    {
        GL_ASSERT( glUniform1fv(location_, count, values) );
    }

    void EffectConstant::bindValue(int value)
    {
        GL_ASSERT( glUniform1i(location_, value) );
    }

    void EffectConstant::bindValue(const int* values, uint32 count)
    {
        GL_ASSERT( glUniform1iv(location_, count, values) );
    }

    void EffectConstant::bindValue(const Matrix& value)
    {
        Matrix mat;
        mat.transpose(value);
        GL_ASSERT(glUniformMatrix4fv(location_, 1, GL_FALSE, (const float*) (&mat)));
    }

    void EffectConstant::bindValue(const Matrix* values, uint32 count, bool transposed)
    {
        ASSERT_1(values && count > 0);

        if(transposed)
        {
            glUniformMatrix4fv(location_, count, GL_FALSE, (GLfloat*) values);
        }
        else
        {
            Matrix * pBuffer = new Matrix[count];
            for (uint32 i = 0; i < count; ++i)
                pBuffer[i].transpose(values[i]);

            glUniformMatrix4fv(location_, count, GL_FALSE, (GLfloat*) pBuffer);

            delete [] pBuffer;
        }
    }

    void EffectConstant::bindValue(const Vector2& value)
    {
        GL_ASSERT( glUniform2f(location_, value.x, value.y) );
    }

    void EffectConstant::bindValue(const Vector2* values, uint32 count)
    {
        GL_ASSERT( glUniform2fv(location_, count, (GLfloat*)values) );
    }

    void EffectConstant::bindValue(const Vector3& value)
    {
        GL_ASSERT( glUniform3f(location_, value.x, value.y, value.z) );
    }

    void EffectConstant::bindValue(const Vector3* values, uint32 count)
    {
        GL_ASSERT( glUniform3fv(location_, count, (GLfloat*)values) );
    }

    void EffectConstant::bindValue(const Vector4& value)
    {
        GL_ASSERT( glUniform4f(location_, value.x, value.y, value.z, value.w) );
    }

    void EffectConstant::bindValue(const Vector4* values, uint32 count)
    {
        GL_ASSERT( glUniform4fv(location_, count, (GLfloat*)values) );
    }

    void EffectConstant::bindValue(const Color & color)
    {
        GL_ASSERT( glUniform4f(location_, color.r, color.g, color.b, color.a) );
    }

    void EffectConstant::bindValue(const MaterialColor & color)
    {
        //GL_ASSERT( glUniform4fv(location_, 4, (GLfloat*)&color) );

        EffectConstant *pConst = getChild("ambient");
        if(pConst) pConst->bindValue(color.ambient);

        pConst = getChild("diffuse");
        if (pConst) pConst->bindValue(color.diffuse);

        pConst = getChild("specular");
        if (pConst) pConst->bindValue(color.specular);

        pConst = getChild("emissive");
        if (pConst) pConst->bindValue(color.emissive);
    }

    void EffectConstant::bindValue(TexturePtr texture)
    {
        ASSERT_1(type_ == GL_SAMPLER_2D);

        texture_ = texture;

        //binds the texture
        GL_ASSERT(glActiveTexture(GL_TEXTURE0 + index_));

        if (texture)
        {
            texture->bind();
        }
        else
        {
            GL_ASSERT( glBindTexture(GL_TEXTURE_2D, 0) );
        }
        
        GL_ASSERT( glUniform1i(location_, index_) );
    }

    //////////////////////////////////////////////////////////////////

    /*static*/ std::map<std::string, EffectAutoConstant*> EffectAutoConstant::s_autoConstMap;

    /*static*/ EffectAutoConstant * EffectAutoConstant::get(const std::string & name)
    {
        auto it = s_autoConstMap.find(name);
        if (it != s_autoConstMap.end()) return it->second;

        return nullptr;
    }

    /*static*/ void EffectAutoConstant::set(const std::string & name, EffectAutoConstant * autoConst)
    {
        s_autoConstMap.insert(std::make_pair(name, autoConst));
    }

    /*static*/ void EffectAutoConstant::fini()
    {
        for (auto it = s_autoConstMap.begin(); it != s_autoConstMap.end(); ++it)
        {
            delete it->second;
        }
        s_autoConstMap.clear();
    }


    EffectAutoConstant::EffectAutoConstant()
    {

    }

    EffectAutoConstant::~EffectAutoConstant()
    {

    }

}