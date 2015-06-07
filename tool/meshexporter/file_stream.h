#pragma once


#include "smartpointer.hpp"
#include "quick_file_reader.h"

typedef SmartPointer<class FileSream> FileSreamPtr;

bool readFromFile(const std::string & filename, std::string & output, bool binary=false);
bool writeToFile(const std::string & filename, const std::string & input, bool binary=false);

class FileSream : public ReferenceCount
{
public:
    typedef std::vector<FileSreamPtr> Children;

    static FileSreamPtr openStream(const std::string & filename);
    static bool saveStream(FileSreamPtr root, const std::string & filename);

    FileSream(void);
    explicit FileSream(const std::string & tag);
    FileSream(const std::string & tag, const std::string & value);
    ~FileSream(void);

    const std::string & getTag() const { return tag_; }
    void setTag(const std::string & tag){ tag_ = tag; }

    void setString(const std::string & v){ value_ = v;}
    const std::string & getString() const { return value_; }

    void addChild(FileSreamPtr child);
    FileSreamPtr getChild(const std::string & tag, bool createIfMiss=false);
    virtual FileSreamPtr newChild(const std::string & tag, const std::string & value="");
    void clearChildren();

    FileSreamPtr write(const std::string & tag, const std::string & value);
    FileSreamPtr read(const std::string & tag, bool createIfMiss=false);

    template<typename T>
    void operator << (const T & t);

    template<typename T>
    void operator >> (T & t);

    template<typename T>
    void writeV(const std::string & tag, const T & v);

    template<typename T>
    bool readV(const std::string & tag, T & v, const T & defV = T());

    Children::iterator begin(){ return children_.begin(); }
    Children::iterator end(){ return children_.end(); }

    void print(std::ostream & stream);
    void print(std::string & stream);

    bool scan(const std::string & stream);
    bool scan(QuickFileReader & reader);

private:

    void print(std::ostream & stream, int depth);

    std::string tag_;
    std::string value_;
    Children children_;
};

template<typename T>
void FileSream::operator << (const T & t)
{
    std::ostringstream ss;
    ss << t;
    value_ = ss.str();
}

template<typename T>
void FileSream::operator >> (T & t)
{
    std::istringstream ss(value_);
    ss >> t;
}

template<typename T>
void FileSream::writeV(const std::string & tag, const T & v)
{
    FileSreamPtr p = read(tag, true);
    (*p) << v;
}

template<typename T>
bool FileSream::readV(const std::string & tag, T & v, const T & defV)
{
    FileSreamPtr p = read(tag, false);

    if(p)
    {
        (*p) >> v;
        return true;
    }
    else
    {   
        v = defV;
        return false;
    }
}