
#include "pch.hpp"
#include "file_stream.h"
#include "string_tool.h"

class TabProxy
{
    int n_;

public:

    explicit TabProxy(int n)
        : n_(n)
    { }

    friend std::ostream & operator<<(std::ostream & stream, const TabProxy & tab);
};

std::ostream & operator << (std::ostream & stream, const TabProxy & tab)
{
    int n = tab.n_;
    while(n-- > 0) stream << '\t';

    return stream;
}

bool readFromFile(const std::string & filename, std::string & output, bool binary)
{
    FILE *pFile = fopen(filename.c_str(), binary ? "rb" : "r");
    if(!pFile) return false;

    fseek(pFile, 0, SEEK_END);
    long flen = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    output.resize(flen, '\0');
    bool ret = (fread(&output[0], flen, 1, pFile) != 1);
    
    fclose(pFile);
    return ret;
}

bool writeToFile(const std::string & filename, const std::string & input, bool binary)
{
    FILE *pFile = fopen(filename.c_str(), binary ? "wb" : "w");
    if(!pFile) return false;

    bool ret = fwrite(input.c_str(), input.length(), 1, pFile) == 1;
    fclose(pFile);

    return ret;
}


void trimString(std::string & str)
{
    if(str.empty()) return;

    int length = int(str.size());

    char ch;
    int p1 = 0;
    for(; p1 < length; ++p1)
    {
        ch = str[p1];
        if(ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' && ch != '\0')
        {
            break;
        }
    }

    int p2 = length - 1;
    for(; p2 >= 0; --p2)
    {
        ch = str[p2];
        if(ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' && ch != '\0')
        {
            break;
        }
    }

    if(p1 > p2)
    {
        str.clear();
        return;
    }

    int n = p2 - p1 + 1;
    if(p1 != 0)
    {
        for(int i = 0; i<n; ++i)
        {
            str[i] = str[p1 + i];
        }
    }

    str.erase(n, str.npos);
}

bool ignoreAllSpace(QuickFileReader & reader)
{
    char ch;
    do
    {
        ch = reader.get();
        if(!reader.good())  return false;
    }while(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\0');

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
    }while(ch != '\n');
    return true;
}

char translateChar(char ch)
{
    switch(ch)
    {
    case 'n': return '\n';
    case '0': return '\0';
    default: return ch;
    };
}

std::string valueStringFormat(const std::string & input)
{
    //转义符
    static const std::string StrNeedFormat("={}#\n\\");

    char ch;
    std::string  output;
    size_t len = input.length();
    for(size_t i = 0; i<len; ++i)
    {
        ch = input[i];
        if(ch == '\0' || StrNeedFormat.find(ch) != StrNeedFormat.npos)
        {
            output += '\\';
            switch(ch)
            {
            case '\n': ch = 'n'; break;
            case '\0': ch = '0'; break;
            default: break;
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
            if(++i > input.size()) break;

            ch = translateChar(input[i]);
        }

        output += ch;
    }
    return output;
}

//////////////////////////////////////////////////////////////////////////
typedef std::map<std::string, FileSreamPtr> StreamCache;
static StreamCache s_streamCache;

/*static */FileSreamPtr FileSream::openStream(const std::string & filename)
{
    if(filename.empty()) return NULL;

    StreamCache::iterator it = s_streamCache.find(filename);
    if(it != s_streamCache.end()) return it->second;

    FileSreamPtr p;
    std::string content;
    if(readFromFile(filename, content))
    {
        p = new FileSream("root");
        p->scan(content);
    }
    else
    {
        ERROR_MSG("Failed to open faile '%s'", filename.c_str());
    }

    s_streamCache[filename] = p;
    return p;
}

/*static*/ bool FileSream::saveStream(FileSreamPtr root, const std::string & filename)
{
    assert(root);

    s_streamCache[filename] = root;

    std::string stream;
    root->print(stream);
    return writeToFile(filename, stream);
}


FileSream::FileSream(void)
{
}


FileSream::~FileSream(void)
{
}

FileSream::FileSream(const std::string & tag)
    : tag_(tag)
{

}

FileSream::FileSream(const std::string & tag, const std::string & value)
    : tag_(tag)
    , value_(value)
{

}

void FileSream::addChild(FileSreamPtr child)
{
    children_.push_back(child);
}

FileSreamPtr FileSream::getChild(const std::string & tag, bool createIfMiss)
{
    for(Children::iterator it = children_.begin();
        it != children_.end(); ++it)
    {
        if((*it)->getTag() == tag) return *it;
    }

    if(createIfMiss)
    {
        return newChild(tag);
    }

    return NULL;
}

FileSreamPtr FileSream::newChild(const std::string & tag, const std::string & value)
{
    FileSreamPtr p = new FileSream(tag, value);
    addChild(p);
    return p;
}

void FileSream::clearChildren()
{
    children_.clear();
}

FileSreamPtr FileSream::write(const std::string & tag, const std::string & value)
{
    FileSreamPtr p = read(tag, true);
    p->setString(value);
    return p;
}

FileSreamPtr FileSream::read(const std::string & tag, bool createIfMiss)
{
    size_t pos = tag.find('/');
    if(pos == tag.npos)
    {
        return getChild(tag, createIfMiss);
    }

    FileSreamPtr child = getChild(tag.substr(0, pos), createIfMiss);
    if(!child) return NULL;
    
    return child->read(tag.substr(pos+1), createIfMiss);
}

void FileSream::print(std::ostream & stream, int depth)
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

void FileSream::print(std::ostream & stream)
{
    for(Children::iterator it = begin(); it != end(); ++it)
    {
        (*it)->print(stream, 0);
    }
}

void FileSream::print(std::string & stream)
{
    std::ostringstream out;
    
    print(out);

    stream = out.str();
}


bool FileSream::scan(const std::string & stream)
{
    QuickFileReader reader(stream.c_str(), stream.length());
    return scan(reader);
}

bool FileSream::scan(QuickFileReader & reader)
{
    std::string ctag;
    std::string cvalue;
    char ch;
    FileSreamPtr lastChild;

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
            lastChild = NULL;
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

            lastChild = newChild(ctag, cvalue);
        }
    }

    return true;
}