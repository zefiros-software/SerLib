#pragma once
#ifndef __VARINT_H__
#define __VARINT_H__

#include "types.h"
#include <vector>
#include <iostream>

template< typename U >
class VarInt
{
public:

    VarInt( U val = 0 )
    {
        mBytes.resize( ( size_t )ceil( ( sizeof( U ) << 3 ) / 7.0 ) );
        SetValue( val );
    }

    void SetValue( const U val )
    {
        mValue = val;
        ConvertToBytes();
    }

    U GetValue() const
    {
        return mValue;
    }

    size_t GetByteSize() const
    {
        return mSize;
    }

    void WriteToStream( std::ostream &stream ) const
    {
        stream.write( &mBytes.front(), mSize );
    }

    void ReadFromStream( std::istream &stream )
    {
        mSize = 0;

        auto it = mBytes.begin();
        char c = 0;

        for ( bool next = true; next; next = ( c & 128 ) > 0, ++it, ++mSize )
        {
            stream.read( &c, 1 );
            *it = c;
        }

        ConvertFromBytes();
    }

private:

    std::vector< char > mBytes;
    U mValue;
    size_t mSize;

    void ConvertToBytes()
    {
        mSize = 0;

        auto it = mBytes.begin();

        for ( U val = mValue; val > 0; val >>= 7, ++it, ++mSize )
        {
            char &c = *it;
            c = val & 127;

            if ( ( U )c != val )
            {
                c |= 128;
            }
        }
    }

    void ConvertFromBytes()
    {
        mValue = 0;
        size_t shift = 0;

        for ( auto it = mBytes.begin(), end = mBytes.begin() + mSize; it != end; ++it, shift += 7 )
        {
            mValue |= ( ( *it ) & 127 ) << shift;
        }
    }
};

#endif