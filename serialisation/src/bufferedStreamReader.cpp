/**
 * Copyright (c) 2017 Zefiros Software.
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
#include "serialisation/bufferedStreamReader.h"

SERIALISATION_INLINE BufferedStreamReader::BufferedStreamReader( const std::string &fileName )
    : mStreamReader( fileName ),
      mReadIndex( 0 ),
      mReadSize( 0 )
{

}

SERIALISATION_INLINE BufferedStreamReader::BufferedStreamReader( std::ifstream &stream )
    : mStreamReader( stream ),
      mReadIndex( 0 ),
      mReadSize( 0 )
{

}

SERIALISATION_INLINE BufferedStreamReader::BufferedStreamReader( std::fstream &stream )
    : mStreamReader( stream ),
      mReadIndex( 0 ),
      mReadSize( 0 )
{

}

SERIALISATION_INLINE BufferedStreamReader::BufferedStreamReader( std::istream &stream )
    : mStreamReader( stream ),
      mReadIndex( 0 ),
      mReadSize( 0 )
{
}

SERIALISATION_INLINE BufferedStreamReader::~BufferedStreamReader()
{
    Close();
}

SERIALISATION_INLINE void BufferedStreamReader::ClearBuffer()
{
    std::streamsize gCount = mStreamReader.GCount();

    mStreamReader.SeekG( gCount + mReadIndex - mReadSize );

    mReadIndex = 0;
    mReadSize = 0;
}

SERIALISATION_INLINE void BufferedStreamReader::Close()
{
    ClearBuffer();

    mStreamReader.Close();
}

SERIALISATION_INLINE void BufferedStreamReader::ReadBytes( char *const firstByte, size_t byteCount )
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

    memcpy( c, mReadBuffer + mReadIndex, byteCount );
    mReadIndex += static_cast<uint32_t>( byteCount );
}

SERIALISATION_INLINE void BufferedStreamReader::ReadBlock( char *const firstByte, size_t byteCount )
{
    uint32_t diff = mReadSize - mReadIndex;

    if ( byteCount < diff )
    {
        memcpy( firstByte, mReadBuffer + mReadIndex, byteCount );
        mReadIndex += static_cast<uint32_t>( byteCount );
    }
    else
    {
        memcpy( firstByte, mReadBuffer + mReadIndex, diff );
        mReadIndex = mReadSize;

        mStreamReader.ReadBytes( firstByte + diff, byteCount - diff );
    }
}

SERIALISATION_INLINE size_t BufferedStreamReader::ReadSize()
{
    size_t size = 0;
    uint8_t shift = 0;

    uint8_t byte;

    ReadPrimitive( byte );

    while ( byte & 0x80 )
    {
        size |= static_cast<size_t>( byte & 0x7F ) << shift;
        ReadPrimitive( byte );
        shift += 7;
    }

    size |= static_cast<size_t>( byte ) << shift;

    return size;
}

SERIALISATION_INLINE void BufferedStreamReader::FillReadBuffer()
{
    mReadIndex = 0;
    mStreamReader.ReadBytes( mReadBuffer, SERIALISATION_SERIALISERS_BUFFERSIZE );
    mReadSize = static_cast<uint32_t>( mStreamReader.GCount() );

    mStreamReader.ClearEOF();

    assert( mReadSize > mReadIndex );
}