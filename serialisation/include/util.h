#pragma region copyright
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
#pragma endregion

#pragma once
#ifndef __UTIL_H__
#define __UTIL_H__

#include "types.h"

#include <iostream>
#include <vector>
#include <cmath>

namespace Util
{
    uint32_t FloatToUInt32( const float f );

    float UInt32ToFloat( const uint32_t i );

    uint64_t DoubleToUInt64( const double f );

    double UInt64ToDouble( const uint64_t i );

    template< typename T >
    uint8_t CalculateVarIntSize( T val )
    {
        uint8_t size = 1;

        for ( val >>= 7; val > 0; val >>= 7 )
        {
            ++size;
        }

        return size;
    }

    template< typename S, typename U >
    U ZigZag( const S s )
    {
        return ( s >> ( ( sizeof( S ) << 3 ) - 1 ) ) ^ ( s << 1 );
    }

    template< typename U, typename S >
    S ZagZig( const U u )
    {
        return ( u >> 1 ) ^ ( -( ( S )u & 1 ) );
    }

    uint64_t CreateHeader( uint32_t index, Type::Type t );

    Type::Type GetHeaderType( uint64_t header );

    uint32_t GetHeaderIndex( uint64_t header );
}

#endif