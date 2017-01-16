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
#pragma once
#ifndef __SERIALISATION_STREAMWRITER_H__
#define __SERIALISATION_STREAMWRITER_H__

#include "serialisation/defines.h"
#include "serialisation/types.h"

#include <assert.h>
#include <fstream>
#include <limits>

class StreamWriter
{
public:

    explicit StreamWriter( const std::string &fileName );

    explicit StreamWriter( std::ofstream &stream );

    explicit StreamWriter( std::fstream &stream );

    explicit StreamWriter( std::ostream &stream );

    ~StreamWriter();

    void ClearBuffer();

    void Close();

    SERIALISATION_FORCEINLINE void WriteBytes( const char *const firstByte, size_t byteCount )
    {
        mStream->write( firstByte, byteCount );
    }

    void WriteBlock( const char *const firstByte, size_t byteCount );

    void WriteSize( size_t size );

    template< typename TPrimitive >
    void WritePrimitive( const TPrimitive &value )
    {
        WriteBytes( reinterpret_cast< const char *const >( &value ), sizeof( TPrimitive ) );
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

private:

    uint8_t mVarIntBuffer[ 10 ];

    std::ofstream mFileStream;
    std::ostream *mStream;

    StreamWriter &operator=( const StreamWriter & );
    StreamWriter( const StreamWriter & );
};

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/streamWriter.cpp"
#endif

#endif