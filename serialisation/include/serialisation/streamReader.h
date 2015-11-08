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
#ifndef __SERIALISATION_STREAMREADER_H__
#define __SERIALISATION_STREAMREADER_H__

#include "serialisation/types.h"

#include <assert.h>
#include <fstream>
#include <limits>

class StreamReader
{
public:

    friend class BufferedStreamReader;

    explicit StreamReader( const std::string &fileName )
        : mFileStream( fileName.c_str(), std::ifstream::binary | std::ifstream::in ),
          mStream( &mFileStream )
    {
        assert( mFileStream.is_open() && "File does not exist" );
    }

    explicit StreamReader( std::ifstream &stream )
        : mStream( &stream )
    {
        assert( mStream->flags() & std::ios::binary );
    }

    explicit StreamReader( std::fstream &stream )
        : mStream( &stream )
    {
        assert( ( stream.flags() & std::ios::in ) && "Not an input stream" );
        assert( ( stream.flags() & std::ios::binary ) && "File stream is not in binary mode" );
    }

    explicit StreamReader( std::istream &stream )
        : mStream( &stream )
    {
    }

    inline void ClearBuffer()
    {
    }

    inline void Close()
    {
        ClearBuffer();

        if ( mFileStream.is_open() )
        {
            mFileStream.close();
        }
    }

    ~StreamReader()
    {
        Close();
    }

    inline void ReadBytes( char *const firstByte, size_t byteCount )
    {
        mStream->read( firstByte, byteCount );
    }

    template< typename TPrimitive >
    void ReadPrimitive( TPrimitive &value )
    {
        ReadBytes( reinterpret_cast< char *const >( &value ), sizeof( TPrimitive ) );
    }

    inline void ReadBlock( char *const firstByte, size_t byteCount )
    {
        ReadBytes( firstByte, byteCount );
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

    std::ifstream mFileStream;
    std::istream *mStream;

    StreamReader &operator=( const StreamReader & );
    StreamReader( const StreamReader & );

    void SeekG( std::ios::off_type count )
    {
        mStream->seekg( count, std::ios_base::beg );
    }

    std::streamsize GCount()
    {
        return mStream->gcount();
    }

    void ClearEOF()
    {
        if ( mStream->eof() )
        {
            mStream->clear();
        }
    }
};

#endif