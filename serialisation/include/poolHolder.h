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
#ifndef __SERIALISATION_POOLHOLDER_H__
#define __SERIALISATION_POOLHOLDER_H__

#include "repeatedData.h"
#include "varIntData.h"
#include "objectPool.h"
#include "message.h"

#include <stdint.h>

class PoolHolder
{
public:

    static PoolHolder &Get();

    template< typename T >
    ObjectPool< T > &GetPool()
    {
        assert( false );
    }

private:

    ObjectPool< RepeatedData< Message > > mRMessagePool;
    ObjectPool< RepeatedData< SerialiseData< std::string > > > mRStringPool;

    ObjectPool< RepeatedData< SerialiseData< uint8_t > > > mRU8Pool;
    ObjectPool< RepeatedData< SerialiseData< uint16_t > > > mRU16Pool;
    ObjectPool< RepeatedData< SerialiseData< uint32_t > > > mRU32Pool;
    ObjectPool< RepeatedData< SerialiseData< uint64_t > > > mRU64Pool;

    ObjectPool< RepeatedData< VarIntData > > mRVarIntPool;

    ObjectPool< RepeatedData< SerialiseData< int8_t > > > mRS8Pool;
    ObjectPool< RepeatedData< SerialiseData< int16_t > > > mRS16Pool;
    ObjectPool< RepeatedData< SerialiseData< int32_t > > > mRS32Pool;
    ObjectPool< RepeatedData< SerialiseData< int64_t > > > mRS64Pool;

    ObjectPool< RepeatedData< SerialiseData< float > > > mRFloatPool;
    ObjectPool< RepeatedData< SerialiseData< double > > > mRDoublePool;

    ObjectPool< Message > mMessagePool;
    ObjectPool< SerialiseData< std::string > > mStringPool;

    ObjectPool< SerialiseData< uint8_t > > mU8Pool;
    ObjectPool< SerialiseData< uint16_t > > mU16Pool;
    ObjectPool< SerialiseData< uint32_t > > mU32Pool;
    ObjectPool< SerialiseData< uint64_t > > mU64Pool;

    ObjectPool< VarIntData > mVarIntPool;

    ObjectPool< SerialiseData< int8_t > > mS8Pool;
    ObjectPool< SerialiseData< int16_t > > mS16Pool;
    ObjectPool< SerialiseData< int32_t > > mS32Pool;
    ObjectPool< SerialiseData< int64_t > > mS64Pool;

    ObjectPool< SerialiseData< float > > mFloatPool;
    ObjectPool< SerialiseData< double > > mDoublePool;

    PoolHolder();
    PoolHolder( const PoolHolder & );
    void operator=( const PoolHolder & );
};

template<>
ObjectPool< RepeatedData< Message > > &PoolHolder::GetPool();

template<>
ObjectPool< RepeatedData< SerialiseData< std::string > > > &PoolHolder::GetPool();

template<>
ObjectPool< RepeatedData< SerialiseData< uint8_t > > > &PoolHolder::GetPool();

template<>
ObjectPool< RepeatedData< SerialiseData< uint16_t > > > &PoolHolder::GetPool();

template<>
ObjectPool< RepeatedData< SerialiseData< uint32_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< uint64_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< VarIntData > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< int8_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< int16_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< int32_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< int64_t > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< float > > > &PoolHolder::GetPool();

template<>
ObjectPool < RepeatedData< SerialiseData< double > > > &PoolHolder::GetPool();

template<>
ObjectPool < Message > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< std::string > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< uint8_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< uint16_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< uint32_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< uint64_t > > &PoolHolder::GetPool();

template<>
ObjectPool < VarIntData > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< int8_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< int16_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< int32_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< int64_t > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< float > > &PoolHolder::GetPool();

template<>
ObjectPool < SerialiseData< double > > &PoolHolder::GetPool();

#endif