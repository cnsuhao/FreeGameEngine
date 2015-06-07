#include "file_tool.h"
#include "log_tool.h"
#include "file_imp.h"

namespace ora
{

    bool readFile(std::string & content, const std::string & filename, bool isBinary)
    {
        IFile::Mode mode = IFile::Mode(IFile::ModeRead | (isBinary ? IFile::ModeBinary : 0));
        FilePtr file = FileSystemMgr::instance()->fileSystem()->openFile(filename, mode);
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
        IFile::Mode mode = IFile::Mode(IFile::ModeWrite | (isBinary ? IFile::ModeBinary : 0));
        FilePtr file = FileSystemMgr::instance()->fileSystem()->openFile(filename, mode);
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
    {
    }
    
    IFileSystem::~IFileSystem()
    {
    }
    
    bool IFileSystem::init()
    {
        ORA_LOG_DEBUG("default path:  %s", _defaultPath.c_str());
        ORA_LOG_DEBUG("current path:  %s", getCurrentPath().c_str());
        ORA_LOG_DEBUG("module path:   %s", getModulePath().c_str());
        ORA_LOG_DEBUG("writable path: %s", getWritablePath().c_str());
        return true;
    }
    
    bool IFileSystem::isFileExist(const std::string & filename)
    {
        return !((getFullPath(filename)).empty());
    }
    
    std::string IFileSystem::getFullPath(const std::string & filename)
    {
        std::string destPath;
        for(const std::string & path : _paths)
        {
            destPath = path + filename;
            canonicalizePath(destPath);
            
            if(existFile(destPath)) return destPath;
        }
        
        destPath.clear();
        return destPath;
    }
    
    std::string IFileSystem::getRelativePath(const std::string & fullPath)
    {
    	std::string destPath;
        for(const std::string & path : _paths)
        {
            if(stringBeginWith(fullPath, path))
                return fullPath.substr(path.length());
        }
        
        destPath.clear();
        return destPath;
    }
    
    void IFileSystem::addSearchPath(const std::string & path)
    {
        std::string relPath = _defaultPath + path;
        canonicalizePath(relPath);
        if(!relPath.empty() && relPath.back() != '/')
            relPath += '/';
        
        _paths.push_back(relPath);
        
        ORA_LOG_INFO("add search path: %s", relPath.c_str());
    }
    
    void IFileSystem::setSearchPath(const StringVector & paths)
    {
        _paths.clear();
        
        for(const std::string & path : paths)
        {
            addSearchPath(path);
        }
    }
    
    FilePtr IFileSystem::openFile(const std::string & filename, IFile::Mode mode)
    {
        std::string fullPath;
        std::string strMode;
        if(mode & IFile::ModeRead)
        {
            fullPath = getFullPath(filename);
            if(fullPath.empty())
            {
                return nullptr;
            }
            
            strMode += "r";
            if(mode & IFile::ModeBinary)
                strMode += "b";
            if(mode & IFile::ModeWrite)
                strMode += "+";
        }
        else if(mode & IFile::ModeWrite)
        {
            fullPath = getWritablePath() + filename;
            
            strMode += "w";
            if(mode & IFile::ModeBinary)
                strMode += "b";
            
            //ORA_LOG_DEBUG("Write File: %s", fullPath.c_str());
        }
        else
        {
            return nullptr;
        }
        
        return openFileInternal(fullPath, strMode);
    }

    FilePtr IFileSystem::openFileInternal(const std::string & filename, const std::string & mode)
    {
        FILE *pFile = fopen(filename.c_str(), mode.c_str());
        if(!pFile) return nullptr;
        
        return new FileImp(pFile);
    }

    /*static*/ void IFileSystem::canonicalizePath(std::string & path)
    {
        formatPathSlash(path);

        std::vector<size_t> slashes;
        slashes.push_back(0);

        size_t plength = path.length();

        std::string ret;
        ret.reserve(plength);

        size_t i = 0;
        while (i < plength)
        {
            ret.push_back(path[i]);

            if (path[i] != '/')
            {
                ++i;
                continue;
            }

            slashes.push_back(ret.size());

            ++i;
            for (; i < plength && path[i] == '/'; ++i) // '//'
                ; //do nothing.

            if (i < plength && path[i] == '.')
            {
                if (i + 1 < plength && path[i + 1] == '.') // '..'
                {
                    if (i + 2 >= plength || path[i + 2] == '/') // '../'
                    {
                        slashes.pop_back();
                        ret.resize(slashes.back());

                        i += 3; // '../next'
                    }
                }
                else if (i + 1 < plength && path[i + 1] == '/') // './'
                {
                    i += 2; // './next'
                }
            }
        }
        path.swap(ret);
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

}// end namespace ora
