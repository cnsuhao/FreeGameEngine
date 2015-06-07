

#include "quick_file_reader.h"
#include "util_config.h"
#include "assert_tool.h"

namespace ora
{

    QuickFileReader::QuickFileReader(const char *data, size_t len, size_t pos)
        : data_(data)
        , len_(len)
        , pos_(pos)
        , good_(true)
    {
        checkSize(0);
    }

    QuickFileReader & QuickFileReader::operator >> ( std::string & t )
    {
        uint32 len = 0;
        (*this) >> len;

        if(len > 0)
        {
            if(checkSize(len))
            {
                t.resize(len);
                read(&t[0], len);
            }
            else pos_ += len;
        }

        return *this;
    }

    char QuickFileReader::get()
    {
        char ch = 0;
        if(checkSize(1))
        {
            ch = data_[pos_];
        }

        pos_ += 1;
        return ch;
    }

    bool QuickFileReader::read( void* buffer, size_t size )
    {
        if(checkSize(size))
        {
            _read_no_check(buffer, size);
        }
        else pos_ += size;

        return good_;
    }

    void QuickFileReader::offset(int size)
    {
        pos_ += size;
    }

    void QuickFileReader::seek(int pos)
    {
        pos_ = pos;
    }

    bool QuickFileReader::checkSize(size_t n) const
    {
        if(pos_ + n > len_)
        {
            good_ = false;
        }

        return good_;
    }

    void QuickFileReader::_read_no_check(void *pBuffer, size_t size)
    {
        ASSERT_1(pBuffer && pos_ + size <= len_);

        memcpy(pBuffer, data_ + pos_, size);
        pos_ += size;
    }

}// end namespace ora