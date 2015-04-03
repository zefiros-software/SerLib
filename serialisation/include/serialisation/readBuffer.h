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
#ifndef __READBUFFER_H__
#define __READBUFFER_H__

#include "defines.h"
#include "types.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <limits>

class ReadBuffer
{
public:

    ReadBuffer( const std::string &fileName )
        : mFileStream( fileName.c_str(), std::ifstream::binary | std::ifstream::in ),
          mStream( &mFileStream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
        assert( mFileStream.is_open() && "File does not exist" );
    }

    ReadBuffer( std::ifstream &stream )
        : mStream( &stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
        assert( mStream->flags() & std::ios::binary );
    }

    ReadBuffer( std::fstream &stream )
        : mStream( &stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
        assert( ( stream.flags() & std::ios::in ) && "Not an input stream" );
        assert( ( stream.flags() & std::ios::binary ) && "File stream is not in binary mode" );
    }

    ReadBuffer( std::istream &stream )
        : mStream( &stream ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {

    }

    inline void ClearBuffer()
    {
        mStream->seekg( static_cast<  std::ios::off_type >( mReadIndex - mReadSize ), std::ios::cur );
        mReadIndex = 0;
        mReadSize = 0;
    }

    inline void Close()
    {
        ClearBuffer();

        if ( mFileStream.is_open() )
        {
            mFileStream.close();
        }
    }

    ~ReadBuffer()
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

            mStream->read( firstByte + diff, byteCount - diff );
        }
    }

    template< typename TPrimitive >
    void ReadPrimitiveBlock( TPrimitive *first, size_t count )
    {
        const size_t maxBlockSize = std::numeric_limits< size_t >::max() / sizeof( TPrimitive );

        size_t readBlockSize;

        while ( count > 0 )
        {
            readBlockSize = count > maxBlockSize ? maxBlockSize : count;

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

    char mReadBuffer[ SERIALISERS_BUFFERSIZE ];

    std::ifstream mFileStream;
    std::istream *mStream;

    uint32_t mReadIndex;
    uint32_t mReadSize;

    inline void FillReadBuffer()
    {
        mReadIndex = 0;
        mStream->read( mReadBuffer, SERIALISERS_BUFFERSIZE );
        mReadSize = static_cast< uint32_t >( mStream->gcount() );

        if ( mStream->eof() )
        {
            mStream->clear();
        }
    }

    ReadBuffer &operator=( const ReadBuffer & );
    ReadBuffer( const ReadBuffer & );
};

#endif