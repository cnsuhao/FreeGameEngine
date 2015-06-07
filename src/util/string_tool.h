//
//  string_tool.h
//  liborange
//
//  Created by zhoubao on 14-4-18.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__string_tool__
#define __liborange__string_tool__

#include <string>
#include <vector>
#include <iostream>

namespace ora
{

    typedef std::vector<std::string> StringVector;

    const std::string EmptyString;


    bool isWhiteSpace(char ch);

    void trimString(std::string & str);
    
    bool stringFromFormat(std::string & out, const char * format, ...);
    bool stringFromVAList(std::string & out, const char * format, va_list va);
    bool stringBeginWith(const std::string & str, const std::string & prefix);
    bool stringEndWith(const std::string & str, const std::string & postfix);

    // 格式化斜线
    void formatPathSlash(std::string & path);

    // 格式化'/'，传回值末尾含'/'
    void formatFilePath(std::string & path);

    // 移除'.'和'..'
    void canonicalizePath(std::string & path);

    // 返回值末尾含'/'
    std::string getFilePath(const std::string & filename);
    std::string getFileName(const std::string & filename);

    // 删除'.'
    std::string removeFileExt(const std::string & name);

    // 返回值包含'.'
    std::string getFileExt(const std::string & name);

    class OutputNChar
    {
        char    ch_;
        int     nRepeat_;

    public:
        OutputNChar(char ch, int nRepeat);

        friend std::ostream & operator<<(std::ostream & stream, const OutputNChar & tab);
    };


    // 输出n个制表符
    class TabProxy
    {
        int n_;

    public:

        explicit TabProxy(int n);

        friend std::ostream & operator<<(std::ostream & stream, const TabProxy & tab);
    };
	
	std::vector<std::string> splitstr(const std::string & str, const std::string & pattern);


}// end namespace ora

#endif /* defined(__liborange__string_tool__) */
