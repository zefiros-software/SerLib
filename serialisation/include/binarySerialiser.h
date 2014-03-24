#pragma once
#ifndef __SERIALISATION_BINARYSERIALISER_H__
#define __SERIALISATION_BINARYSERIALISER_H__

#include "interface/abstractSerialiser.h"
#include "types.h"

#include <iostream>

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

    void WriteHeader( uint32_t index, Internal::Type::Type type );

    template< typename T >
    void SerialiseNum( T &value, const uint32_t flags )
    {
        if ( flags & Message::Packed )
        {
            VarInt< uint64_t >( value ).WriteToStream( *mStream );
        }
        else
        {
            WriteBytes( &value, sizeof( T ) );
        }
    }

    template< typename T >
    void WriteBytes( const T *firstByte, const uint32_t byteCount )
    {
        mStream->write( reinterpret_cast< const char * >( firstByte ), byteCount );
    }
};

#endif