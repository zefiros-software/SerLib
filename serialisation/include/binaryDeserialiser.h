#pragma once
#ifndef __SERIALISATION_BINARYDESERIALISER_H__
#define __SERIALISATION_BINARYDESERIALISER_H__

#include "interface/abstractDeserialiser.h"
#include "types.h"

#include <iostream>

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
    uint32_t mBufferSize;
    uint32_t mBufferIndex;

    void Deserialise( Message &message, const uint32_t index, const Internal::Type::Type type );

    void DeserialiseRepeated( Message &message, const uint32_t index );

    template< typename T >
    void DeserialiseNum( Message &message, const uint32_t index )
    {
        T value;
        ReadBytes( &value, sizeof( T ) );
        message.Store( value, index, 0 );
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
    void ReadBytes( T *firstByte, const uint32_t byteCount )
    {
        char *c = reinterpret_cast< char * >( firstByte );
        int32_t diff = mBufferSize - mBufferIndex;
        int32_t diff2 = byteCount - diff;

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
            ReadBytes( c + diff, diff2 );
        }
    }

    void FillBuffer()
    {
        uint32_t remaining = mBufferSize - mBufferIndex;
        memcpy( mBuffer, mBuffer + mBufferIndex, remaining );
        mBufferIndex = 0;
        mStream->read( mBuffer + remaining, sizeof( mBuffer ) - remaining );
        mBufferSize = remaining + (uint32_t)mStream->gcount();
    }

    void ReadString( std::string &str );

    uint64_t ReadVarInt();
};

#endif