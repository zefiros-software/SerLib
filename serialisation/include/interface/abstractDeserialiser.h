#pragma once
#ifndef __SERIALISATION_ABSTRACTDESERIALISER_H__
#define __SERIALISATION_ABSTRACTDESERIALISER_H__

#include "message.h"

#include <stdint.h>
#include <string>

class AbstractRepeatedData;
class ISerialiseData;

template< typename T >
class SerialiseData;

class AbstractDeserialiser
{
    friend class Message;
    friend class VarIntData;

    template< typename T >
    friend class RepeatedData;

    template< typename T >
    friend class SerialiseData;

public:

    virtual void DeserialiseMessage( Message &message ) = 0;

protected:

    template< typename T >
    ISerialiseData *GetFromMessage( Message &message, const uint32_t index, const uint32_t flags = 0x00 )
    {
        ISerialiseData *data = message.FindSerialisable( index );

        if ( !data )
        {
            data = message.CreateDataType< T >( flags );
            message.InsertSerialiseDataAt( data, index );
        }

        return data;
    }
};

#endif