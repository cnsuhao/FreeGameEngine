//
//  string_tool.cpp
//  liborange
//
//  Created by zhoubao on 14-4-18.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "string_tool.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>

namespace ora
{

    bool isWhiteSpace(char ch)
    {
        return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\0';
    }

    void trimString(std::string & str)
    {
        if(str.empty()) return;

        int length = int(str.size());

        int p1 = 0;
        for(; p1 < length; ++p1)
        {
            if(!isWhiteSpace(str[p1])) break;
        }

        int p2 = length - 1;
        for(; p2 >= 0; --p2)
        {
            if(!isWhiteSpace(str[p2])) break;
        }

        if(p1 > p2)
        {
            str.clear();
            return;
        }

        int n = p2 - p1 + 1;
        if(p1 != 0)
        {
            for(int i = 0; i < n; ++i)
            {
                str[i] = str[p1 + i];
            }
        }

        str.erase(n, str.npos);
    }

    std::string removeFileExt(const std::string & name)
    {
        size_t pos = name.find_last_of('.');
        if(pos != name.npos)
        {
            return name.substr(0, pos);
        }

        return name;
    }

    std::string getFileExt(const std::string & name)
    {
        size_t pos = name.find_last_of('.');
        if(pos != name.npos)
        {
            return name.substr(pos);
        }

        return "";
    }

    void formatPathSlash(std::string & path)
    {
        std::replace(path.begin(), path.end(), '\\', '/');
    }

    void formatFilePath(std::string & path)
    {
        if(path.empty()) return;

        formatPathSlash(path);
        if(path.back() != '/') path += '/';
    }

    void canonicalizePath(std::string & path)
    {
        formatPathSlash(path);

        std::string ret;
        ret.reserve(path.size());

        std::vector<size_t> dirs;

        char ch;
        bool startWithPath = false;
        for(size_t i = 0; i < path.size(); ++i)
        {
            ch = path[i];

            if(ch == '/')
            {
                if(!startWithPath) // may be "//" or "/./"
                {
                    ret += '/';
                    dirs.push_back(i + 1);
                }
                startWithPath = true;
                continue;
            }
            
            if(startWithPath && ch == '.')
            {
                if(i + 1 >= path.size() || path[i+1] == '/') //'.'
                {
                    //this is a valid path '.'
                    //so we do nothing.
                    continue;
                }
                else if(i + 1 >= path.size() || path[i+1] == '.') //'..'
                {
                    if(i + 2 >= path.size() || path[i + 2] == '/') 
                    {
                        //this is a valid path '..'
                        //so we need jump to parent path.
                        if(dirs.size() > 1)
                        {
                            dirs.pop_back();
                            ret.erase(dirs.back());
                        }
                        i += 2;
                        continue;
                    }
                }
            }

            ret += ch;
            startWithPath = false;
        }

        path = ret;
    }

    std::string getFilePath(const std::string & filename)
    {
        if(filename.empty())
            return std::string();

        std::string name = filename;
        formatPathSlash(name);
        if(name.back() == '/')
            name.pop_back();

        size_t pos = name.find_last_of('/');
        if(pos == std::string::npos)
            return std::string();

        return name.substr(0, pos + 1);
    }

    std::string getFileName(const std::string & filename)
    {
        if(filename.empty())
            return std::string();

        std::string name = filename;
        formatPathSlash(name);
        if(name.back() == '/')
            name.pop_back();

        size_t pos = name.find_last_of('/');
        if(pos == std::string::npos)
            return name;

        return name.substr(pos + 1);
    }
    
    bool stringFromFormat(std::string & out, const char * format, ...)
    {
        assert(format != nullptr);
        
        va_list pArgList;
        va_start (pArgList, format);
        
        bool ret = stringFromVAList(out, format, pArgList);
        
        va_end(pArgList);
        
        return ret;
    }
    
    bool stringFromVAList(std::string & out, const char * format, va_list va)
    {
        int len = 128;
        int ret = -1;
        
        while(ret < 0)
        {
            len <<= 1;
            
            out.resize(len);
            ret = vsnprintf(&out[0], len, format, va);
            
            //mac上，如果长度不够长，会返回需要的长度
            if(ret > len)
            {
                len = (ret >> 1) + 1;
                ret = -1;
            }
            
            assert(len < 65535 && "stringFromVAList");
        }
        
        out.erase(ret);
        return true;
    }
    
    bool stringBeginWith(const std::string & str, const std::string & prefix)
    {
        if(str.length() < prefix.length()) return false;
        
        for(size_t i = 0; i < prefix.length(); ++i)
        {
            if(str[i] != prefix[i]) return false;
        }
        
        return true;
    }
    
    bool stringEndWith(const std::string & str, const std::string & postfix)
    {
        if(str.length() < postfix.length()) return false;
        
        size_t offset = str.length() - postfix.length();
        for(size_t i = 0; i < postfix.length(); ++i)
        {
            if(str[i + offset] != postfix[i]) return false;
        }
        
        return true;
    }



    OutputNChar::OutputNChar(char ch, int nRepeat)
        : ch_(ch)
        , nRepeat_(nRepeat)
    {
    }

    std::ostream & operator<<(std::ostream & stream, const OutputNChar & output)
    {
        int n = output.nRepeat_;
        while(n-- > 0) stream << output.ch_;

        return stream;
    }


    TabProxy::TabProxy(int n)
        : n_(n)
    {}


    std::ostream & operator << (std::ostream & stream, const TabProxy & tab)
    {
        int n = tab.n_;
        while(n-- > 0) stream << '\t';

        return stream;
    }

	std::vector<std::string> splitstr(const std::string & str, const std::string & pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		std::string src(str+pattern);

		for ( size_t i = 0; i < src.size(); ++i )
		{
			pos = src.find(pattern, i);
			if ( pos < src.size() )
			{
				result.push_back(src.substr(i, pos - i));
				i = pos + pattern.size() - 1;
			}
		}

		return result;
	}
}