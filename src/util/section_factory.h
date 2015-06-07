//
//  section_factory.h
//  orange
//
//  Created by zhoubao on 14-6-23.
//
//

#ifndef __orange__section_factory__
#define __orange__section_factory__

#include "section.h"
#include "singleton.h"
#include <map>

namespace ora
{
    
    class SectionFactory :
        public Singleton<SectionFactory>
    {
    public:
        SectionFactory();
        ~SectionFactory();
        
        static SectionPtr load(const std::string & filename, SectionType type);
        static bool save(const std::string & filename, SectionPtr root);
        
        static SectionPtr loadXml(const std::string & filename);
        static SectionPtr loadJson(const std::string & filename);
        static SectionPtr loadDS(const std::string & filename);

		static SectionPtr parseXml(const std::string & content);
		static SectionPtr parseJson(const std::string & content);
		static SectionPtr parseDS(const std::string & content);
        
        static bool saveXml(const std::string & filename, SectionPtr root);
        static bool saveJson(const std::string & filename, SectionPtr root);
        static bool saveDS(const std::string & filename, SectionPtr root);
        
        static void purge(ISection * p);
        
    private:
    
        SectionPtr findWithDefault(const std::string & name, SectionPtr def=nullptr);
        void set(const std::string & name, SectionPtr section);
        void purgeByPtr(ISection * p);
        void purgeByName(const std::string & name);
        
    private:
        typedef std::map<std::string, ISection*> CacheMap;
        CacheMap cache_;
    };

} // end namespace ora

#endif /* defined(__orange__section_factory__) */
