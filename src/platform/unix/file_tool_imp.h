#ifndef FILE_TOOL_IMP_H
#define FILE_TOOL_IMP_H

#include "util/file_tool.h"

namespace ora
{
    class FileSystemImp : public IFileSystem
    {
    public:
        FileSystemImp();
        ~FileSystemImp();
        
        virtual bool exist(const std::string & path) override;
        virtual bool isFile(const std::string & path) override;
        virtual bool isDir(const std::string & path) override;
        
        virtual FilePtr openFile(const std::string & path, IFile::Mode mode) override;
        virtual FILE* openRawFile(const char* path, const char * mode) override;
        
        virtual bool removeFile(const std::string & path) override;
        virtual bool renameFile(const std::string & oldpath, const std::string & newpath) override;
        
        virtual bool createDir(const std::string & path) override;
        virtual bool removeDir(const std::string & path) override;
        virtual bool listDir(const std::string & path, StringVector & files) override;
    };
}

#endif //FILE_TOOL_IMP_H
