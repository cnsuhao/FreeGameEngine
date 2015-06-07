
#include "xml_rapidxml.h"
#include "log_tool.h"
#include "rapidxml/rapidxml_print.hpp"

#include "section_factory.h"
#include <sstream>

namespace rapidxml
{
    void parse_error_handler(const char *what, void *where)
    {
        ORA_LOG_ERROR("rapidxml parse error: %s", what);
    }
}

namespace ora
{

    RapidXmlImpl::RapidXmlImpl(const std::string & tag, const std::string & value)
        : doc_(new XmlDocumentType())
        , pNode_(&*doc_)
    {
        setTag(tag);

        if (!value.empty())
            setString(value);
    }

    RapidXmlImpl::RapidXmlImpl(XmlNodeType* node, ContentPtr content, XmlDocumentTypePtr doc)
        : content_(content)
        , doc_(doc)
        , pNode_(node)
    {
    }

    RapidXmlImpl::~RapidXmlImpl()
    {
    }
    
    
    void RapidXmlImpl::setString(const std::string &value)
    {
        pNode_->value(doc_->allocate_string(value.c_str(), value.length()));
    }
    
    void RapidXmlImpl::setInt(int value)
    {
        std::ostringstream ss;
        ss << value;
        setString(ss.str());
    }
    
    void RapidXmlImpl::setUint(uint32 value)
    {
        std::ostringstream ss;
        ss << value;
        setString(ss.str());
    }
    
    void RapidXmlImpl::setFloat(float value)
    {
        std::ostringstream ss;
        ss << value;
        setString(ss.str());
    }
    
    void RapidXmlImpl::setDouble(double value)
    {
        std::ostringstream ss;
        ss << value;
        setString(ss.str());
    }
    
    void RapidXmlImpl::setBool(bool value)
    {
        setString(value ? stringTrue : stringFalse);
    }
    
    std::string RapidXmlImpl::getString() const
    {
        return pNode_->value();
    }
    
    int RapidXmlImpl::getInt() const
    {
        return atoi(pNode_->value());
    }
    
    uint32 RapidXmlImpl::getUint() const
    {
        return (uint32) atoi(pNode_->value());
    }
    
    float RapidXmlImpl::getFloat() const
    {
        return (float) atof(pNode_->value());
    }
    
    double RapidXmlImpl::getDouble() const
    {
        return atof(pNode_->value());
    }
    
    bool RapidXmlImpl::getBool() const
    {
        if(stringTrue == pNode_->value())
            return true;
        else if(stringFalse == pNode_->value())
            return false;
        
        return atoi(pNode_->value()) != 0;
    }
    
    SectionPtr RapidXmlImpl::read(const std::string &tag) const
    {
        XmlNodeType *p = findFirstNodeRecursive(tag.c_str());
        return makeNode(p);
    }

    SectionPtr RapidXmlImpl::write(const std::string &tag)
    {
        XmlNodeType *p = addFirstNodeRecursive(tag.c_str());
        return makeNode(p);
    }

    const char* RapidXmlImpl::ctag() const
    {
        ASSERT_2(pNode_, "pNode_ is NULL!");
        return pNode_->name();
    }

    const char * RapidXmlImpl::cvalue() const
    {
        ASSERT_2(pNode_, "pNode_ is NULL!");
        return pNode_->value();
    }

    void RapidXmlImpl::setTag(const std::string &tag)
    {
        pNode_->name(doc_->allocate_string(tag.c_str(), tag.length()));
    }

    
    SectionPtr RapidXmlImpl::newChild(const std::string & tag)
    {
        XmlNodeType *pChild = doc_->allocate_node(rapidxml::node_element);
        pChild->name(doc_->allocate_string(tag.c_str(), tag.size()));
        pNode_->append_node(pChild);
        
        return makeNode(pChild);
    }

    SectionPtr RapidXmlImpl::makeNode(XmlNodeType* pNode) const
    {
        if (!pNode) return NULL;

        return SectionPtr(new RapidXmlImpl(pNode, content_, doc_));
    }	

    RapidXmlImpl::XmlNodeType * RapidXmlImpl::findFirstNodeRecursive(const char * pTag) const
    {
        XmlNodeType *pNode = pNode_;
        while (pNode)
        {
            const char * p = strchr(pTag, '/');
            if (p == NULL)
                return pNode->first_node(pTag);

            pNode = pNode->first_node(pTag, p - pTag);
            pTag = p + 1;
        }
        return NULL;
    }

    RapidXmlImpl::XmlNodeType* RapidXmlImpl::addFirstNodeRecursive(const char * pTag)
    {
        XmlNodeType *pNode = pNode_;
        bool isFini = false;
        while (pNode && !isFini)
        {
            size_t n = 0;
            const char * p = strchr(pTag, '/');
            if (p == NULL)
            {
                isFini = true;
                n = strlen(pTag);
            }
            else n = p - pTag;

            XmlNodeType *pChild = pNode->first_node(pTag, n);
            if (!pChild)
            {
                pChild = doc_->allocate_node(rapidxml::node_element);
                pChild->name(doc_->allocate_string(pTag, n));
                pNode->append_node(pChild);
            }

            pNode = pChild;
            pTag += n + 1;
        }
        return pNode;
    }

    void RapidXmlImpl::print(std::ostream & stream)
    {
        rapidxml::print(std::ostream_iterator<char>(stream), *pNode_);
    }

    void RapidXmlImpl::getChildren(SectionPtrVector & children)
    {
        for (XmlNodeType * p = pNode_->first_node(); p; p = p->next_sibling())
        {
            children.push_back(makeNode(p));
        }
    }
    
    void RapidXmlImpl::clearChildren()
    {
        pNode_->remove_all_nodes();
    }

    SectionPtr RapidXmlImpl::getFirstChild() const
    {
        return makeNode(pNode_->first_node());
    }

}// end namespace ora
