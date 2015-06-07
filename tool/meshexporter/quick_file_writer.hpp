/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef QUICK_FILE_WRITER_HPP
#define QUICK_FILE_WRITER_HPP

#include "smartpointer.hpp"

/**
 *	This class is used to used to write from a buffer to a
 *	binary block.
 */
class QuickFileWriter
{
public:
	QuickFileWriter() { }

	template <class T> QuickFileWriter & operator <<( const T & t )
	{
		int len = sizeof( T );
		data_.append( (char*)&t, len );
		return *this;
	}


    QuickFileWriter & operator << (const std::string & s)
    {
        (*this) << (uint32)s.length();
        if(!s.empty()) this->write(s.c_str(), s.length());
        return *this;
    }

    QuickFileWriter & operator << (const char *p)
    {
        uint32 len = p ? strlen(p) : 0;
        *this << len;
        if(len > 0) this->write(p, len);
        return *this;
    }

	template <class E> QuickFileWriter & operator <<( std::vector<E> & v )
	{
		this->writeV( v );
		return *this;
	}

	template <class E> QuickFileWriter & operator <<( const std::vector<E> & v )
	{
		this->writeV( v );
		return *this;
	}

	QuickFileWriter& write( const void* buffer, uint32 size )
	{
		data_.append( (char*) buffer, size );
		return *this;
	}

    const std::string & getData() const { return data_; }

private:
	std::string data_;

	template <class E> void writeV( const std::vector<E> & v )
	{
		int elen = sizeof( E ); // Quenya: star :)
		int vlen = elen * v.size();
		data_.append( (char*) &v.front(), vlen );
	}
};

#endif // QUICK_FILE_WRITER_HPP
