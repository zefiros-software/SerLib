/**
 * Copyright (c) 2014 Mick van Duijn, Koen Visscher and Paul Visscher
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once
#ifndef __VARINT_H__
#define __VARINT_H__

#include <iostream>
#include <stdint.h>

template< typename U >
class VarInt
{
public:

    VarInt( U val = 0 ) : mSize(0)
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

        for ( uint16_t i = 0; i < mSize; i++, shift += 7 )
        {
            char byteVal = mBytes[ i ];
            U result = byteVal & 0x7F;
            mValue |= ( U )( result ) << shift;
        }
    }
};

#endif
