//
//  section_factory.cpp
//  orange
//
//  Created by zhoubao on 14-6-23.
//
//

#include "section_factory.h"
#include "file_tool.h"
#include "log_tool.h"
#include "assert_tool.h"

#include "xml_RapidXml.h"
#include "json_rapidjson.h"
#include "data_stream.h"

namespace ora
{
    IMPLEMENT_SINGLETON(SectionFactory);
    
    SectionFactory::SectionFactory()
    {
        
    }
    
    SectionFactory::~SectionFactory()
    {
        
    }
    
    SectionPtr SectionFactory::findWithDefault(const std::string & name, SectionPtr def)
    {
        CacheMap::iterator it = cache_.find(name);
        if (it != cache_.end())
            return it->second;
        
        cache_[name] = def.get();
        return def;
    }
    
    void SectionFactory::set(const std::string & name, SectionPtr section)
    {
        cache_[name] = section.get();
    }
    
    
    /*static*/ SectionPtr SectionFactory::load(const std::string & filename, SectionType type)
    {
        switch (type)
        {
            case SectionType::Xml:
                return loadXml(filename);
                
            case SectionType::Json:
                return loadJson(filename);
                
            case SectionType::DS:
                return loadDS(filename);
                
            default:
                return nullptr;
        }
    }
    
    /*static*/ bool SectionFactory::save(const std::string & filename, SectionPtr root)
    {
        ASSERT_1(root);
        
        switch(root->getType())
        {
            case SectionType::Xml:
                return saveXml(filename, root);
                
            case SectionType::Json:
                return saveJson(filename, root);
                
            case SectionType::DS:
                return saveDS(filename, root);
                
            default:
                return false;
        };
    }
    
    
    /*static*/ void SectionFactory::purge(ISection * p)
    {
        if(hasInstance())
            instance()->purgeByPtr(p);
    }
    
    void SectionFactory::purgeByPtr(ISection * p)
    {
        CacheMap::iterator it;
        for (it = cache_.begin(); it != cache_.end(); ++it)
        {
            if (p == it->second)
            {
                cache_.erase(it);
                break;
            }
        }
    }
    
    void SectionFactory::purgeByName(const std::string & name)
    {
        CacheMap::iterator it = cache_.find(name);
        if(it != cache_.end())
        {
            cache_.erase(it);
        }
    }
    
	/*static*/ SectionPtr SectionFactory::parseXml(const std::string & content)
	{
		RapidXmlImpl::XmlDocumentTypePtr doc(new RapidXmlImpl::XmlDocumentType());

		RapidXmlImpl::ContentPtr c(new std::string(content));

		const int flag = rapidxml::parse_trim_whitespace;
		doc->parse<flag>(const_cast<char *>(c->c_str()));

		SectionPtr root = new RapidXmlImpl(&*doc, c, doc);

		return root;
	}

	/*static*/ SectionPtr SectionFactory::parseJson(const std::string &content)
	{

		RapidJsonImpl::JsonDocumentTypePtr document(new RapidJsonImpl::JsonDocumentType);

		RapidJsonImpl::JsonNodeTypePtr node(new RapidJsonImpl::JsonNodeType);
		if (document->Parse<0>(content.c_str()).HasParseError())
		{
			ORA_LOG_ERROR("Json parse error %s.", content.c_str());
			return NULL;
		}

		node->value = dynamic_cast<rapidjson::Value &> (*document);
		SectionPtr root = new RapidJsonImpl(&*node, node, document);

		return root;
	}

	/*static*/ SectionPtr SectionFactory::parseDS(const std::string & content)
	{
		DataStreamPtr root = new DataStream("root");
		root->scan(content);
		return root;
	}

    /*static*/ SectionPtr SectionFactory::loadXml(const std::string & filename)
    {
        SectionPtr root = instance()->findWithDefault(filename);
        if(root) return root;
        
        RapidXmlImpl::ContentPtr content(new std::string());
        if (!readFile(*content, filename, true))
            return nullptr;
        
        RapidXmlImpl::XmlDocumentTypePtr doc(new RapidXmlImpl::XmlDocumentType());
        
        const int flag = rapidxml::parse_trim_whitespace;
        doc->parse<flag>(const_cast<char *>(content->c_str()));
        
        root = new RapidXmlImpl(&*doc, content, doc);
        
        instance()->set(filename, root);
        return root;
    }
    
    /*static*/ bool SectionFactory::saveXml(const std::string & filename, SectionPtr root)
    {
        instance()->set(filename, root);
        
        RapidXmlImpl *pRoot = root.cast<RapidXmlImpl>();
        
        std::ostringstream ss;
        pRoot->print(ss);
        
        return writeFile(ss.str(), filename, true);
    }
    
    
    /*static*/ SectionPtr SectionFactory::loadJson(const std::string &filename)
    {
        SectionPtr root = instance()->findWithDefault(filename);
        if(root) return root;
        
        std::string content;
        if (!readFile(content, filename, true))
            return NULL;
        
        RapidJsonImpl::JsonDocumentTypePtr document(new RapidJsonImpl::JsonDocumentType);
        
        RapidJsonImpl::JsonNodeTypePtr node(new RapidJsonImpl::JsonNodeType);
        if (document->Parse<0>(content.c_str()).HasParseError())
        {
            ORA_LOG_ERROR("Json parse error %s.", filename.c_str());
            return NULL;
        }
        
        node->value = dynamic_cast<rapidjson::Value &> (*document);
        root = new RapidJsonImpl(&*node, node, document);
        
        instance()->set(filename, root);
        return root;
    }
    
    /*static*/ bool SectionFactory::saveJson(const std::string &filename, SectionPtr root)
    {
        instance()->set(filename, root);
        
        RapidJsonImpl * pRoot = root.cast<RapidJsonImpl>();
        
        std::ostringstream ss;
        pRoot->print(ss);
        
        return writeFile(ss.str(), filename, true);
    }
    
    
    /*static*/ SectionPtr SectionFactory::loadDS(const std::string & filename)
    {
        SectionPtr root = instance()->findWithDefault(filename);
        if(root) return root;
        
        DataStreamPtr realRoot = new DataStream("root");
        if(!realRoot->load(filename))
            return nullptr;
        
        instance()->set(filename, realRoot);
        return realRoot;
    }
    
    /*static*/ bool SectionFactory::saveDS(const std::string & filename, SectionPtr root)
    {
        instance()->set(filename, root);
        
        DataStream *pRoot = root.cast<DataStream>();
        return pRoot->save(filename);
    }
    
}