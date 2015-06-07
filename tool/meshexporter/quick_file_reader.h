#pragma once


class QuickFileReader
{
public:
    QuickFileReader(const char *data, size_t len, size_t pos = 0);

    bool empty() const { return pos_ >=len_; }
    bool good() const { return good_; }

    int remain() const { return int(len_) - int(pos_); }

    template <class T>
    QuickFileReader & operator >> ( T & t )
    {
        read(&t, sizeof(T));
        return *this;
    }

    QuickFileReader & operator >> ( std::string & t );

    char get();
    bool read( void* buffer, unsigned int size );
    void offset(int size);
    void seek(int pos);
    size_t length() const { return len_; }
    size_t tell() const{ return pos_; }

    template<typename T>
    bool readVector(std::vector<T> & v, size_t n)
    {
        v.resize(n);
        if(n > 0) return read(&v[0], n * sizeof(T));

        return true;
    }

private:
    const char *data_;
    size_t len_;
    size_t pos_;
    mutable bool good_;

    bool checkSize(size_t n) const;
};