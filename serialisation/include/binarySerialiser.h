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
#ifndef __SERIALISATION_BINARYSERIALISER_H__
#define __SERIALISATION_BINARYSERIALISER_H__

#include "interface/abstractSerialiser.h"

#include "defines.h"
#include "types.h"

#include <iostream>
#include <string.h>
#include <vector>

class BinarySerialiser
    : public AbstractSerialiser
{
public:

    BinarySerialiser( std::ostream &stream );

    void SerialiseMessage( Message &message );

protected:

    virtual void Prepare( ISerialiseData *const data, const uint32_t index, const uint32_t flags );

    virtual void Serialise( AbstractRepeatedData *const data );

    virtual void Serialise( Message *const message );

    virtual void Serialise( SerialiseData< std::string > *const data );

    virtual void Serialise( SerialiseData< uint8_t > *const data );

    virtual void Serialise( SerialiseData< uint16_t > *const data );

    virtual void Serialise( SerialiseData< uint32_t > *const data );

    virtual void Serialise( SerialiseData< uint64_t > *const data );

    virtual void Serialise( VarIntData *const data );

    virtual void Serialise( SerialiseData< int8_t > *const data );

    virtual void Serialise( SerialiseData< int16_t > *const data );

    virtual void Serialise( SerialiseData< int32_t > *const data );

    virtual void Serialise( SerialiseData< int64_t > *const data );

    virtual void Serialise( SerialiseData< float > *const data );

    virtual void Serialise( SerialiseData< double > *const data );

protected:

    std::ostream *mStream;

    size_t mBufferIndex;
    size_t mBufferSize;

    char mBuffer[ SERIALISERS_BUFFERSIZE ];

    void WriteHeader( uint32_t index, Internal::Type::Type type );

    template< typename T >
    void SerialiseNum( T &value, const uint32_t flags )
    {
        if ( flags & Message::Packed )
        {
            WriteVarInt( value );
        }
        else
        {
            WriteBytes( &value, sizeof( T ) );
        }
    }

    void FlushBuffer()
    {
        mStream->write( mBuffer, mBufferIndex );
        mBufferIndex = 0;
    }

    template< typename T >
    void WriteBytes( const T *firstByte, const size_t byteCount )
    {
        const int32_t diff = mBufferSize - mBufferIndex;
        const int32_t diff2 = byteCount - diff;

        const int8_t *c = reinterpret_cast< const int8_t * >( firstByte );

        if ( diff2 <= 0 )
        {
            memcpy( mBuffer + mBufferIndex, c, byteCount );
            mBufferIndex += byteCount;
        }
        else
        {
            memcpy( mBuffer + mBufferIndex, c, diff );
            mBufferIndex += diff;
            FlushBuffer();
            WriteBytes( c + diff, diff2 );
        }
    }

    void WriteVarInt( const uint64_t value )
    {
        uint32_t size = 0;

        for ( uint64_t val = value; val > 0 || size == 0; val >>= 7, ++size )
        {
            int8_t c = val & 127;

            if ( ( uint64_t )c != val )
            {
                c |= 128;
            }

            WriteBytes( &c, 1 );
        }
    }
};

#endif