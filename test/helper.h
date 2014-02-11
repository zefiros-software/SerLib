#pragma once
#ifndef __HELPER_H__
#define __HELPER_H__

#include <climits>
#include <sstream>

template< typename T >
void SimpleSerialiseDeserialiseStream( T &c1, T &c2 )
{
    std::stringstream ss;
    {
        Message message( Message::Serialise );
        c1.OnSerialise( message );
        message.WriteToStream( ss );
    }

    {
        Message message( Message::Deserialise );
        message.ReadFromStream( ss );
        c2.OnSerialise( message );
    }
}

template< typename T >
T GenerateZebraValue()
{
    const uint16_t bits = sizeof( T ) << 3;
    T result = 0;

    for ( uint16_t i = 0; i < bits; ++++i )
    {
        result |= (T)( 1ull << i );
    }

    return result;
}

template< typename T >
T GenerateInvZebraValue()
{
    return GenerateZebraValue< T >() ^ std::numeric_limits<T>::max();
}

#endif