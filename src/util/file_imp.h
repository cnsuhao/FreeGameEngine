#ifndef H_FILE_IMP
#define H_FILE_IMP

#include "file_tool.h"

namespace ora
{

    class FileImp : public IFile
    {
        FILE   *_pFile;
        size_t  _length;
    public:
        
        FileImp(FILE * p);
        ~FileImp();
        
        size_t length();
        
        size_t tell();
        void seek(size_t pos, Seek seekMode);
        
        size_t read(void * buffer, size_t n);
        size_t write(const void * buffer, size_t n);
        
        void flush();
        void close();
    };

} // end namespace ora

#endif //H_FILE_IMP
