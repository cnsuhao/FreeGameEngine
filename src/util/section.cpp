
#include "section.h"
#include "section_factory.h"

#include <cstdio>
#include <cstdarg>

namespace ora
{
    ISection::ISection()
    {
    }

    ISection::~ISection()
    {
        SectionFactory::purge(this);
    }

    bool ISection::reads(const std::string & tag, SectionPtrVector & vector)
    {
        vector.clear();

        std::string tailTag = tag;
        SectionPtr ptr;
        
        size_t pos = tag.find_last_of('/');
        if(pos != tag.npos)
        {
            ptr = read(tag.substr(0, pos));
            if(!ptr)
                return false;
            
            tailTag = tag.substr(pos+1); 
        }
        else
        {
            ptr = this;
        }
        
        SectionPtrVector children;
        ptr->getChildren(children);
        getChildren(children);
        
        for(SectionPtr child : children)
        {
            if(child->getTag() == tag)
                vector.push_back(child);
        }
        
        return !vector.empty();
    }
    
}// end namespace ora
