
#import <Foundation/Foundation.h>
#include "file_tool_imp.h"

#define MAX_PATH 1024

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
        std::string path = [[[NSBundle mainBundle] resourcePath] UTF8String];
        formatFilePath(path);
        
        return path;
    }
    
    std::string FileSystemImp::getWritablePath()
    {
	    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	    NSString *documentsDirectory = [paths objectAtIndex: 0];
	    std::string strRet = [documentsDirectory UTF8String];
	    formatFilePath(strRet);
	    return strRet;
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
	    
	    if (fullPath[0] != '/')
	    {
	        std::string path = getFilePath(fullPath);
	        std::string file = getFileName(fullPath);
	        
	        NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()]
	                                                             ofType:nil
	                                                        inDirectory:[NSString stringWithUTF8String:path.c_str()]];
	        return (fullpath != nil);
	    }
	    else
	    {
	        // Search path is an absolute path.
	        return ([[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]);
	    }
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
