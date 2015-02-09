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
#ifndef __SERIALISATION_STREAMBUFFER_H__
#define __SERIALISATION_STREAMBUFFER_H__

#include "serialisation/types.h"

#include <fstream>

#include <string.h>
#include <stdio.h>

template< uint32_t BufferSize >
class StreamBuffer
{
public:

    StreamBuffer( const std::string &fileName )
        : mFileStream( fileName.c_str() ),
          mStream( &mFileStream ),
          mWriteIndex( 0 ),
          mWriteSize( 0 ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {
    }

    StreamBuffer( std::iostream &stream )
        : mStream( &stream ),
          mWriteIndex( 0 ),
          mWriteSize( 0 ),
          mReadIndex( 0 ),
          mReadSize( 0 )
    {

    }

    ~StreamBuffer()
    {
        Close();
    }

    void Close()
    {
        ClearReadBuffer();
        FlushWriteBuffer();

        if ( mFileStream.is_open() )
        {
            mFileStream.close();
        }
    }

    template< typename T >
    void WriteBytes( const T *const firstByte, size_t byteCount )
    {
        const int32_t diff = BufferSize - mWriteIndex;
        const std::ptrdiff_t diff2 = byteCount - diff;

        const int8_t *const c = reinterpret_cast< const int8_t *const >( firstByte );

        if ( diff2 <= 0 )
        {
            //std::copy( c, c + byteCount, mWriteBuffer + mWriteIndex );
            memcpy( mWriteBuffer + mWriteIndex, c, byteCount );
            mWriteIndex += static_cast< uint32_t >( byteCount );
        }
        else
        {
            //std::copy( c, c + diff, mWriteBuffer + mWriteIndex );
            memcpy( mWriteBuffer + mWriteIndex, c, diff );
            mWriteIndex += diff;
            FlushWriteBuffer();
            WriteBytes( c + diff, diff2 );
        }
    }

    template< typename T >
    void ReadBytes( T *const firstByte, size_t byteCount )
    {
        char *const c = reinterpret_cast< char *const >( firstByte );
        const int32_t diff = mReadSize - mReadIndex;
        const std::ptrdiff_t diff2 = byteCount - diff;

        if ( diff2 <= 0 )
        {
            //char *const firstRead = mReadBuffer + mReadIndex;
            //std::copy( firstRead, firstRead + byteCount, c );
            memcpy( c, mReadBuffer + mReadIndex, byteCount );
            mReadIndex += static_cast< uint32_t >( byteCount );
        }
        else
        {
            //char *const firstRead = mReadBuffer + mReadIndex;
            //std::copy( firstRead, firstRead + diff, c );
            memcpy( c, mReadBuffer + mReadIndex, diff );
            mReadIndex += diff;
            FillReadBuffer();

            char *size = c + diff;
            ReadBytes( size, diff2 );
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

        WriteBytes( mVarIntBuffer, ++bufferIndex );
    }

    size_t ReadSize()
    {
        uint64_t size = 0; uint8_t shift = 0;

        uint8_t byte;

        ReadBytes( &byte, 1 );

        while ( byte & 0x80 )
        {
            size |= static_cast< uint64_t >( byte & 0x7F ) << shift;
            ReadBytes( &byte, 1 );
            shift += 7;
        }

        size |= static_cast< uint64_t >( byte ) << shift;

        return static_cast< size_t >( size );
    }
    void FlushWriteBuffer()
    {
        mStream->write( mWriteBuffer, mWriteIndex );
        mStream->flush();
        mWriteIndex = 0;
    }

    void ClearReadBuffer()
    {
        mStream->seekg( static_cast<  std::ios::off_type >( mReadIndex - mReadSize ), std::ios::cur );
        mReadIndex = 0;
        mReadSize = 0;
    }

private:

    char mReadBuffer[ BufferSize ];
    char mWriteBuffer[ BufferSize ];

    std::fstream mFileStream;

    std::iostream *mStream;

    uint8_t mVarIntBuffer[ 10 ];

    int32_t mWriteIndex;
    int32_t mWriteSize;
    int32_t mReadIndex;
    int32_t mReadSize;

    void FillReadBuffer()
    {
        if ( mWriteIndex > 0 )
        {
            FlushWriteBuffer();
        }

        const int32_t remaining = mReadSize - mReadIndex;

        //char *const firstRemaining = mReadBuffer + mReadIndex;
        //std::copy( firstRemaining, firstRemaining + remaining, mReadBuffer );
        memcpy( mReadBuffer, mReadBuffer + mReadIndex, remaining );
        mReadIndex = 0;

        //const uint32_t size = AvailableReadCound();

        const uint32_t remainingBufferSize = BufferSize - remaining;

        //const uint32_t readSize = remainingBufferSize < size ? remainingBufferSize : size;

        mStream->read( mReadBuffer + remaining, remainingBufferSize );
        const uint32_t bytesRead = static_cast< uint32_t >( mStream->gcount() );
        mReadSize = remaining + bytesRead;

        //If eof was reached, clear the error bits
        if ( mStream->eof() )
        {
            mStream->clear();
        }
    }
};

#endif