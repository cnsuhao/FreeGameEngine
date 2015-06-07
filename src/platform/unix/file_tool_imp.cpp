

#include "file_tool_imp.h"
#include <unistd.h>

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

namespace ora
{
	FileSystemImp::FileSystemImp()
	{

	}

    FileSystemImp::~FileSystemImp()
    {

    }
    
    bool FileSystemImp::init()
    {
        _defaultPath = getModulePath();

        return IFileSystem::init();
    }
    
    std::string FileSystemImp::getCurrentPath()
    {
        
        char path[MAX_PATH];
        path[0] = 0;
        getcwd(path, MAX_PATH);
        
        std::string strPath = path;
        formatFilePath(strPath);
    	return strPath;
    }
    
    /** ios & android，返回是res路径，其他平台返回app所在路径 */
    std::string FileSystemImp::getModulePath()
    {
        char path[MAX_PATH];
        readlink ("/proc/self/exe", path, MAX_PATH);
        
        std::string strPath = path;
        formatFilePath(strPath);
    	return strPath;
    }
    
    std::string FileSystemImp::getWritablePath()
    {
	    return getModulePath();
    }
    
    
    bool FileSystemImp::createDir(const std::string & fullPath)
    {
    	return false;
    }

    bool FileSystemImp::deleteDir(const std::string & fullPath)
    {
    	return false;
    }
    
    bool FileSystemImp::existFile(const std::string & fullPath)
    {
    	if (fullPath.empty())
	        return false;
	    
        return access(fullPath.c_str(), F_OK);
    }

    bool FileSystemImp::deleteFile(const std::string & fullPath)
    {
    	return false;
    }

    bool FileSystemImp::renameFile(const std::string & fullPath)
    {
    	return false;
    }

} // end namespace ora
