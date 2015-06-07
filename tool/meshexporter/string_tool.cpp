#include "pch.hpp"
#include "string_tool.h"

#define FORMAT_BUFFER_SIZE 2048

void formatStringVS(std::string & out, const char * pFormat, va_list args)
{
    out.resize(FORMAT_BUFFER_SIZE);
    int n = vsnprintf(&out[0], FORMAT_BUFFER_SIZE, pFormat, args);
    if(n < 0) n = 0;

    out.erase(n);
}

void formatString(std::string & out, const char * pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);

    formatStringVS(out, pFormat, args);
    
    va_end(args);
}

void debugString(const char * pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);

    std::string out;
    formatStringVS(out, pFormat, args);

    va_end(args);

    OutputDebugStringA(out.c_str());
}

void printAssert(const char * expstr, const char * desc, const char * file, int lineno)
{
    std::ostringstream ss;
    ss << "Assert Failed: \nexpression = " << expstr << "\n";
    if(desc) ss << "desc = " << desc << "\n";
    ss << "file = " << file << "\nlineno:" << lineno;

    throw(std::runtime_error("assert failed!"));
}

bool bw_wtoutf8( const wchar_t * wsrc, std::string& output )
{
    int len = WideCharToMultiByte( CP_UTF8, 0, wsrc, -1, NULL, 0, NULL, NULL );
    if (len <= 0)
    {
        return false;
    }
    else
    {
        output.resize( len );
        int res = WideCharToMultiByte( CP_UTF8, 0, wsrc, -1, &output[0], len, NULL, NULL );
        output.resize( output.length() - 1 );
        return res != 0;
    }
}

bool bw_acptoutf8( const char * src, std::string& output )
{
    int len = MultiByteToWideChar( CP_ACP, 0, src, -1, NULL, 0 );
    if (len <= 0)
    {
        return false;
    }
    else
    {
        // MultiByteToWideChar will process the \0 at the end of the source,
        //  so len will contain that too, meaning that the output size WILL
        //  include the \0 at the end, which breaks string concatenation,
        //  since they will be put AFTER the \0. So, after processing the string
        //  we remove the \0 of the output.

        std::wstring temp;
        temp.resize( len );
        int res = MultiByteToWideChar( CP_ACP, 0, src, -1, &temp[0], len );
        temp.resize( temp.length() - 1 );
        return bw_wtoutf8( temp.c_str(), output );
    }
}

void formatFileName(std::string & filename)
{
    if(filename.empty())
        return;

    std::replace(filename.begin(), filename.end(), '\\', '/');

    if(filename.back() == '/')
        filename.pop_back();
}

void formatPathName(std::string & pathname)
{
    if(pathname.empty())
        return;

    std::replace(pathname.begin(), pathname.end(), '\\', '/');

    if(pathname.back() != '/')
        pathname.push_back('/');
}

std::string getFilePath(const std::string & filename)
{
    if(filename.empty())
        return filename;

    std::string temp = filename;
    formatFileName(temp);

    size_t pos = temp.find_last_of('/');
    if(pos == std::string::npos)
        return "";

    temp.erase(pos+1);
    return temp;
}

std::string getFileName(const std::string & filename)
{
    if(filename.empty())
        return filename;

    std::string temp = filename;
    formatFileName(temp);

    size_t pos = temp.find_last_of('/');
    if(pos == std::string::npos)
        return "";

    return temp.substr(pos+1);
}

void removeFileExtension(std::string & filename)
{
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos)
    {
        filename.erase(pos);
    }
}

//字符串是否以后缀结束
bool endsWith( const std::string& s, const std::string& postfix )
{
    if (s.length() < postfix.length()) 
        return false;

    return (s.substr(s.length() - postfix.length()) == postfix);
}

bool fileExists(const std::string & filename)
{
    return GetFileAttributesA(filename.c_str()) != -1;
}

std::string getModuleFileName(HINSTANCE hInstance)
{
    char buffer[1024];
    DWORD n = ::GetModuleFileNameA(hInstance, buffer, 1024);
    buffer[n] = 0;

    return buffer;
}

std::string getModuleFilePath(HINSTANCE hInstance)
{
    std::string filename = getModuleFileName(hInstance);
    return getFilePath(filename);
}
