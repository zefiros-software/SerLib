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
#ifndef __SERIALISATION_WRITEBUFFER_H__
#define __SERIALISATION_WRITEBUFFER_H__

#include "serialisation/streamWriter.h"
#include "serialisation/defines.h"

#include <cstring>

class BufferedStreamWriter
{
public:

    explicit BufferedStreamWriter( const std::string &fileName )
        : mStreamWriter( fileName ),
          mWriteIndex( 0 )
    {
    }

    explicit BufferedStreamWriter( std::ofstream &stream )
        : mStreamWriter( stream ),
          mWriteIndex( 0 )
    {
    }

    explicit BufferedStreamWriter( std::fstream &stream )
        : mStreamWriter( stream ),
          mWriteIndex( 0 )
    {
    }

    explicit BufferedStreamWriter( std::ostream &stream )
        : mStreamWriter( stream ),
          mWriteIndex( 0 )
    {
    }

    inline void ClearBuffer()
    {
        mStreamWriter.WriteBytes( mWriteBuffer, mWriteIndex );
        mWriteIndex = 0;

        mStreamWriter.ClearBuffer();
    }

    inline void Close()
    {
        ClearBuffer();

        mStreamWriter.Close();
    }

    ~BufferedStreamWriter()
    {
        Close();
    }

    SERIALISATION_FORCEINLINE void WriteBytes( const char *const firstByte, size_t byteCount )
    {
        uint32_t diff = SERIALISATION_SERIALISERS_BUFFERSIZE - mWriteIndex;

        const char *c = firstByte;

        while ( diff < byteCount )
        {
            memcpy( mWriteBuffer + mWriteIndex, c, diff );

            mWriteIndex += diff;
            c += diff;
            byteCount -= diff;

            ClearBuffer();

            diff = SERIALISATION_SERIALISERS_BUFFERSIZE - mWriteIndex;
        }

        memcpy( mWriteBuffer  + mWriteIndex, c, byteCount );
        mWriteIndex += static_cast< uint32_t >( byteCount );
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void WritePrimitive( const TPrimitive &value )
    {
        WriteBytes( reinterpret_cast< const char *const >( &value ), sizeof( TPrimitive ) );
    }

    SERIALISATION_FORCEINLINE void WriteBlock( const char *const firstByte, size_t byteCount )
    {
        uint32_t diff = SERIALISATION_SERIALISERS_BUFFERSIZE - mWriteIndex;

        if ( byteCount < diff )
        {
            memcpy( mWriteBuffer + mWriteIndex, firstByte, byteCount );
            mWriteIndex += static_cast< uint32_t >( byteCount );
        }
        else
        {
            ClearBuffer();

            mStreamWriter.WriteBytes( firstByte, byteCount );
        }
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void WritePrimitiveBlock( const TPrimitive *first, size_t count )
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

    SERIALISATION_FORCEINLINE void WriteSize( size_t size )
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

    char mWriteBuffer[ SERIALISATION_SERIALISERS_BUFFERSIZE ];

    StreamWriter mStreamWriter;

    uint8_t mVarIntBuffer[ 10 ];

    uint32_t mWriteIndex;

    BufferedStreamWriter &operator=( const BufferedStreamWriter & );
    BufferedStreamWriter( const BufferedStreamWriter & );
};

#endif