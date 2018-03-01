/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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
 *
 * @endcond
 */
#pragma once
#ifndef __SERIALISATION_UTIL_H__
#define __SERIALISATION_UTIL_H__

#include "serialisation/types.h"

#include <stdlib.h>
#include <math.h>

namespace Util
{
    template< typename S, typename U >
    U ZigZag(const S s)
    {
        return (s >> ((sizeof(S) << 3) - 1)) ^ (s << 1);
    }

    template< typename U, typename S >
    S ZagZig(const U u)
    {
        return (u >> 1) ^ (-(static_cast< S >(u) & 1));
    }

    uint32_t FloatToUInt32(const float f);

    float UInt32ToFloat(const uint32_t i);

    uint64_t DoubleToUInt64(const double f);

    double UInt64ToDouble(const uint64_t i);

    template< typename T >
    uint8_t CalculateVarIntSize(T val)
    {
        uint8_t size = 1;

        for (val >>= 7; val > 0; val >>= 7)
        {
            ++size;
        }

        return size;
    }

    template< typename T >
    T CreateHeader(const T index, const Internal::Type::Type t)
    {
        return (index << 3) | (static_cast< T >(t) & 0x07);
    }

    template< typename T >
    Internal::Type::Type GetHeaderType(const T header)
    {
        return static_cast< Internal::Type::Type >(header & 0x07);
    }

    template< typename T >
    T GetHeaderIndex(const T header)
    {
        return static_cast< T >(header >> 3);
    }
}

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/util.cpp"
#endif

#endif