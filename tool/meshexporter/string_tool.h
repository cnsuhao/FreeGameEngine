#pragma once


void formatStringVS(std::string & out, const char * pFormat, va_list args);

void formatString(std::string & out, const char * pFormat, ...);

void debugString(const char * pFormat, ...);

void printAssert(const char * expstr, const char * desc, const char * file, int lineno);

#define DEBUG_MSG debugString
#define ERROR_MSG debugString

#define MF_ASSERT(exp, desc) \
    do{\
    if(!(exp)) printAssert(#exp, desc, __FILE__, __LINE__);\
    }while(0)


bool bw_wtoutf8( const wchar_t * wsrc, std::string& output );

bool bw_acptoutf8( const char * src, std::string& output );

void formatFileName(std::string & filename);

void formatPathName(std::string & pathname);

std::string getFilePath(const std::string & filename);

std::string getFileName(const std::string & filename);

void removeFileExtension(std::string & filename);

bool endsWith( const std::string& s, const std::string& postfix );

bool fileExists(const std::string & filename);

std::string getModuleFileName(HINSTANCE hInstance=0);
std::string getModuleFilePath(HINSTANCE hInstance=0);