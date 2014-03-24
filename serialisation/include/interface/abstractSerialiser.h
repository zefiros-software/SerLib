#pragma once
#ifndef __SERIALISATION_ABSTRACTSERIALISER_H__
#define __SERIALISATION_ABSTRACTSERIALISER_H__

#include <stdint.h>
#include <string>

class ISerialiseData;
class AbstractRepeatedData;
class Message;

template< typename T >
class SerialiseData;

class AbstractSerialiser
{
	friend class Message;
	friend class VarIntData;

	template< typename T >
	friend class RepeatedData;

	template< typename T >
	friend class SerialiseData;
public:

	virtual void SerialiseMessage( Message &message ) = 0;

protected:

	std::pair< uint32_t, ISerialiseData * > GetFromMessage( Message *const message, const uint32_t index );

	virtual void Serialise( ISerialiseData *const data );

	virtual void Prepare( ISerialiseData *const data, const uint32_t index, const uint32_t flags ) = 0;

	virtual void Serialise( AbstractRepeatedData *const repeated ) = 0;

	virtual void Serialise( Message *const message ) = 0;

	virtual void Serialise( SerialiseData< std::string > *const data ) = 0;

	virtual void Serialise( SerialiseData< uint8_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< uint16_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< uint32_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< uint64_t > *const data ) = 0;

	virtual void Serialise( VarIntData *const data ) = 0;

	virtual void Serialise( SerialiseData< int8_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< int16_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< int32_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< int64_t > *const data ) = 0;

	virtual void Serialise( SerialiseData< float > *const data ) = 0;

	virtual void Serialise( SerialiseData< double > *const data ) = 0;
};

#endif