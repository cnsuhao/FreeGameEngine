#ifndef H_QUICK_FILE_READER_H
#define H_QUICK_FILE_READER_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

namespace ora
{

    class QuickFileReader
    {
    public:
        QuickFileReader(const char *data, size_t len, size_t pos = 0);

        bool empty() const
        {
            return pos_ >= len_;
        }
        bool good() const
        {
            return good_;
        }

        int remain() const
        {
            return int(len_) - int(pos_);
        }

        template <class T>
        QuickFileReader & operator >> ( T & t )
        {
            read(&t, sizeof(T));
            return *this;
        }

        QuickFileReader & operator >> ( std::string & t );

        char get();
        bool read( void* buffer, size_t size );
        void offset(int size);
        void seek(int pos);

        const char * cdata() const
        {
            return data_;
        }
        size_t length() const
        {
            return len_;
        }
        size_t tell() const
        {
            return pos_;
        }

        template<typename TSeq>
        bool readSequence(TSeq & v, size_t n)
        {
            if(checkSize(n))
            {
                v.resize(n);
                _read_no_check(&v[0], n * sizeof(v[0]));
                return true;
            }
            else
            {
                v.resize(0);
                pos_ += n;
                return false;
            }
        }

        template<typename T>
        bool readVector(std::vector<T> & v, size_t n)
        {
            return readSequence(v, n);
        }

        template<typename SizeType>
        bool readString(std::string & v)
        {
            SizeType len;
            (*this) >> len;

            return readSequence(v, len);
        }

    private:
        const char *data_;
        size_t len_;
        size_t pos_;
        mutable bool good_;

        bool checkSize(size_t n) const;
        void _read_no_check(void *pBuffer, size_t size);
    };

}//end namespace ora

#endif // H_QUICK_FILE_READER_H