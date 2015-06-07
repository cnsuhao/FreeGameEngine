
#include "pch.hpp"
#include "quick_file_reader.h"



QuickFileReader::QuickFileReader(const char *data, size_t len, size_t pos)
    : data_(data)
    , pos_(pos)
    , len_(len)
    , good_(true)
{
    checkSize(0);
}

QuickFileReader & QuickFileReader::operator >> ( std::string & t )
{
    size_t len = 0;
    (*this) >> len;

    if(len > 0)
    {
        t.resize(len);
        read(&t[0], len);
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

bool QuickFileReader::read( void* buffer, unsigned int size )
{
    if(checkSize(size))
    {
        memcpy(buffer, data_ + pos_, size);
    }

    pos_ += size;
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