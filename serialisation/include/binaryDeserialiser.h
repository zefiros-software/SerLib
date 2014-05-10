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
#ifndef __SERIALISATION_BINARYDESERIALISER_H__
#define __SERIALISATION_BINARYDESERIALISER_H__

#include "interface/abstractDeserialiser.h"

#include "types.h"

#include <iostream>
#include <cstring>

class Message;

class BinaryDeserialiser
    : public AbstractDeserialiser
{
public:

    BinaryDeserialiser( std::istream &stream );

    virtual void DeserialiseMessage( Message &message );

protected:

    std::istream *mStream;
    char mBuffer[ 256 ];
    size_t mBufferSize;
    size_t mBufferIndex;

    void Deserialise( Message &message, const uint32_t index, const Internal::Type::Type type );

    void DeserialiseRepeated( Message &message, const uint32_t index );

    template< typename T >
    void DeserialiseNum( Message &message, const uint32_t index )
    {
        T value;
        ReadBytes( &value, sizeof( T ) );
        message.Store( value, index, 0x00 );
    }

    template< typename T >
    void DeserialiseRepeatedNum( Message &message, const uint32_t index, const uint32_t size )
    {
        T value;

        for ( uint32_t i = 0; i < size; ++i )
        {
            ReadBytes( &value, sizeof( T ) );
            message.StoreRepeated( value, index, i );
        }
    }

    template< typename T >
    void ReadBytes( T *firstByte, const size_t byteCount )
    {
        char *c = reinterpret_cast< char * >( firstByte );
        const size_t diff = mBufferSize - mBufferIndex;
        const size_t diff2 = byteCount - diff;

        if ( diff2 <= 0 )
        {
            memcpy( c, mBuffer + mBufferIndex, byteCount );
            mBufferIndex += byteCount;
        }
        else
        {
            memcpy( c, mBuffer + mBufferIndex, diff );
            mBufferIndex += diff;
            FillBuffer();

            char *size = c + diff;
            ReadBytes( size, diff2 );
        }
    }

    void FillBuffer()
    {
        const size_t remaining = mBufferSize - mBufferIndex;
        memcpy( mBuffer, mBuffer + mBufferIndex, remaining );
        mBufferIndex = 0;
        mStream->read( mBuffer + remaining, sizeof( mBuffer ) - remaining );
        mBufferSize = remaining + ( uint32_t )mStream->gcount();
    }

    void ReadString( std::string &str );

    uint64_t ReadVarInt();
};

#endif