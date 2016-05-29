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

#include <cstring>

class BufferedStreamReader
{
public:

    explicit BufferedStreamReader( const std::string &fileName );

    explicit BufferedStreamReader( std::ifstream &stream );

    explicit BufferedStreamReader( std::fstream &stream );

    explicit BufferedStreamReader( std::istream &stream );

    ~BufferedStreamReader();

    void ClearBuffer();

    void Close();

    void ReadBytes( char *const firstByte, size_t byteCount );

    void ReadBlock( char *const firstByte, size_t byteCount );

    size_t ReadSize();

    template< typename TPrimitive >
    void ReadPrimitive( TPrimitive &value )
    {
        ReadBytes( reinterpret_cast< char *const >( &value ), sizeof( TPrimitive ) );
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

private:

    char mReadBuffer[ SERIALISATION_SERIALISERS_BUFFERSIZE ];

    StreamReader mStreamReader;

    uint32_t mReadIndex;
    uint32_t mReadSize;

    void FillReadBuffer();

    BufferedStreamReader &operator=( const BufferedStreamReader & );
    BufferedStreamReader( const BufferedStreamReader & );
};

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/bufferedStreamReader.cpp"
#endif

#endif