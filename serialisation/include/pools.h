#pragma once
#ifndef __SERIALISATION_POOLS_H__
#define __SERIALISATION_POOLS_H__

#include "repeatedData.h"
#include "objectPool.h"
#include "message.h"

struct Pools
{
    ObjectPool< RepeatedData< Message > > RMessagePool;
    ObjectPool< RepeatedData< std::string > > RStringPool;

    ObjectPool< RepeatedData< uint8_t > > RU8Pool;
    ObjectPool< RepeatedData< uint16_t > > RU16Pool;
    ObjectPool< RepeatedData< uint32_t > > RU32Pool;
    ObjectPool< RepeatedData< uint64_t > > RU64Pool;

    ObjectPool< RepeatedData< int8_t > > RS8Pool;
    ObjectPool< RepeatedData< int16_t > > RS16Pool;
    ObjectPool< RepeatedData< int32_t > > RS32Pool;
    ObjectPool< RepeatedData< int64_t > > RS64Pool;

    ObjectPool< RepeatedData< float > > RFloatPool;
    ObjectPool< RepeatedData< double > > RDoublePool;

    ObjectPool< Message > MessagePool;
    ObjectPool< SerialiseData< std::string > > StringPool;

    ObjectPool< SerialiseData< uint8_t > > U8Pool;
    ObjectPool< SerialiseData< uint16_t > > U16Pool;
    ObjectPool< SerialiseData< uint32_t > > U32Pool;
    ObjectPool< SerialiseData< uint64_t > > U64Pool;

    ObjectPool< SerialiseData< int8_t > > S8Pool;
    ObjectPool< SerialiseData< int16_t > > S16Pool;
    ObjectPool< SerialiseData< int32_t > > S32Pool;
    ObjectPool< SerialiseData< int64_t > > S64Pool;

    ObjectPool< SerialiseData< float > > FloatPool;
    ObjectPool< SerialiseData< double > > DoublePool;

	ObjectPool< std::vector< ISerialiseData * > > ISDVectorPool;
	ObjectPool< std::vector< uint32_t > > IndexVectorPool;
};

#endif