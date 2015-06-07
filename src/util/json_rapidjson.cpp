//
//  json_rapidjson.cpp
//  orange
//
//  Created by Kevin on 14-6-16.
//
//

#include "json_rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"
#include "log_tool.h"

#include "section_factory.h"

namespace ora{

    RapidJsonImpl::RapidJsonImpl(const std::string & tag)
        : doc_(new JsonDocumentType())
    {
        node_ = new JsonNodeType();
        doc_->SetObject();
        node_->value.SetObject();
        setTag(tag);
    }
    
    RapidJsonImpl::RapidJsonImpl(JsonNodeType* node, JsonNodeTypePtr root, JsonDocumentTypePtr doc)
        : root_(root)
        , doc_(doc)
        , node_(node)
    {
    }
    
    RapidJsonImpl::~RapidJsonImpl()
    {
    }
    
    SectionPtr RapidJsonImpl::newChild(const std::string & tag)
    {
        JsonNodeType node;
        node.name.SetString(tag.c_str(), tag.size(), doc_->GetAllocator());
        node_->value.AddMember(node.name, node.value, doc_->GetAllocator());
        JsonNodeType *pNode = node_->value.MemberEnd() - 1;
        
        return makeNode(pNode);
    }
    
    SectionPtr RapidJsonImpl::makeNode(JsonNodeType *node) const
    {
        if ( !node )
            return NULL;
        
        return SectionPtr(new RapidJsonImpl(node, root_, doc_));
    }
    
    SectionPtr RapidJsonImpl::read(const std::string & tag) const
    {
        return makeNode(findFirstNodeRecursive(tag.c_str()));
    }

    SectionPtr RapidJsonImpl::write(const std::string & tag)
    {
        return makeNode(addFirstNodeRecursive(tag.c_str()));
    }
    
    void RapidJsonImpl::setString(const std::string &value)
    {
        node_->value.SetString(value.c_str(), value.length(), doc_->GetAllocator());
    }
    
    void RapidJsonImpl::setInt(int value)
    {
        node_->value.SetInt(value);
    }
    
    void RapidJsonImpl::setUint(uint32 value)
    {
        node_->value.SetUint(value);
    }
    
    void RapidJsonImpl::setFloat(float value)
    {
        node_->value.SetDouble(double(value));
    }
    
    void RapidJsonImpl::setDouble(double value)
    {
        node_->value.SetDouble(value);
    }
    
    void RapidJsonImpl::setBool(bool value)
    {
        node_->value.SetBool(value);
    }
    
    std::string RapidJsonImpl::getString() const
    {
        return node_->value.GetString();
    }
    
    int RapidJsonImpl::getInt() const
    {
        return node_->value.GetInt();
    }
    
    uint32 RapidJsonImpl::getUint() const
    {
        return node_->value.GetUint();
    }
    
    float RapidJsonImpl::getFloat() const
    {
        return (float) node_->value.GetDouble();
    }
    
    double RapidJsonImpl::getDouble() const
    {
        return node_->value.GetDouble();
    }
    
    bool RapidJsonImpl::getBool() const
    {
        return node_->value.GetBool();
    }
    
    
    const char * RapidJsonImpl::ctag() const
    {
        ASSERT_2(node_, "node_ is NULL!");
        return node_->name.GetString();
    }
    
    void RapidJsonImpl::setTag(const std::string &tag)
    {
        node_->name.SetString(tag.c_str(), tag.length(), doc_->GetAllocator());
    }
        
    void RapidJsonImpl::print(std::ostream &stream)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        node_->value.Accept(writer);
        
        stream << buffer.GetString();
    }
    
    void RapidJsonImpl::getChildren(SectionPtrVector &children)
    {
        for (JsonDocumentType::MemberIterator iter = node_->value.MemberBegin();
             iter != node_->value.MemberEnd(); ++iter) {
            children.push_back(makeNode(iter));
        }
    }
    
    void RapidJsonImpl::clearChildren()
    {
        node_->value.Clear();
    }
    
    SectionPtr RapidJsonImpl::getFirstChild() const
    {
        return makeNode(node_->value.MemberBegin());
    }
    
    RapidJsonImpl::JsonNodeType * RapidJsonImpl::findFirstNodeRecursive(const char *tag) const
    {
        if ( node_->value.FindMember(tag) == 0 )
            return nullptr;
        
        return node_->value.FindMember(tag);
    }
    
    RapidJsonImpl::JsonNodeType * RapidJsonImpl::addFirstNodeRecursive(const char *tag) const
    {
        if ( !node_->value.IsObject() )
            node_->value.SetObject();
        
        JsonNodeType * node = node_->value.FindMember(tag);
        if ( node == 0)
        {
            JsonNodeType node;
            node.name.SetString(tag, strlen(tag), doc_->GetAllocator());
            node_->value.AddMember(node.name, node.value, doc_->GetAllocator());
            return node_->value.MemberEnd()-1;
        }
        else
        {
            return node;
        }
    }
    
}; // end of namespace ora