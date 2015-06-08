#include "file_imp.h"

namespace ora
{
    FileImp::FileImp(FILE * p)
        : _pFile(p)
        , _length(-1)
    {}
    
    FileImp::~FileImp()
    {
        close();
    }
    
    size_t FileImp::length()
    {
        if(_length == -1)
        {
            size_t cur = tell();
            seek(0, SK_END);
            _length = tell();
            seek(cur, SK_BEG);
        }
        return _length;
    }
    
    size_t FileImp::tell()
    {
        assert(_pFile);
        
        return size_t(ftell(_pFile));
    }
    
    void FileImp::seek(size_t pos, Seek seekMode)
    {
        assert(_pFile);
        
        int seek_mode = SEEK_CUR;
        if(seekMode == SK_BEG) seek_mode = SEEK_SET;
        else if(seekMode == SK_END) seek_mode = SEEK_END;
        
        fseek(_pFile, pos, seek_mode);
    }
    
    size_t FileImp::read(void * buffer, size_t n)
    {
        assert(_pFile);
        
        return fread(buffer, 1, n, _pFile);
    }
    
    size_t FileImp::write(const void * buffer, size_t n)
    {
        assert(_pFile);
        return fwrite(buffer, 1, n, _pFile);
    }
    
    void FileImp::flush()
    {
        if(_pFile != nullptr)
        {
            fflush(_pFile);
        }
    }
    
    void FileImp::close()
    {
        if(_pFile != nullptr)
        {
            fclose(_pFile);
            _pFile = nullptr;
        }
    }

} // end namespace ora
