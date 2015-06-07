//
//  vertex_declaration.cpp
//  liborange
//
//  Created by zhoubao on 14-2-21.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "vertex_declaration.h"
#include "effect.h"
#include "vertex.h"
#include "util/string_tool.h"
#include "util/log_tool.h"

#include "util/section_factory.h"

namespace ora
{
    namespace
    {
        std::map<std::string, VertexUsage> AttrMap;
        std::map<std::string, VertexElement> TypeMap;

        bool registerUsage()
        {
#define REGISTER_ATTR(ATTR, USAGE) \
    AttrMap[ATTR] = VertexUsage::USAGE

            REGISTER_ATTR("POSITION",    POSITION);
            REGISTER_ATTR("NORMAL",      NORMAL);
            REGISTER_ATTR("COLOR",       COLOR);
            REGISTER_ATTR("TANGENT",     TANGENT);
            REGISTER_ATTR("BINORMAL",    BINORMAL);
            REGISTER_ATTR("BLENDWEIGHT", BLENDWEIGHTS);
            REGISTER_ATTR("BLENDINDICES",BLENDINDICES);
            REGISTER_ATTR("PSIZE",       PSIZE);
            REGISTER_ATTR("TESSFACTOR",  TESSFACTOR);
            REGISTER_ATTR("POSITIONT",   POSITIONT);
            REGISTER_ATTR("FOG",         FOG);
            REGISTER_ATTR("DEPTH",       DEPTH);
            REGISTER_ATTR("SAMPLE",      SAMPLE);
            REGISTER_ATTR("TEXCOORD",    TEXCOORD0 );
            REGISTER_ATTR("TEXCOORD0",   TEXCOORD0);
            REGISTER_ATTR("TEXCOORD1",   TEXCOORD1);
            REGISTER_ATTR("TEXCOORD2",   TEXCOORD2);
            REGISTER_ATTR("TEXCOORD3",   TEXCOORD3);
            REGISTER_ATTR("TEXCOORD4",   TEXCOORD4);
            REGISTER_ATTR("TEXCOORD5",   TEXCOORD5);
            REGISTER_ATTR("TEXCOORD6",   TEXCOORD6);
            REGISTER_ATTR("TEXCOORD7",   TEXCOORD7);

#undef REGISTER_ATTR

#define REGISTER_TYPE(NAME, COM, TYPE, TP_SIZE, NML) \
    TypeMap[NAME] = VertexElement(VertexUsage::NONE, COM, TYPE, TP_SIZE, NML)

            REGISTER_TYPE( "FLOAT1",    1, GL_FLOAT, 4, false );
		    REGISTER_TYPE( "FLOAT2",    2, GL_FLOAT, 4, false );
		    REGISTER_TYPE( "FLOAT3",    3, GL_FLOAT, 4, false );
		    REGISTER_TYPE( "FLOAT4",    4, GL_FLOAT, 4, false );
		    REGISTER_TYPE( "D3DCOLOR",  4, GL_UNSIGNED_BYTE, 1, true );
		    REGISTER_TYPE( "UBYTE4",    4, GL_UNSIGNED_BYTE, 1, false );
		    REGISTER_TYPE( "UBYTE4N",   4, GL_UNSIGNED_BYTE, 1, true );
            REGISTER_TYPE( "SHORT2",    2, GL_SHORT, 2, false );
		    REGISTER_TYPE( "SHORT4",    4, GL_SHORT, 2, false );
		    REGISTER_TYPE( "SHORT2N",   2, GL_SHORT, 2, true );
		    REGISTER_TYPE( "SHORT4N",   4, GL_SHORT, 2, true );
            REGISTER_TYPE( "USHORT2N",  2, GL_UNSIGNED_SHORT, 2, true );
		    REGISTER_TYPE( "USHORT4N",  4, GL_UNSIGNED_SHORT, 2, true );

#undef REGISTER_TYPE

            return true;
        }

        bool _token = registerUsage();


    }

    VertexUsage vertexAttr2Usage(const std::string & attr)
    {
        auto it = AttrMap.find(attr);
        if(it != AttrMap.end())
            return it->second;

        return VertexUsage::NONE;
    }

    bool vertexType2Element(const std::string & type, VertexElement & e)
    {
        auto it = TypeMap.find(type);
        if(it != TypeMap.end())
        {
            e = it->second;
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    VertexElement::VertexElement()
        : nComponent(0)
        , type(GL_FLOAT)
        , normalized(GL_TRUE)
        , tpSize(sizeof(float))
    {}

    VertexElement::VertexElement(VertexUsage usage_, int nComponent_)
        : usage(usage_)
        , nComponent(nComponent_)
        , type(GL_FLOAT)
        , normalized(GL_TRUE)
        , tpSize(sizeof(float))
    {}

    VertexElement::VertexElement(VertexUsage usage_, int nComponent_,
        int type_, int tpsize_, bool normalized_)
        : usage(usage_)
        , nComponent(nComponent_)
        , type(type_)
        , tpSize(tpsize_)
        , normalized(normalized_)
    {}

    int VertexElement::size() const
    {
        return tpSize * nComponent;
    }
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    /*static*/ VertexDeclaration * VertexDeclaration::s_pActiveDecl = nullptr;

    VertexDeclaration::VertexDeclaration(const std::string & name)
        : name_(name)
        , vertexSize_(0)
    {

    }

    VertexDeclaration::~VertexDeclaration()
    {
        if(this == s_pActiveDecl) s_pActiveDecl = nullptr;
    }

    bool VertexDeclaration::load(SectionPtr section)
    {
        SectionPtrVector secs;
        section->getChildren(secs);

        for(SectionPtrVector::iterator it = secs.begin();
            it != secs.end(); ++it)
        {
            std::string usage = (*it)->getTag();
            if(usage == "alias")
                continue;

            VertexElement element;
            std::string type = (*it)->readString("type", "FLOAT3");
            if(!vertexType2Element(type, element))
            {
                ORA_LOG_ERROR("VertexDeclaration '%s' found unsupported type '%s'",
                    name_.c_str(), type.c_str());
                return false;
            }

            int offset = (*it)->getInt();
            element.usage = vertexAttr2Usage( usage );
            element.usage = VertexUsage(int(element.usage) + offset);
            if(element.usage == VertexUsage::NONE)
            {
                ORA_LOG_ERROR("VertexDeclaration '%s' found unsupported usage '%s'",
                    name_.c_str(), usage.c_str());
                return false;
            }
            
            this->addElement(element);
        }

        return true;
    }

    void VertexDeclaration::addElement(const VertexElement & e)
    {
        elements_.push_back(e);
        vertexSize_ += e.size();
    }

    void VertexDeclaration::addElement(VertexUsage usage, int nComponent)
    {
        elements_.push_back(VertexElement(usage, nComponent));
        vertexSize_ += elements_.back().size();
    }

    const VertexElement & VertexDeclaration::getElement(size_t i) const
    {
        return elements_[i];
    }

    size_t VertexDeclaration::getNumElement() const
    {
        return elements_.size();
    }

    size_t VertexDeclaration::getVertexSize() const
    {
        return vertexSize_;
    }

    void VertexDeclaration::bind()
    {
        s_pActiveDecl = this;
    }

    void VertexDeclaration::unbind()
    {
        ASSERT_2(s_pActiveDecl == this, "VertexDeclaration::unbind - invalid operation!");
        s_pActiveDecl = nullptr;
    }

    void VertexDeclaration::merge(VertexDeclaration * p)
    {
        ASSERT_1(p);

        for(size_t i = 0; i < p->getNumElement(); ++i)
        {
            this->addElement(p->getElement(i));
        }
    }

    ////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(VertexDeclMgr);

    VertexDeclMgr::VertexDeclMgr()
    {

    }

    VertexDeclMgr::~VertexDeclMgr()
    {

    }

    void VertexDeclMgr::init()
    {
        //add internal vertex format
        VertexDeclarationPtr decl;

        decl = new VertexDeclaration(VertexXYZ::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        add(decl);

        decl = new VertexDeclaration(VertexXYZColor::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::COLOR, 4);
        add(decl);

        decl = new VertexDeclaration(VertexXYZN::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::NORMAL, 3);
        add(decl);

        decl = new VertexDeclaration(VertexXYZUV::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::TEXCOORD0, 2);
        add(decl);

        decl = new VertexDeclaration(VertexXYZNUV::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::NORMAL, 3);
        decl->addElement(VertexUsage::TEXCOORD0, 2);
        add(decl);

        decl = new VertexDeclaration(VertexXYZColorUV::getType());
        decl->addElement(VertexUsage::POSITION, 3);
        decl->addElement(VertexUsage::COLOR, 4);
        decl->addElement(VertexUsage::TEXCOORD0, 2);
        add(decl);
    }

    void VertexDeclMgr::add(VertexDeclarationPtr decl)
    {
        auto it = decls_.find(decl->getName());
        if (it != decls_.end())
        {
            ORA_LOG_ERROR("the vertex declaration '%s' has been exist!", decl->getName().c_str());
            ASSERT_1(0);
            return;
        }
        decls_.insert(std::make_pair(decl->getName(), decl));
    }

    VertexDeclarationPtr VertexDeclMgr::combine(
	    VertexDeclarationPtr orig, VertexDeclarationPtr extra )
    {
	    std::string newName = orig->getName() + "_" + extra->getName();

	    // see if the new decl has been created before..
	    auto it = decls_.find( newName );

	    // not found...
	    if (it == decls_.end())
	    {
		    // build a new declaration.
		    VertexDeclarationPtr vd = new VertexDeclaration( newName );

            vd->merge(orig.get());
            vd->merge(extra.get());

			decls_[newName] = vd;
			return vd;
	    }

	    return it->second;
    }

    VertexDeclarationPtr VertexDeclMgr::get(const std::string & name)
    {
        //search it in the cache
        auto it = decls_.find(name);
        if(it != decls_.end())
            return it->second;

        //if not found, try load it from formats file.
        VertexDeclarationPtr decl;

        std::string fullname;
        stringFromFormat(fullname, "shaders/formats/%s.xml", name.c_str());
        SectionPtr section = SectionFactory::instance()->loadXml(fullname);
        if(section && section->getFirstChild())
        {
            ORA_LOG_TRACE("Load vertex declaration '%s'", fullname.c_str());

            decl = new VertexDeclaration(name);
            if(!decl->load(section->getFirstChild()))
            {
                ORA_LOG_ERROR("Failed to load vertex declaration '%s'", fullname.c_str());
                decl = nullptr;
            }
        }

        decls_[name] = decl; //cache it
        return decl;
    }

}