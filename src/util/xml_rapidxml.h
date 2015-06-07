
#ifndef IMUST_OJ_XML_RAPIDXML_H
#define IMUST_OJ_XML_RAPIDXML_H

#include <memory>

#include "section.h"

#define RAPIDXML_NO_EXCEPTIONS

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

namespace ora
{

    class RapidXmlImpl : public ISection
    {
    public:

        typedef rapidxml::xml_document<char>    XmlDocumentType;
        typedef rapidxml::xml_node<char>        XmlNodeType;
        typedef rapidxml::xml_attribute<char>   XmlAttributeType;

        typedef std::shared_ptr<XmlDocumentType>    XmlDocumentTypePtr;
        typedef std::shared_ptr<std::string>        ContentPtr;

    public:
        explicit RapidXmlImpl(const std::string & tag, const std::string & value = "");
        RapidXmlImpl(XmlNodeType* node, ContentPtr content, XmlDocumentTypePtr doc);
        ~RapidXmlImpl();
        
        virtual SectionType getType() const{ return SectionType::Xml; }

        virtual bool valid() const { return doc_ && pNode_; }
        virtual const char* ctag() const;
        virtual const char * cvalue() const;
        
        virtual std::string getTag() const { return ctag(); }
        virtual void setTag(const std::string &tag);

    public:
        virtual void setString( const std::string & value );
        virtual void setInt(int value);
        virtual void setUint(uint32 value);
        virtual void setFloat(float value);
        virtual void setDouble(double value);
        virtual void setBool(bool value);
        
        virtual std::string getString() const;
        virtual int         getInt() const;
        virtual uint32      getUint() const;
        virtual float       getFloat() const;
        virtual double      getDouble() const;
        virtual bool        getBool() const;
        
        virtual SectionPtr read(const std::string &tag) const;
        virtual SectionPtr write(const std::string &tag);
        virtual SectionPtr getFirstChild() const;
        virtual SectionPtr newChild(const std::string & tag);
        virtual void getChildren(SectionPtrVector & children);
        virtual void clearChildren();
        virtual void print(std::ostream & stream);

    private:
        SectionPtr makeNode(XmlNodeType* pNode) const;

        XmlNodeType * findFirstNodeRecursive(const char * pTag) const;
        XmlNodeType * addFirstNodeRecursive(const char * pTag);

    private:
        ContentPtr          content_;
        XmlDocumentTypePtr  doc_;
        XmlNodeType*        pNode_;

        friend class SectionFactory;
    };
    
}// end namespace ora

#endif//  IMUST_OJ_XML_RAPIDXML_H
