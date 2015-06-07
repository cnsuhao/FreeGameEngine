//
//  vertex_declaration.h
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__VertexDeclaration__
#define __liborange__VertexDeclaration__

#include "util/smartptr.h"
#include "util/singleton.h"
#include "util/section.h"

#include <map>

namespace ora
{
    class Effect;

    enum class VertexUsage
    {
        NONE,
        POSITION,
        NORMAL,
        COLOR,
        TANGENT,
        BINORMAL,
        BLENDWEIGHTS,
        BLENDINDICES,
        PSIZE,
        TESSFACTOR,
        POSITIONT,
        FOG,
        DEPTH,
        SAMPLE,
        TEXCOORD0,
        TEXCOORD1,
        TEXCOORD2,
        TEXCOORD3,
        TEXCOORD4,
        TEXCOORD5,
        TEXCOORD6,
        TEXCOORD7,

        MAX_NUM
    };

    const int VertexUsageMax = int(VertexUsage::MAX_NUM);

    VertexUsage vertexAttr2Usage(const std::string & attr);

    ////////////////////////////////////////////////////////////////////
    /// VertexElement
    ////////////////////////////////////////////////////////////////////
    class VertexElement
    {
    public:

        VertexUsage usage;//The vertex element usage semantic.
        int         nComponent;//The number of values in the vertex element.
        int         type;
        int         tpSize;
        bool        normalized;

        VertexElement();
        VertexElement(VertexUsage usage, int nComponent);
        VertexElement(VertexUsage usage, int nComponent,
            int type, int tpsize, bool normalized);

        int size() const;
    };

    ////////////////////////////////////////////////////////////////////
    /// VertexDeclaration
    ////////////////////////////////////////////////////////////////////
    class VertexDeclaration : public IReferenceCount
    {
    public:

        VertexDeclaration(const std::string & name = "");
        virtual ~VertexDeclaration();

        bool load(SectionPtr section);

        void addElement(const VertexElement & e);
        void addElement(VertexUsage usage, int nComponent);
        const VertexElement & getElement(size_t i) const;
        size_t getNumElement() const;
        size_t getVertexSize() const;

        const std::string & getName() const { return name_; }

        void bind();
        void unbind();

        void merge(VertexDeclaration * p);

        static VertexDeclaration * getActiveDecl() { return s_pActiveDecl; }

    private:
        std::string name_;
        size_t vertexSize_;
        std::vector<VertexElement> elements_;

        static VertexDeclaration * s_pActiveDecl;
    };

    typedef SmartPtr<VertexDeclaration> VertexDeclarationPtr;

    ////////////////////////////////////////////////////////////////////
    /// Vertex Declaration Manager
    ////////////////////////////////////////////////////////////////////
    class VertexDeclMgr : public Singleton<VertexDeclMgr>
    {
    public:
        VertexDeclMgr();
        ~VertexDeclMgr();

        void init();

        void add(VertexDeclarationPtr decl);

        VertexDeclarationPtr combine(
            VertexDeclarationPtr orig, VertexDeclarationPtr extra );

        VertexDeclarationPtr get(const std::string & name);

    private:
        std::map<std::string, VertexDeclarationPtr> decls_;
    };

}//end namespace ora


#endif /* defined(__liborange__VertexDeclaration__) */
