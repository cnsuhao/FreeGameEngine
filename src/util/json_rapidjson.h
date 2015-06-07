//
//  json_rapidjson.h
//  orange
//
//  Created by Kevin on 14-6-16.
//
//

#ifndef __orange__json_rapidjson__
#define __orange__json_rapidjson__

#include "section.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

namespace ora{

    class RapidJsonImpl : public ISection{
    public:
        typedef rapidjson::Document         JsonDocumentType;
        typedef JsonDocumentType::Member    JsonNodeType;
        
        typedef std::shared_ptr<JsonDocumentType>   JsonDocumentTypePtr;
        typedef std::shared_ptr<JsonNodeType>       JsonNodeTypePtr;
        
    public:
        explicit RapidJsonImpl(const std::string & tag);
        RapidJsonImpl(JsonNodeType * node, JsonNodeTypePtr root, JsonDocumentTypePtr doc);
        ~RapidJsonImpl();
        
        virtual SectionType getType() const{ return SectionType::Json; }
        virtual bool valid() const { return !!doc_; };
        
        virtual const char* ctag() const;

        virtual std::string getTag() const { return ctag(); }
        virtual void setTag(const std::string &tag);
    public:
        virtual void setString(const std::string & value);
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
        SectionPtr makeNode(JsonNodeType * node) const;
        JsonNodeType* findFirstNodeRecursive(const char * tag) const;
        JsonNodeType* addFirstNodeRecursive(const char * tag) const;
        
    public:
        JsonNodeTypePtr     root_;
        JsonDocumentTypePtr doc_;
        JsonNodeType*       node_;
    };
    
}; // end of namespace ora

#endif /* defined(__orange__json_rapidjson__) */
