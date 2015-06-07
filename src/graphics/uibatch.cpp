//
//  uibatch.cpp
//  my3d_libs
//
//  Created by zhoubao on 14-5-30.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "uibatch.h"
#include "vertex_declaration.h"
#include "effect_constant.h"
#include "render_device.h"
#include "viewport.h"
#include "effect_mgr.h"

#include "util/data_stream.h"
#include "util/log_tool.h"
#include "util/watch_time.h"

IMPLEMENT_SINGLETON(ora::UIBatch);

namespace UIShader
{
    const int Default = 0;
    const int ColorOnly = 1;
    const int TextureOnly = 2;
    const int TextureColor = 3;
}

namespace ora
{
    UIBatch::UIBatch()
        : vIndex_(0)
		, enableTestDraw_(false)
    {
        ORA_STACK_TRACE;

        for (int i = 0; i < 4; ++i)
        {
            shaders_[i] = nullptr;
        }
    }
    
    UIBatch::~UIBatch()
    {
        ORA_STACK_TRACE;
    }
    
    bool UIBatch::init()
    {
        ORA_STACK_TRACE;

        const char * shaders[] = {
            "shader/ui_default",
            "shader/ui_color_only",
            "shader/ui_texture_only",
            "shader/ui_texture_color",
        };

        for (int i = 0; i < 4; ++i)
        {
            std::string str = g_sysConfig->readString(shaders[i]);
            if (str.empty())
            {
                ORA_LOG_ERROR("UIBatch::init - can't find shader key '%s' in sysConfig", shaders[i]);
                return false;
            }

            shaders_[i] = EffectMgr::instance()->get(str);
            if (!shaders_[i])
            {
                ORA_LOG_ERROR("UIBatch::init - can't load shader '%s'", str.c_str());
                return false;
            }
        }
        
        vertexDecl_ = VertexDeclMgr::instance()->get(UIVertex::getType());
        vb_ = new VertexBufferEx<UIVertex>(BufferUsage::Dynamic, 4);
   
        return true;
    }

    bool UIBatch::beginDraw()
    {
        RenderDevice *pDevice = renderDev();
        
        oldView_ = pDevice->getView();
        oldProj_ = pDevice->getProjection();
        
        pDevice->pushWorld(Matrix::identity);
        pDevice->setView(Matrix::identity);
        pDevice->setProjection(Matrix::identity);
        pDevice->setRenderState(RenderState::DepthTest, false);
        pDevice->setRenderState(RenderState::CullFace, false);
        
        return true;
    }
    
    void UIBatch::endDraw()
    {
        realDraw();
        
        RenderDevice *pDevice = renderDev();
        
        pDevice->setRenderState(RenderState::DepthTest, true);
        pDevice->setRenderState(RenderState::CullFace, true);
        
        pDevice->setView(oldView_);
        pDevice->setProjection(oldProj_);
        pDevice->popWorld();
    }
    
    void UIBatch::drawTrangles(UIVertex *pVertices, size_t nTriangle, TexturePtr tex, bool enableColor)
    {
        EffectPtr effect = choiceEffect(tex && tex->getHandle(), enableColor);
        drawTrangles(pVertices, nTriangle, tex, effect);
    }
    
    void UIBatch::drawTrangles(UIVertex *pVertices, size_t nTriangle, TexturePtr tex, EffectPtr effect, TexturePtr secondTex/*=nullptr*/)
    {
        ASSERT_1(pVertices && effect);
        
        while(nTriangle != 0)
        {
            if (vIndex_ >= MaxBatchVertices - 3)
            {
                realDraw();
            }
            
            size_t nTriangleDraw = nTriangle;
            if (vIndex_ + nTriangleDraw * 3 > MaxBatchVertices)
            {
                nTriangleDraw = (MaxBatchVertices - vIndex_) / 3;
            }
            
            size_t nVertices = nTriangleDraw * 3;
            
            UIBatchNode node;
            node.start_ = vIndex_;
            node.count_ = nVertices;
            node.texture_ = tex;
			node.secondTexture_ = secondTex;
            node.effect_ = effect;
            
            memcpy(vertices_ + vIndex_, pVertices, nVertices * sizeof(UIVertex));
            vIndex_ += nVertices;
            
            geometries_.push_back(node);
        
        
            ASSERT_1(nTriangle >= nTriangleDraw);
            nTriangle -= nTriangleDraw;
            pVertices += nVertices;
        }
    }
    
    EffectPtr UIBatch::choiceEffect(bool enableTex, bool enableColor)
    {
        int i = UIShader::Default;
        if(enableTex && enableColor)
        {
            i = UIShader::TextureColor;
        }
        else if(enableTex)
        {
            i = UIShader::TextureOnly;
        }
        else if(enableColor)
        {
            i = UIShader::ColorOnly;
        }
        
        return shaders_[i];
    }
    
    void UIBatch::realDraw()
    {
        if(geometries_.empty()) return;
        
        WATCH_TIME(ui_realdraw);
        
        //start draw
        
        {
            WATCH_TIME(ui_realdraw_fill_vb);
            vb_->resizeBuffer(vIndex_, vertices_);
        }
        
        vertexDecl_->bind();
        vb_->bind();
        
        EffectPtr activeEffect = nullptr;
        TexturePtr activeTexture = nullptr;
        TexturePtr activeTexture2 = nullptr;
        
        for(UIBatchNode & node : geometries_)
        {
            if(node.effect_ != activeEffect)
            {
                if(activeEffect) activeEffect->end();
                
                activeEffect = node.effect_;
                activeEffect->begin();
                
                activeTexture = nullptr;
                activeTexture2 = nullptr;
            }
            
            int tex1 = node.texture_ ? node.texture_->getHandle() : 0;
            int tex2 = activeTexture ? activeTexture->getHandle() : 0;
            if(tex1 != tex2)
            {
                activeTexture = node.texture_;
                
                EffectConstant *pConst = activeEffect->getConstant(EffectConstType::Texture0);
                if(pConst)
                    pConst->bindValue(activeTexture);
            }
			
            int tex3 = node.secondTexture_ ? node.texture_->getHandle() : 0;
            int tex4 = activeTexture2 ? activeTexture2->getHandle() : 0;
            if(tex3 != tex4)
            {
                activeTexture2 = node.secondTexture_;
                
                EffectConstant *pConst = activeEffect->getConstant(EffectConstType::Texture1);
                if(pConst && activeTexture2)
                    pConst->bindValue(activeTexture2);
            }
            
            WATCH_TIME(ui_realdraw_primitive);
            renderDev()->drawPrimitive(PrimitiveType::TriangleList, node.start_, node.count_);
        }
        
        if(activeEffect) activeEffect->end();
        
        //vb_->unbind();
        //ib_->unbind();
        //vertexDecl->unbind();
        
        //clean up
        geometries_.clear();
        vIndex_ = 0;
    }
    
} // end namespace ora

