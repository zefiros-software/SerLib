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

#include "poolHolder.h"
#include "pools.h"


PoolHolder &PoolHolder::Get()
{
    static Pools mStaticPools;
    static PoolHolder mPoolHolder( &mStaticPools );
    return mPoolHolder;
}

template<>
ObjectPool< RepeatedData< Message > > &PoolHolder::GetPool()
{
    return mPools->RMessagePool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< std::string > > > &PoolHolder::GetPool()
{
    return mPools->RStringPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint8_t > > > &PoolHolder::GetPool()
{
    return mPools->RU8Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint16_t > > > &PoolHolder::GetPool()
{
    return mPools->RU16Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint32_t > > > &PoolHolder::GetPool()
{
    return mPools->RU32Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint64_t > > > &PoolHolder::GetPool()
{
    return mPools->RU64Pool;
}

template<>
ObjectPool< RepeatedData< VarIntData > > &PoolHolder::GetPool()
{
    return mPools->RVarIntPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int8_t > > > &PoolHolder::GetPool()
{
    return mPools->RS8Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int16_t > > > &PoolHolder::GetPool()
{
    return mPools->RS16Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int32_t > > > &PoolHolder::GetPool()
{
    return mPools->RS32Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int64_t > > > &PoolHolder::GetPool()
{
    return mPools->RS64Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< float > > > &PoolHolder::GetPool()
{
    return mPools->RFloatPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< double > > > &PoolHolder::GetPool()
{
    return mPools->RDoublePool;
}

template<>
ObjectPool< Message > &PoolHolder::GetPool()
{
    return mPools->MessagePool;
}

template<>
ObjectPool< SerialiseData< std::string > > &PoolHolder::GetPool()
{
    return mPools->StringPool;
}

template<>
ObjectPool< SerialiseData< uint8_t > > &PoolHolder::GetPool()
{
    return mPools->U8Pool;
}

template<>
ObjectPool< SerialiseData< uint16_t > > &PoolHolder::GetPool()
{
    return mPools->U16Pool;
}

template<>
ObjectPool< SerialiseData< uint32_t > > &PoolHolder::GetPool()
{
    return mPools->U32Pool;
}

template<>
ObjectPool< SerialiseData< uint64_t > > &PoolHolder::GetPool()
{
    return mPools->U64Pool;
}

template<>
ObjectPool< VarIntData > &PoolHolder::GetPool()
{
    return mPools->VarIntPool;
}

template<>
ObjectPool< SerialiseData< int8_t > > &PoolHolder::GetPool()
{
    return mPools->S8Pool;
}

template<>
ObjectPool< SerialiseData< int16_t > > &PoolHolder::GetPool()
{
    return mPools->S16Pool;
}

template<>
ObjectPool< SerialiseData< int32_t > > &PoolHolder::GetPool()
{
    return mPools->S32Pool;
}

template<>
ObjectPool< SerialiseData< int64_t > > &PoolHolder::GetPool()
{
    return mPools->S64Pool;
}

template<>
ObjectPool< SerialiseData< float > > &PoolHolder::GetPool()
{
    return mPools->FloatPool;
}

template<>
ObjectPool< SerialiseData< double > > &PoolHolder::GetPool()
{
    return mPools->DoublePool;
}

PoolHolder::PoolHolder( Pools *const pools )
    : mPools( pools )
{
}

template<>
ObjectPool < std::vector< ISerialiseData * > > &PoolHolder::GetPool()
{
	return mPools->ISDVectorPool;
}

template<>
ObjectPool < std::vector< uint32_t > > &PoolHolder::GetPool()
{
	return mPools->IndexVectorPool;
}