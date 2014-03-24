#pragma once
#ifndef __SERIALISATION_ISERIALISER_H__
#define __SERIALISATION_ISERIALISER_H__

#include <stdint.h>

class AbstractRepeatedData;
class Message;
class StringData;
class VarIntData;

template< typename T >
class NumData;

class ISerialiser
{
public:

    virtual void Serialise( VarIntData *const data, const uint32_t index );

    virtual void Serialise( NumData< uint8_t > *const data, const uint32_t index );

    virtual void Serialise( NumData< uint16_t > *const data, const uint32_t index );

    virtual void Serialise( NumData< uint32_t > *const data, const uint32_t index );

    virtual void Serialise( NumData< uint64_t > *const data, const uint32_t index );

    virtual void Serialise( StringData *const data, const uint32_t index );

    virtual void Serialise( Message *const message, const uint32_t index );

    virtual void Serialise( AbstractRepeatedData *const repeated, const uint32_t index );
};

#endif