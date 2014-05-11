#pragma once
#ifndef __SERIALISATION_POOLS_H__
#define __SERIALISATION_POOLS_H__

#include "repeatedData.h"
#include "varIntData.h"
#include "objectPool.h"
#include "message.h"

struct Pools
{
    ObjectPool< RepeatedData< Message > > RMessagePool;
    ObjectPool< RepeatedData< SerialiseData< std::string > > > RStringPool;

    ObjectPool< RepeatedData< SerialiseData< uint8_t > > > RU8Pool;
    ObjectPool< RepeatedData< SerialiseData< uint16_t > > > RU16Pool;
    ObjectPool< RepeatedData< SerialiseData< uint32_t > > > RU32Pool;
    ObjectPool< RepeatedData< SerialiseData< uint64_t > > > RU64Pool;

    ObjectPool< RepeatedData< VarIntData > > RVarIntPool;

    ObjectPool< RepeatedData< SerialiseData< int8_t > > > RS8Pool;
    ObjectPool< RepeatedData< SerialiseData< int16_t > > > RS16Pool;
    ObjectPool< RepeatedData< SerialiseData< int32_t > > > RS32Pool;
    ObjectPool< RepeatedData< SerialiseData< int64_t > > > RS64Pool;

    ObjectPool< RepeatedData< SerialiseData< float > > > RFloatPool;
    ObjectPool< RepeatedData< SerialiseData< double > > > RDoublePool;

    ObjectPool< Message > MessagePool;
    ObjectPool< SerialiseData< std::string > > StringPool;

    ObjectPool< SerialiseData< uint8_t > > U8Pool;
    ObjectPool< SerialiseData< uint16_t > > U16Pool;
    ObjectPool< SerialiseData< uint32_t > > U32Pool;
    ObjectPool< SerialiseData< uint64_t > > U64Pool;

    ObjectPool< VarIntData > VarIntPool;

    ObjectPool< SerialiseData< int8_t > > S8Pool;
    ObjectPool< SerialiseData< int16_t > > S16Pool;
    ObjectPool< SerialiseData< int32_t > > S32Pool;
    ObjectPool< SerialiseData< int64_t > > S64Pool;

    ObjectPool< SerialiseData< float > > FloatPool;
    ObjectPool< SerialiseData< double > > DoublePool;
};

#endif