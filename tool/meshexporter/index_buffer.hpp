/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include "mmath.h"

namespace Moo
{

class IndicesReference;

/**
 *	This is class is used to hold one reference to an index buffer
 *	entry. Its size is determinated by the index buffer's format
 */
class IndexReference
{
	int32 offset_;
	IndicesReference& reference_;
public:
	IndexReference( IndicesReference& reference, int32 offset )
		: reference_( reference ), offset_( offset )
	{}
	operator uint32() const;
	void operator=( uint32 value );
};

/**
 *	A reference to a block of memory representing an index buffer.
 *	These objects are generally returned by locking mechanics.
 *	The index array is not managed here, it is just used as a source.
 */
class IndicesReference
{
protected:
	void* indices_;
	uint32 size_;
	D3DFORMAT format_;
public:
	IndicesReference() : size_( 0 ), indices_( 0 ), format_( D3DFMT_INDEX16 )
	{}
	virtual ~IndicesReference(){}
	virtual void assign( void* buffer, uint32 numOfIndices, D3DFORMAT format )
	{
		size_ = numOfIndices;
		indices_ = buffer;
		format_ = format;
	}
	bool valid() const
	{
		return !!indices_;
	}
	void fill( const void* buffer, uint32 numOfIndices )
	{
		memcpy( indices_, buffer, numOfIndices * entrySize() );
	}
	void pull( void* buffer, uint32 numOfIndices ) const
	{
		memcpy( buffer, indices_, numOfIndices * entrySize() );
	}
	void copy( const IndicesReference& src, int count, int start = 0, int srcStart = 0, uint32 vertexBase = 0 )
	{
		for( int i = 0; i < count; i++ )
			set( start + i, (uint32)src[ srcStart + i ] + vertexBase );
	}
	D3DFORMAT format() const
	{
		return format_;
	}
	int entrySize() const
	{
		return bestEntrySize( format_ );
	}
	void* indices()
	{
		return indices_;
	}
	const void* indices() const
	{
		return indices_;
	}
	uint32 size() const
	{
		return size_;
	}
	IndexReference operator[]( int32 index )
	{
		return IndexReference( *this, index );
	}
	uint32 operator[]( int32 index ) const
	{
		return get( index );
	}
	void set( int32 index, uint32 value )
	{
		if( format_ == D3DFMT_INDEX16 )
			( (uint16*)indices_ )[ index ] = (uint16)value;
		else
			( (uint32*)indices_ )[ index ] = value;
		
	}
	uint32 get( int32 index ) const
	{
		if( format_ == D3DFMT_INDEX16 )
			return ( (uint16*)indices_ )[ index ];
		else
			return ( (uint32*)indices_ )[ index ];
		
	}
	static int bestEntrySize( D3DFORMAT format )
	{
		return format == D3DFMT_INDEX16 ? 2 : 4;
	}
	static D3DFORMAT bestFormat( uint32 vertexNum )
	{
		return vertexNum <= 65536 ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
	}
};

inline IndexReference::operator uint32() const
{
	return reference_.get( offset_ );
}

inline void IndexReference::operator=( uint32 value )
{
	reference_.set( offset_, value );
}

/**
 *	A class to hold the actual index data which can be updated and used
 *	to upload into a D3D resource via the IndexBuffer class below.
 */
class IndicesHolder : public IndicesReference
{
	IndicesHolder( const IndicesHolder& );
	void operator= ( const IndicesHolder& );
public:
	IndicesHolder( D3DFORMAT format = D3DFMT_INDEX16, uint32 entryNum = 0 )
	{
		format_ = format;
		size_ = entryNum;
		indices_ = new unsigned char[ size() * entrySize() ];
	}
	~IndicesHolder()
	{
		delete[] indices_;
	}
	void setSize( uint32 entryNum, D3DFORMAT format )
	{
		delete[] indices_;
		indices_ = 0;
		format_ = format;
		if( entryNum != 0 )
		{
			size_ = entryNum;
			indices_ = new unsigned char[ size() * entrySize() ];
		}
	}
	virtual void assign( const void* buffer, uint32 entryNum, D3DFORMAT format )
	{// exception unsafe
		delete[] indices_;
		size_ = entryNum;
		format_ = format;
		indices_ = new unsigned char[ size() * entrySize() ];
		memcpy( indices_, buffer, size() * entrySize() );
	}
};

};

#endif//INDEX_BUFFER_HPP
