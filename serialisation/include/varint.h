#pragma once
#ifndef __VARINT_H__
#define __VARINT_H__

#include "types.h"

#include <iostream>
#include <vector>
#include <math.h>

template< typename U >
class VarInt
{
public:

    VarInt( U val = 0 )
    {
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
        stream.write( mBytes, mSize );
    }

    void ReadFromStream( std::istream &stream )
    {
        mSize = 0;
        char c = 0;

        for ( bool next = true; next; next = ( c & 128 ) > 0, ++mSize )
        {
            stream.read( &c, 1 );
            mBytes[ mSize ] = c;
        }

        ConvertFromBytes();
    }

private:

    char mBytes[10];
    U mValue;
    size_t mSize;

    void ConvertToBytes()
    {
        mSize = 0;

        for ( U val = mValue; ( val > 0 || mSize == 0 ); val >>= 7, ++mSize )
        {
            char c = val & 127;

            if ( ( U )c != val )
            {
                c |= 128;
            }

            mBytes[ mSize ] = c;
        }
    }

    void ConvertFromBytes()
    {
        mValue = 0;
        size_t shift = 0;

        for ( int16_t i=0; i < mSize; i++, shift += 7 )
        {
            char byteVal = mBytes[ i ];
            U result = byteVal & 0x7F;
            mValue |= ( U )( result ) << shift;
        }
    }
};

#endif
