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

#include "objectPool.h"

#include <stdint.h>
#include <string>

template< typename T >
class RepeatedData;

template< typename T >
class SerialiseData;

class VarIntData;
class Message;
class ISerialiseData;

struct Pools;

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

    Pools *mPools;

    PoolHolder( Pools *const pools );
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

template<>
ObjectPool < std::vector< ISerialiseData * > > &PoolHolder::GetPool();

template<>
ObjectPool < std::vector< uint32_t > > &PoolHolder::GetPool();

#endif