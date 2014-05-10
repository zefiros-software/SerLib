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

#include "repeatedData.h"
#include "poolHolder.h"
#include "varIntData.h"
#include "message.h"

PoolHolder &PoolHolder::Get()
{
    static PoolHolder mPoolHolder;
    return mPoolHolder;
}

template<>
ObjectPool< RepeatedData< Message > > &PoolHolder::GetPool()
{
    return mRMessagePool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< std::string > > > &PoolHolder::GetPool()
{
    return mRStringPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint8_t > > > &PoolHolder::GetPool()
{
    return mRU8Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint16_t > > > &PoolHolder::GetPool()
{
    return mRU16Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint32_t > > > &PoolHolder::GetPool()
{
    return mRU32Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< uint64_t > > > &PoolHolder::GetPool()
{
    return mRU64Pool;
}

template<>
ObjectPool< RepeatedData< VarIntData > > &PoolHolder::GetPool()
{
    return mRVarIntPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int8_t > > > &PoolHolder::GetPool()
{
    return mRS8Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int16_t > > > &PoolHolder::GetPool()
{
    return mRS16Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int32_t > > > &PoolHolder::GetPool()
{
    return mRS32Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< int64_t > > > &PoolHolder::GetPool()
{
    return mRS64Pool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< float > > > &PoolHolder::GetPool()
{
    return mRFloatPool;
}

template<>
ObjectPool< RepeatedData< SerialiseData< double > > > &PoolHolder::GetPool()
{
    return mRDoublePool;
}

template<>
ObjectPool< Message > &PoolHolder::GetPool()
{
    return mMessagePool;
}

template<>
ObjectPool< SerialiseData< std::string > > &PoolHolder::GetPool()
{
    return mStringPool;
}

template<>
ObjectPool< SerialiseData< uint8_t > > &PoolHolder::GetPool()
{
    return mU8Pool;
}

template<>
ObjectPool< SerialiseData< uint16_t > > &PoolHolder::GetPool()
{
    return mU16Pool;
}

template<>
ObjectPool< SerialiseData< uint32_t > > &PoolHolder::GetPool()
{
    return mU32Pool;
}

template<>
ObjectPool< SerialiseData< uint64_t > > &PoolHolder::GetPool()
{
    return mU64Pool;
}

template<>
ObjectPool< VarIntData > &PoolHolder::GetPool()
{
    return mVarIntPool;
}

template<>
ObjectPool< SerialiseData< int8_t > > &PoolHolder::GetPool()
{
    return mS8Pool;
}

template<>
ObjectPool< SerialiseData< int16_t > > &PoolHolder::GetPool()
{
    return mS16Pool;
}

template<>
ObjectPool< SerialiseData< int32_t > > &PoolHolder::GetPool()
{
    return mS32Pool;
}

template<>
ObjectPool< SerialiseData< int64_t > > &PoolHolder::GetPool()
{
    return mS64Pool;
}

template<>
ObjectPool< SerialiseData< float > > &PoolHolder::GetPool()
{
    return mFloatPool;
}

template<>
ObjectPool< SerialiseData< double > > &PoolHolder::GetPool()
{
    return mDoublePool;
}

PoolHolder::PoolHolder()
{

}
