#ifndef IMUST_OJ_XML_H
#define	IMUST_OJ_XML_H

#include "smartptr.h"

namespace ora
{
  
#define SECTION_RW_DECLARATION(METHOD_NAME, VALUE_TYPE)                         \
void write##METHOD_NAME(const std::string & tag, const VALUE_TYPE & value)      \
{                                                                               \
    write(tag)->set##METHOD_NAME(value);                                        \
}                                                                               \
VALUE_TYPE read##METHOD_NAME(const std::string & tag,                           \
    const VALUE_TYPE & defaultV=VALUE_TYPE()) const                             \
{                                                                               \
    SectionPtr child = read(tag);                                               \
    if(!child) return defaultV;                                                 \
    return child->get##METHOD_NAME();                                           \
}
    
    enum class SectionType
    {
        Xml,
        Json,
        DS,
    };
    
    const std::string stringTrue("true");
    const std::string stringFalse("false");
    
    typedef SmartPtr<class ISection> SectionPtr;
    typedef std::vector<SectionPtr> SectionPtrVector;
    
    //extern from core/engine.cpp
    extern SectionPtr g_sysConfig;
    
    class ISection : public IReferenceCount
    {
    public:
        ISection();
        virtual ~ISection();

        virtual bool valid() const = 0;
        virtual SectionType getType() const = 0;

        virtual const char* ctag() const = 0;
        virtual std::string getTag() const = 0;
        virtual void setTag(const std::string &tag) = 0;

    public:
        virtual void setString(const std::string & value) = 0;
        virtual void setInt(int value) = 0;
        virtual void setUint(uint32 value) = 0;
        virtual void setFloat(float value) = 0;
        virtual void setDouble(double value) = 0;
        virtual void setBool(bool value) = 0;
        
        virtual std::string getString() const = 0;
        virtual int         getInt() const = 0;
        virtual uint32      getUint() const = 0;
        virtual float       getFloat() const = 0;
        virtual double      getDouble() const = 0;
        virtual bool        getBool() const = 0;
        
        virtual SectionPtr read(const std::string &tag) const = 0;
        virtual SectionPtr write(const std::string &tag) = 0;
        
        virtual SectionPtr newChild(const std::string & tag) = 0;
        virtual SectionPtr getFirstChild() const = 0;
        
        virtual void getChildren(SectionPtrVector & children) = 0;
        virtual void clearChildren() = 0;
        virtual void print(std::ostream & stream) = 0;

    public:
        
        bool reads(const std::string & tag, SectionPtrVector & vector);
        
        SECTION_RW_DECLARATION(String, std::string)
        SECTION_RW_DECLARATION(Int, int)
        SECTION_RW_DECLARATION(Uint, uint32)
        SECTION_RW_DECLARATION(Float, float)
        SECTION_RW_DECLARATION(Double, double)
        SECTION_RW_DECLARATION(Bool, bool)
    };

}// end namespace ora

#endif  // IMUST_OJ_XML_H
