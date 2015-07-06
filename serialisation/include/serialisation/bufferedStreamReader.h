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
#ifndef __SERIALISATION_READBUFFER_H__
#define __SERIALISATION_READBUFFER_H__

#include "serialisation/streamReader.h"
#include "serialisation/defines.h"

class BufferedStreamReader
{
public:

    explicit BufferedStreamReader( const std::string &fileName )
        : mStreamReader( fileName ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
    }

    explicit BufferedStreamReader( std::ifstream &stream )
        : mStreamReader( stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
    }

    explicit BufferedStreamReader( std::fstream &stream )
        : mStreamReader( stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
    }

    explicit BufferedStreamReader( std::istream &stream )
        : mStreamReader( stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
    }

    inline void ClearBuffer()
    {
        mStreamReader.SeekG( static_cast< std::ios::off_type >( mReadIndex - mReadSize ) );
        mReadIndex = 0;
        mReadSize = 0;
    }

    inline void Close()
    {
        ClearBuffer();

        mStreamReader.Close();
    }

    ~BufferedStreamReader()
    {
        Close();
    }

    inline void ReadBytes( char *const firstByte, size_t byteCount )
    {
        uint32_t diff = mReadSize - mReadIndex;

        char *c = firstByte;

        while ( diff < byteCount )
        {
            memcpy( c, mReadBuffer + mReadIndex, diff );

            mReadIndex += diff;
            c += diff;
            byteCount -= diff;

            FillReadBuffer();

            diff = mReadSize - mReadIndex;
        }

        memcpy( c, mReadBuffer  + mReadIndex, byteCount );
        mReadIndex += static_cast< uint32_t >( byteCount );
    }

    template< typename TPrimitive >
    void ReadPrimitive( TPrimitive &value )
    {
        ReadBytes( reinterpret_cast< char *const >( &value ), sizeof( TPrimitive ) );
    }

    inline void ReadBlock( char *const firstByte, size_t byteCount )
    {
        uint32_t diff = mReadSize - mReadIndex;

        if ( byteCount < diff )
        {
            memcpy( firstByte, mReadBuffer + mReadIndex, byteCount );
            mReadIndex += static_cast< uint32_t >( byteCount );
        }
        else
        {
            memcpy( firstByte, mReadBuffer + mReadIndex, diff );
            mReadIndex = mReadSize;

            mStreamReader.ReadBytes( firstByte + diff, byteCount - diff );
        }
    }

    template< typename TPrimitive >
    void ReadPrimitiveBlock( TPrimitive *first, size_t count )
    {
        const size_t maxBlockSize = std::numeric_limits< size_t >::max() / sizeof( TPrimitive );

        while ( count > 0 )
        {
            const size_t readBlockSize = count > maxBlockSize ? maxBlockSize : count;

            ReadBlock( reinterpret_cast< char *const >( first ), readBlockSize * sizeof( TPrimitive ) );
            count -= readBlockSize;
            first += readBlockSize;
        }
    }

    size_t ReadSize()
    {
        size_t size = 0;
        uint8_t shift = 0;

        uint8_t byte;

        ReadPrimitive( byte );

        while ( byte & 0x80 )
        {
            size |= static_cast< size_t >( byte & 0x7F ) << shift;
            ReadPrimitive( byte );
            shift += 7;
        }

        size |= static_cast< size_t >( byte ) << shift;

        return size;
    }

private:

    char mReadBuffer[ SERIALISATION_SERIALISERS_BUFFERSIZE ];

    StreamReader mStreamReader;

    uint32_t mReadIndex;
    uint32_t mReadSize;

    inline void FillReadBuffer()
    {
        mReadIndex = 0;
        mStreamReader.ReadBytes( mReadBuffer, SERIALISATION_SERIALISERS_BUFFERSIZE );
        mReadSize = static_cast< uint32_t >( mStreamReader.GCount() );

        mStreamReader.ClearEOF();
    }

    BufferedStreamReader &operator=( const BufferedStreamReader & );
    BufferedStreamReader( const BufferedStreamReader & );
};

#endif