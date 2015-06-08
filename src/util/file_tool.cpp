#include "file_tool.h"
#include "log_tool.h"
#include "file_imp.h"

namespace ora
{

    bool readFile(std::string & content, const std::string & filename, bool isBinary)
    {
        IFile::Mode mode = IFile::Mode(IFile::MD_READ | (isBinary ? IFile::MD_BINARY : 0));
        FilePtr file = FileSystemMgr::instance()->openFile(filename, mode);
        if (!file)
        {
            ORA_LOG_ERROR("Failed to open file '%s'", filename.c_str());
            return false;
        }

        content.resize(file->length());
        if (!content.empty())
        {
            file->read(&content[0], content.size());
        }
        
        return true;
    }

    bool writeFile(const std::string & content, const std::string & filename, bool isBinary)
    {
        IFile::Mode mode = IFile::Mode(IFile::MD_CREATE | (isBinary ? IFile::MD_BINARY : 0));
        FilePtr file = FileSystemMgr::instance()->openFile(filename, mode);
        if (!file)
        {
            ORA_LOG_ERROR("Failed to create file '%s'", filename.c_str());
            return false;
        }

        if (!content.empty())
        {
            file->write(content.c_str(), content.size());
        }
        
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    IFile::IFile()
    {
    }
    
    IFile::~IFile()
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    
    IFileSystem::IFileSystem()
        : access_(FileAccess::FA_NONE)
    {
    }
    
    IFileSystem::~IFileSystem()
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(FileSystemMgr);
    
    FileSystemMgr::FileSystemMgr()
    {
    }
    
    FileSystemMgr::~FileSystemMgr()
    {
    }
    
    void FileSystemMgr::addSearchPath(const std::string & path, bool front)
    {
        FileSystemPtr fs = createFileSystem(path);
        if(!fs)
        {
            ORA_LOG_ERROR("the search path '%s' doens't exist.", path.c_str());
            return;
        }
        
        addSearchPath(path, fs, front);
    }
    
    void FileSystemMgr::addSearchPath(const std::string & path, FileSystemPtr fs, bool front)
    {
        std::string relPath = path;
        canonicalizePath(relPath);
        if(!relPath.empty() && relPath.back() != '/')
            relPath += '/';
        
        searchPaths_.push_back(relPath);
        fileSystems_.push_back(fs);
        
        ORA_LOG_INFO("add search path: %s", relPath.c_str());
    }
    
    void FileSystemMgr::setSearchPath(const StringVector & paths)
    {
        searchPaths_.clear();
        fileSystems_.clear();
        
        for(const std::string & path : paths)
        {
            addSearchPath(path);
        }
    }
    
    bool FileSystemMgr::isFileExist(const std::string & path)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(fs->getAccess() & FA_READ && fs->exist(searchPaths_[i] + path))
            {
                return true;
            }
        }
        return false;
    }
    
    FilePtr FileSystemMgr::openFile(const std::string & path, IFile::Mode mode)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            
            if(mode & (IFile::MD_WRITE | IFile::MD_CREATE))
            {
                if(!(fs->getAccess() & FA_WRITE)) continue; // no write access
            }
            else if(!(fs->getAccess() & FA_READ))
            {
                continue; // no read access
            }
            
            FilePtr file = fs->openFile(searchPaths_[i] + path, mode);
            if(file)
            {
                return file;
            }
        }
        return nullptr;
    }
    
    FILE* FileSystemMgr::openRawFile(const std::string & file, const char * mode)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            
            if(strchr(mode, 'w') != nullptr || strchr(mode, '+') != nullptr)
            {
                if(!(fs->getAccess() & FA_WRITE)) continue; // no write access
            }
            else if(!(fs->getAccess() & FA_READ))
            {
                continue; // no read access
            }
            
            FILE *fp = fs->openRawFile((searchPaths_[i] + file).c_str(), mode);
            if(fp)
            {
                return fp;
            }
        }
        return nullptr;
    }
    
    void FileSystemMgr::setWritablePath(const std::string & path)
    {
        writablePath_ = path;
        canonicalizePath(writablePath_);
        
        if(!writablePath_.empty() && writablePath_.back() != '/')
            writablePath_ += '/';
    }
    
    std::string FileSystemMgr::getFullPath(const std::string & path)
    {
        std::string destPath;
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(!(fs->getAccess() & FA_READ)) continue;
            
            destPath = searchPaths_[i] + path;
            canonicalizePath(destPath);
            
            if(fs->exist(destPath))
            {
                return destPath;
            }
        }

        destPath.clear();
        return destPath;
    }
    
    std::string FileSystemMgr::getRelativePath(const std::string & fullPath)
    {
        std::string destPath = fullPath;
        canonicalizePath(destPath);
        
        for(const std::string & path : searchPaths_)
        {
            if(stringBeginWith(destPath, path))
                return fullPath.substr(path.length());
        }
        
        destPath.clear();
        return destPath;
    }
    
    bool FileSystemMgr::createDir(const std::string & path)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(fs->getAccess() & FA_WRITE && fs->createDir(searchPaths_[i] + path))
            {
                return true;
            }
        }
        return false;
    }
    
    bool FileSystemMgr::removeDir(const std::string & path)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(fs->getAccess() & FA_REMOVE && fs->removeDir(searchPaths_[i] + path))
            {
                return true;
            }
        }
        return false;
    }
    
    bool FileSystemMgr::removeFile(const std::string & path)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(fs->getAccess() & FA_REMOVE && fs->removeFile(searchPaths_[i] + path))
            {
                return true;
            }
        }
        return false;
    }
    
    bool FileSystemMgr::renameFile(const std::string & oldpath, const std::string & newpath)
    {
        for(size_t i = 0; i < searchPaths_.size(); ++i)
        {
            FileSystemPtr & fs = fileSystems_[i];
            if(fs->getAccess() & (FA_WRITE | FA_WRITE) &&
               fs->renameFile(searchPaths_[i] + oldpath, searchPaths_[i] + newpath))
            {
                return true;
            }
        }
        return false;
    }

}// end namespace ora
