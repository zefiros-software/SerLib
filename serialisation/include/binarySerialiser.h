#pragma once
#ifndef __SERIALISATION_BINARYSERIALISER_H__
#define __SERIALISATION_BINARYSERIALISER_H__

#include "interface/abstractSerialiser.h"
#include "types.h"

#include <iostream>
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

    uint32_t mBufferSize;
    uint32_t mBufferIndex;
    std::ostream *mStream;
    char mBuffer[ 256 ];

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
    void WriteBytes( const T *firstByte, const uint32_t byteCount )
    {
        int32_t diff = mBufferSize - mBufferIndex;
        int32_t diff2 = byteCount - diff;

        const char *c = reinterpret_cast< const char * >( firstByte );

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
		uint32_t mSize = 0;
		for ( uint64_t val = value; ( val > 0 || mSize == 0 ); val >>= 7, ++mSize )
		{
			char c = val & 127;

			if ( ( uint64_t )c != val )
			{
				c |= 128;
			}

			WriteBytes( &c, 1 );
		}
	}
};

#endif