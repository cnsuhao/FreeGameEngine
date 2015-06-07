#ifndef H_DATA_STREAM_H
#define H_DATA_STREAM_H


#include "quick_file_reader.h"
#include "section.h"

namespace ora
{
    typedef SmartPtr<class DataStream> DataStreamPtr;


    std::string valueStringFormat(const std::string & input);
    std::string valueStringUnformat(const std::string & input);


    class DataStream : public ISection
    {
    public:
        typedef std::vector<DataStreamPtr> Children;

        DataStream(void);
        explicit DataStream(const std::string & tag);
        DataStream(const std::string & tag, const std::string & value);
        ~DataStream(void);
        
        virtual SectionType getType() const{ return SectionType::DS; }
        virtual bool valid() const { return true; }
        
        virtual const char* ctag() const { return tag_.c_str(); }
        virtual std::string getTag() const { return tag_; }
        virtual void setTag(const std::string &tag){ tag_ = tag; }
        
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


    public:
        DataStreamPtr newDSChild(const std::string & tag);

        void addChild(DataStreamPtr child);
        void clearChildren();

        DataStreamPtr getChild(const std::string & tag, bool createIfMiss);

        Children::iterator begin() { return children_.begin(); }
        Children::iterator end() { return children_.end(); }

        void print(std::ostream & stream);
        void print(std::string & stream);
        void print(std::ostream & stream, int depth);

        bool scan(const std::string & stream);
        bool scan(QuickFileReader & reader);
        
        bool load(const std::string & filename);
        bool save(const std::string & filename);

    private:

        std::string tag_;
        std::string value_;
        Children children_;
    };

}//end namepace ora


#endif // H_DATA_STREAM_H

