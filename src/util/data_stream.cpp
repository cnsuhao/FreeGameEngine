
#include "data_stream.h"
#include "assert_tool.h"
#include "file_tool.h"
#include "section_factory.h"

namespace ora
{


    bool ignoreAllSpace(QuickFileReader & reader)
    {
        char ch;
        do
        {
            ch = reader.get();
            if(!reader.good())  return false;
        }
        while(isWhiteSpace(ch));

        reader.offset(-1);
        return true;
    }

    bool ignoreComment(QuickFileReader & reader)
    {
        char ch;
        do
        {
            ch = reader.get();
            if(!reader.good()) return false;
        }
        while(ch != '\n');
        return true;
    }

    char translateChar(char ch)
    {
        switch(ch)
        {
        case 'n':
            return '\n';
        case '0':
            return '\0';
        default:
            return ch;
        };
    }

    std::string valueStringFormat(const std::string & input)
    {
        static const std::string StrNeedFormat("={}#\n\\");

        char ch;
        std::string  output;
        size_t len = input.length();
        for(size_t i = 0; i < len; ++i)
        {
            ch = input[i];
            if(ch == '\0' || StrNeedFormat.find(ch) != StrNeedFormat.npos)
            {
                output += '\\';
                switch(ch)
                {
                case '\n':
                    ch = 'n';
                    break;
                case '\0':
                    ch = '0';
                    break;
                default:
                    break;
                };
            }

            output += ch;
        }

        return output;
    }

    std::string valueStringUnformat(const std::string & input)
    {
        char ch;
        std::string output;
        for(size_t i = 0; i < input.size(); ++i)
        {
            ch = input[i];
            if(ch == '\\')
            {
                if(++i >= input.size()) break;

                ch = translateChar(input[i]);
            }

            output += ch;
        }
        return output;
    }

    //////////////////////////////////////////////////////////////////////////


    DataStream::DataStream(void)
    {
    }

    DataStream::~DataStream(void)
    {
    }

    DataStream::DataStream(const std::string & tag)
        : tag_(tag)
    {

    }

    DataStream::DataStream(const std::string & tag, const std::string & value)
        : tag_(tag)
        , value_(value)
    {

    }
    
    
    void DataStream::setString(const std::string &value)
    {
        value_ = value;
    }
    
    void DataStream::setInt(int value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::setUint(uint32 value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::setFloat(float value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::setDouble(double value)
    {
        std::ostringstream ss;
        ss << value;
        value_ = ss.str();
    }
    
    void DataStream::setBool(bool value)
    {
        value_ = value ? stringTrue : stringFalse;
    }
    
    std::string DataStream::getString() const
    {
        return value_;
    }
    
    int DataStream::getInt() const
    {
        return atoi(value_.c_str());
    }
    
    uint32 DataStream::getUint() const
    {
        return (uint32) atoi(value_.c_str());
    }
    
    float DataStream::getFloat() const
    {
        return (float) atof(value_.c_str());
    }
    
    double DataStream::getDouble() const
    {
        return atof(value_.c_str());
    }
    
    bool DataStream::getBool() const
    {
        if(stringTrue == value_)
            return true;
        else if(stringFalse == value_)
            return false;
        
        return atoi(value_.c_str()) != 0;
    }
    
    
    SectionPtr DataStream::getFirstChild() const
    {
        if(children_.empty()) return nullptr;
        
        return children_[0].get();
    }
    
    void DataStream::getChildren(SectionPtrVector & children)
    {
        children.resize(children_.size());
        std::copy_n(children_.begin(), children_.size(), children.begin());
    }

    void DataStream::addChild(DataStreamPtr child)
    {
        children_.push_back(child);
    }

    DataStreamPtr DataStream::getChild(const std::string & tag, bool createIfMiss)
    {
        for(Children::iterator it = children_.begin();
                it != children_.end(); ++it)
        {
            if((*it)->getTag() == tag) return *it;
        }

        if(createIfMiss)
            return newDSChild(tag);

        return NULL;
    }

    SectionPtr DataStream::newChild(const std::string & tag)
    {
        return newDSChild(tag);
    }

    DataStreamPtr DataStream::newDSChild(const std::string & tag)
    {
        DataStreamPtr p = new DataStream(tag);
        addChild(p);
        return p;
    }

    void DataStream::clearChildren()
    {
        children_.clear();
    }

    SectionPtr DataStream::write(const std::string & tag)
    {
        size_t pos = tag.find('/');
        if(pos == tag.npos)
        {
            return getChild(tag, true);
        }
        
        DataStreamPtr child = getChild(tag.substr(0, pos), true);
        if(!child) return NULL;
        
        return child->write(tag.substr(pos + 1));
    }

    SectionPtr DataStream::read(const std::string & tag) const
    {
        DataStream *pThis = const_cast<DataStream*>(this);
        
        size_t pos = tag.find('/');
        if(pos == tag.npos)
        {
            return pThis->getChild(tag, false);
        }

        DataStreamPtr child = pThis->getChild(tag.substr(0, pos), false);
        if(!child) return NULL;

        return child->read(tag.substr(pos + 1));
    }

    void DataStream::print(std::ostream & stream, int depth)
    {
        stream << TabProxy(depth) << valueStringFormat(tag_);
        if(!value_.empty())
        {
            stream  << " = " << valueStringFormat(value_);
        }
        stream  << '\n';

        if(!children_.empty())
        {
            stream << TabProxy(depth) << "{\n";

            for(Children::iterator it = begin(); it != end(); ++it)
            {
                (*it)->print(stream, depth + 1);
            }

            stream << TabProxy(depth) << "}\n";
        }
    }

    void DataStream::print(std::ostream & stream)
    {
        for(Children::iterator it = begin(); it != end(); ++it)
        {
            (*it)->print(stream, 0);
        }
    }

    void DataStream::print(std::string & stream)
    {
        std::ostringstream out;

        print(out);

        stream = out.str();
    }

    bool DataStream::load(const std::string & filename)
    {
        clearChildren();
        std::string content;

        if (!readFile(content, filename, false)) return false;

        return scan(content);
    }
    
    bool DataStream::save(const std::string & filename)
    {
        std::ostringstream ss;
        print(ss);
        
        return writeFile(ss.str(), filename, false);
    }

    bool DataStream::scan(const std::string & stream)
    {
        QuickFileReader reader(stream.c_str(), stream.length());
        return scan(reader);
    }

    bool DataStream::scan(QuickFileReader & reader)
    {
        std::string ctag;
        std::string cvalue;
        char ch;
        DataStreamPtr lastChild;

        while(reader.good())
        {
            if(!ignoreAllSpace(reader)) break;

            ch = reader.get();
            if(ch == '}') //一次解析完成
            {
                break;
            }
            else if(ch == '{')//新的节点
            {
                if(!lastChild)//无效的节点
                {
                    return false;
                }

                lastChild->scan(reader);
                lastChild = nullptr;
            }
            else if(ch == '#')
            {
                ignoreComment(reader);
            }
            else
            {
                reader.offset(-1);
                ctag.clear();
                cvalue.clear();

                //解析tag
                while(reader.good())
                {
                    ch = reader.get();
                    if(ch == '\\')
                    {
                        ch = translateChar(reader.get());
                    }
                    else if(ch == '=')
                    {
                        break;
                    }
                    else if(ch == '{' || ch == '}' || ch == '#' || ch == '\n')
                    {
                        reader.offset(-1);
                        break;
                    }

                    ctag += ch;
                }

                trimString(ctag);
                if(ctag.empty()) return false;

                //解析value
                while(reader.good())
                {
                    ch = reader.get();
                    if(ch == '\\')
                    {
                        ch = translateChar(reader.get());
                    }
                    else if(ch == '{' || ch == '}' || ch == '#' || ch == '\n')
                    {
                        reader.offset(-1);
                        break;
                    }

                    cvalue += ch;
                }

                trimString(cvalue);

                lastChild = newDSChild(ctag);
                lastChild->setString(cvalue);
            }
        }

        return true;
    }

}//end namespace ora
