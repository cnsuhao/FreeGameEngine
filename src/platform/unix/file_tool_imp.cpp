

#include "file_tool_imp.h"
#include "util/file_imp.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include <SDL/include/SDL.h>

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

namespace ora
{
	FileSystemImp::FileSystemImp()
	{
        setAccess(FA_ALL);
	}

    FileSystemImp::~FileSystemImp()
    {

    }
    
    bool FileSystemImp::exist(const std::string & path)
    {
        return access(path.c_str(), F_OK) == 0;
    }

    bool FileSystemImp::isFile(const std::string & path)
    {
        struct stat statbuf;
        if(0 == lstat(path.c_str(), &statbuf))
        {
            return (S_ISREG(statbuf.st_mode));
        }
        return false;
    }

    bool FileSystemImp::isDir(const std::string & path)
    {
        struct stat statbuf;
        if(0 == lstat(path.c_str(), &statbuf))
        {
            return (S_ISDIR(statbuf.st_mode));
        }
        return false;
    }
    
    FilePtr FileSystemImp::openFile(const std::string & path, IFile::Mode mode)
    {
        std::string smode;
        if (mode & IFile::MD_READ && mode & IFile::MD_CREATE) smode += "w+";
        else if(mode & IFile::MD_READ && mode & IFile::MD_WRITE) smode += "r+";
        else if(mode & IFile::MD_CREATE) smode += 'w';
        else if(mode & IFile::MD_READ) smode += 'r';
        
        if(smode.empty()) smode += 'r';
        if(mode & IFile::MD_BINARY) smode += 'b';
        
        FILE *fp = fopen(path.c_str(), smode.c_str());
        if(fp != nullptr) return new FileImp(fp);
        
        return nullptr;
    }

    FILE* FileSystemImp:: openRawFile(const char* path, const char * mode)
    {
        return fopen(path, mode);
    }
    
    bool FileSystemImp::removeFile(const std::string & path)
    {
        return remove(path.c_str());
    }

    bool FileSystemImp::renameFile(const std::string & oldpath, const std::string & newpath)
    {
        return rename(oldpath.c_str(), newpath.c_str());
    }

    
    bool FileSystemImp::createDir(const std::string & path)
    {
        return mkdir(path.c_str(), 0);
    }

    bool FileSystemImp::removeDir(const std::string & path)
    {
        return rmdir(path.c_str());
    }
    
    bool FileSystemImp::listDir(const std::string & path, StringVector & files)
    {
        DIR *dp;
        struct dirent *entry;
        if((dp = opendir(path.c_str())) == NULL)
        {
            return false;
        }
        
        while((entry = readdir(dp)) != NULL)
        {
            if(strcmp(entry->d_name, ".") == 0 ||
               strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            
            files.push_back(entry->d_name);
        }
        closedir(dp);
        return true;
    }


    std::string getCurrentPath()
    {
        char path[MAX_PATH];
        path[0] = 0;
        getcwd(path, MAX_PATH);
        
        std::string strPath = path;
        formatFilePath(strPath);
        return strPath;
    }
    
    /** ios & android，返回是res路径，其他平台返回app所在路径 */
    std::string getModulePath()
    {
        char * path = SDL_GetBasePath();
        
        std::string strPath = path;
        formatFilePath(strPath);
        SDL_free(path);
        
        return strPath;
    }
    
    FileSystemPtr createFileSystem(const std::string & path)
    {
        return new FileSystemImp();
    }
    
} // end namespace ora
