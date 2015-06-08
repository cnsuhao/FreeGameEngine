#pragma once

#include "string_tool.h"
#include "singleton.h"
#include "smartptr.h"
#include <cassert>

namespace ora
{

    bool readFile(std::string & content, const std::string & filename, bool isBinary);
    bool writeFile(const std::string & content, const std::string & filename, bool isBinary);

    class IFile : public IReferenceCount
    {
    public:
        enum Mode
        {
            MD_READ   = 1,
            MD_WRITE  = 2,
            MD_BINARY = 4,
            MD_CREATE = 8,
        };
        
        enum Seek
        {
            SK_BEG,
            SK_CUR,
            SK_END,
        };
        
        IFile();
        virtual ~IFile();
        
        virtual size_t length() = 0;
        virtual size_t tell() = 0;
        virtual void   seek(size_t pos, Seek seekMode) = 0;
        
        virtual size_t read(void * buffer, size_t n) = 0;
        virtual size_t write(const void * buffer, size_t n) = 0;
        
        virtual void flush() = 0;
        virtual void close() = 0;
    };
    typedef SmartPtr<IFile> FilePtr;
    
    enum FileAccess
    {
        FA_NONE = 0,
        FA_READ = 1,
        FA_WRITE = 2,
        FA_REMOVE = 4,
        FA_ALL = 0x7f,
    };

    class IFileSystem : public IReferenceCount
    {
    public:
        IFileSystem();
        virtual ~IFileSystem();
 
        virtual bool exist(const std::string & path) = 0;
        virtual bool isFile(const std::string & path) = 0;
        virtual bool isDir(const std::string & path) = 0;
        
        virtual FilePtr openFile(const std::string & path, IFile::Mode mode) = 0;
        virtual FILE* openRawFile(const char* path, const char * mode) = 0;
        
        virtual bool removeFile(const std::string & path) = 0;
        virtual bool renameFile(const std::string & oldpath, const std::string & newpath) = 0;
        
        virtual bool createDir(const std::string & path) = 0;
        virtual bool removeDir(const std::string & path) = 0;
        virtual bool listDir(const std::string & path, StringVector & files) = 0;
        
        FileAccess getAccess() const { return access_; }
        void setAccess(FileAccess ac){ access_ = ac; }
        
    private:
        FileAccess access_;
    };
    
    typedef SmartPtr<IFileSystem> FileSystemPtr;
    FileSystemPtr createFileSystem(const std::string & path);
    
    
    class FileSystemMgr :
        public Singleton<FileSystemMgr>
    {
    public:
        FileSystemMgr();
        ~FileSystemMgr();
        
        void addSearchPath(const std::string & path, bool front=false);
        void addSearchPath(const std::string & path, FileSystemPtr fs, bool front=false);
        
        void setSearchPath(const StringVector & path);
        const StringVector & getSearchPath() const { return searchPaths_; }
        
        bool isFileExist(const std::string & filename);
        FilePtr openFile(const std::string & filename, IFile::Mode mode);
        FILE* openRawFile(const std::string & file, const char * mode);
        
        std::string getFullPath(const std::string & filename);
        std::string getRelativePath(const std::string & fullPath);
        
        bool createDir(const std::string & path);
        bool removeDir(const std::string & path);
        
        bool removeFile(const std::string & path);
        bool renameFile(const std::string & oldpath, const std::string & newpath);
        
        const std::string & getWritablePath() const { return writablePath_; }
        void setWritablePath(const std::string & path);
        
    private:
        std::string                     writablePath_;
        std::vector<std::string>        searchPaths_;
        std::vector<FileSystemPtr>      fileSystems_;
    };
    
    std::string getCurrentPath();
    
    /** ios & android，返回是document路径，其他平台返回app所在路径 */
    std::string getModulePath();
    
}// end namespace ora
