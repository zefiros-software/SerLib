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
#ifndef __SERIALISATION_STREAMWRITER_H__
#define __SERIALISATION_STREAMWRITER_H__

#include "defines.h"
#include "types.h"

#include <assert.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <limits>

class StreamWriter
{
public:

    StreamWriter( const std::string &fileName )
        : mFileStream( fileName.c_str(), std::ifstream::binary | std::ifstream::out ),
          mStream( &mFileStream )
    {
        assert( mFileStream.is_open() && "File does not exist" );
    }

    StreamWriter( std::ofstream &stream )
        : mStream( &stream )
    {
        assert( mStream->flags() & std::ios::binary );
    }

    StreamWriter( std::fstream &stream )
        : mStream( &stream )
    {
        assert( ( stream.flags() & std::ios::out ) && "Not an input stream" );
        assert( ( stream.flags() & std::ios::binary ) && "File stream is not in binary mode" );
    }

    StreamWriter( std::ostream &stream )
        : mStream( &stream )
    {

    }

    inline void ClearBuffer()
    {
        mStream->flush();
    }

    inline void Close()
    {
        ClearBuffer();

        if ( mFileStream.is_open() )
        {
            mFileStream.close();
        }
    }

    ~StreamWriter()
    {
        Close();
    }

    inline void WriteBytes( const char *const firstByte, size_t byteCount )
    {
        mStream->write( firstByte, byteCount );
    }

    template< typename TPrimitive >
    void WritePrimitive( const TPrimitive &value )
    {
        WriteBytes( reinterpret_cast< const char *const >( &value ), sizeof( TPrimitive ) );
    }

    inline void WriteBlock( const char *const firstByte, size_t byteCount )
    {
        WriteBytes( firstByte, byteCount );
    }

    template< typename TPrimitive >
    void WritePrimitiveBlock( const TPrimitive *first, size_t count )
    {
        const size_t maxBlockSize = std::numeric_limits< size_t >::max() / sizeof( TPrimitive );

        while ( count > 0 )
        {
            const size_t writeBlockSize = count > maxBlockSize ? maxBlockSize : count;

            WriteBlock( reinterpret_cast< const char *const >( first ), writeBlockSize * sizeof( TPrimitive ) );
            count -= writeBlockSize;
            first += writeBlockSize;
        }
    }

    void WriteSize( size_t size )
    {
        uint8_t bufferIndex;

        for ( bufferIndex = 0; size >= 0x80; size >>= 7, bufferIndex++ )
        {
            mVarIntBuffer[ bufferIndex ] = static_cast< uint8_t >( ( size & 0x7F ) | 0x80 );
        }

        mVarIntBuffer[ bufferIndex ] = static_cast< uint8_t >( size );

        WriteBytes( reinterpret_cast< char * >( mVarIntBuffer ), ++bufferIndex );
    }

private:

    uint8_t mVarIntBuffer[ 10 ];

    std::ofstream mFileStream;
    std::ostream *mStream;

    StreamWriter &operator=( const StreamWriter & );
    StreamWriter( const StreamWriter & );
};

#endif