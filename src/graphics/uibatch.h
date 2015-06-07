//
//  uibatch.h
//  my3d_libs
//
//  Created by zhoubao on 14-5-30.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __my3d_libs__UIBatch__
#define __my3d_libs__UIBatch__

#include "util/util_config.h"
#include "util/singleton.h"
#include "texture.h"
#include "effect.h"
#include "vertex.h"
#include "vertex_buffer.h"
#include "moo/vertex_formats.hpp"


namespace ora
{
    const size_t MaxBatchTriangles = 5000;
    const size_t MaxBatchVertices = (MaxBatchTriangles * 3 + 63) & ~63; //64 byte align.

    typedef Moo::VertexXYZDUV UIVertex;
    
    struct UIBatchNode
    {
        size_t      start_;
        size_t      count_;
        TexturePtr  texture_;
		TexturePtr	secondTexture_;
        EffectPtr   effect_;
    };

    class UIBatch :
        public Singleton<UIBatch>
    {
    public:
        UIBatch();
        ~UIBatch();
        
        bool init();
        bool beginDraw();
        void endDraw();
        
        /** each triangle has 3 vertices in clockwise*/
        void drawTrangles(UIVertex *pVertices, size_t nTriangle, TexturePtr tex, bool enableColor=true);
        void drawTrangles(UIVertex *pVertices, size_t nTriangle, TexturePtr tex, EffectPtr effect, TexturePtr secondTex=nullptr);
        
		void setTestDrawEnable(bool enable){ enableTestDraw_ = enable; }

    private:
        EffectPtr choiceEffect(bool enableTex, bool enableColor);
        void realDraw();
    
        UIVertex                    vertices_[MaxBatchVertices];
        size_t                      vIndex_;
        std::vector<UIBatchNode>    geometries_;
        
        SmartPtr<VertexBufferEx<UIVertex>>  vb_;
        VertexDeclarationPtr        vertexDecl_;
        
        Matrix                      oldProj_;
        Matrix                      oldView_;
        EffectPtr                   shaders_[4];
		bool                        enableTestDraw_;
    };
    
} // end namespace ora

#endif /* defined(__my3d_libs__UIBatch__) */
