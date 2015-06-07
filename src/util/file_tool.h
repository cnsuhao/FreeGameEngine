#pragma once

#include "string_tool.h"
#include "singleton.h"
#include "smartptr.h"

namespace ora
{

    bool readFile(std::string & content, const std::string & filename, bool isBinary);
    bool writeFile(const std::string & content, const std::string & filename, bool isBinary);

    class IFile : public IReferenceCount
    {
    public:
    
        enum Mode
        {
            ModeRead   = 1,
            ModeWrite  = 2,
            ModeBinary = 4,
        };
        
        enum Seek
        {
            SeekBeg,
            SeekCur,
            SeekEnd,
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


    class IFileSystem : public IReferenceCount
    {
    public:
        IFileSystem();
        virtual ~IFileSystem();
    
        virtual bool init();
        
        virtual bool isFileExist(const std::string & filename);
        
        /** 如果获取失败，返回空字符串 */
        std::string getFullPath(const std::string & filename);
        std::string getRelativePath(const std::string & fullPath);
        
        //@{ 这里的path，是相对于ModulePath而言的
        void addSearchPath(const std::string & path);
        void setSearchPath(const StringVector & path);
        const StringVector & getSearchPath() const { return _paths; }
        //@}
        
        FilePtr openFile(const std::string & filename, IFile::Mode mode=IFile::ModeRead);
        
    public:
        
        virtual std::string getCurrentPath() = 0;
        
        /** ios & android，返回是res路径，其他平台返回app所在路径 */
        virtual std::string getModulePath() = 0;
        
        virtual std::string getWritablePath() = 0;
        
        //@{ 以下几个方法，参数必须是绝对路径
        
        virtual bool createDir(const std::string & fullPath) = 0;
        virtual bool deleteDir(const std::string & fullPath) = 0;
        
        virtual bool existFile(const std::string & fullPath) = 0;
        virtual bool deleteFile(const std::string & fullPath) = 0;
        virtual bool renameFile(const std::string & fullPath) = 0;

        virtual FilePtr openFileInternal(const std::string & filename, const std::string & mode);
        //@}

        static void canonicalizePath(std::string & path);
        
    protected:
        /** 默认路径。对资源的读取，要以此为参考路径，派生类需要设置此值。*/
        std::string     _defaultPath;
        StringVector    _paths;
    };
    
    typedef SmartPtr<IFileSystem> FileSystemPtr;
    
    
    class FileSystemMgr :
        public Singleton<FileSystemMgr>
    {
    public:
        FileSystemMgr();
        ~FileSystemMgr();
        
        
        //如果文件系统为null，会发生assert（getFileSystem不会）。推荐使用此方法。
        inline FileSystemPtr fileSystem()
        {
            assert(_fileSystem);
            return _fileSystem;
        }
        
        void setFileSystem(FileSystemPtr fileSystem){ _fileSystem = fileSystem; }
        FileSystemPtr getFileSystem(){ return _fileSystem; }
        
    private:
        FileSystemPtr _fileSystem;
    };
    
}// end namespace ora
