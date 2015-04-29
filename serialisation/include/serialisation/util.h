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
#ifndef __SERIALISATION_UTIL_H__
#define __SERIALISATION_UTIL_H__

#include "serialisation/types.h"

#include <stdint.h>
#include <float.h>
#include <math.h>

namespace Util
{
    template< typename S, typename U >
    U ZigZag( const S s )
    {
        return ( s >> ( ( sizeof( S ) << 3 ) - 1 ) ) ^ ( s << 1 );
    }

    template< typename U, typename S >
    S ZagZig( const U u )
    {
        return ( u >> 1 ) ^ ( -( static_cast< S >( u ) & 1 ) );
    }

    inline uint32_t FloatToUInt32( const float f )
    {
        int32_t exp;
        float fi = frexp( f, &exp );

        const int32_t sign = exp < 0 ? 1 : 0;

        uint32_t result = ( abs( exp ) & 0xff ) << 1;
        result |= ZigZag< int32_t, uint32_t >( static_cast< int32_t >( ldexp( fi, 22 ) ) ) << 9;
        result |= sign;

        return result;
    }

    inline float UInt32ToFloat( const uint32_t i )
    {
        int32_t exp = ( i & 0x1ff ) >> 1;

        int32_t sign = ( i & 0x01 );

        exp = ( sign == 1 ) ? -exp : exp;

        return ldexp( ldexp( static_cast< float >( ZagZig< uint32_t, int32_t >( i >> 9 ) ), -22 ), exp );
    }

    inline uint64_t DoubleToUInt64( const double f )
    {
        int32_t exp;
        double fi = frexp( f, &exp );

        const int32_t sign = exp < 0 ? 1 : 0;

        uint64_t result = static_cast< uint64_t >( abs( exp ) & 0x7FF ) << 1;
        result |= ZigZag< int64_t, uint64_t >( static_cast< int64_t >( ldexp( fi, 51 ) ) ) << 12;
        result |= sign;

        return result;
    }

    inline double UInt64ToDouble( const uint64_t i )
    {
        int32_t exp = ( i & 0xfff ) >> 1;

        const int32_t sign = ( i & 0x01 );

        exp = ( sign == 1 ) ? -exp : exp;

        return ldexp( ldexp( static_cast< double >( ZagZig< uint64_t, int64_t >( i >> 12 ) ), -51 ), exp );
    }

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

    template< typename T >
    T CreateHeader( const T index, const Internal::Type::Type t )
    {
        return ( index << 3 ) | ( static_cast< T >( t ) & 0x07 );
    }

    template< typename T >
    Internal::Type::Type GetHeaderType( const T header )
    {
        return static_cast< Internal::Type::Type >( header & 0x07 );
    }

    template< typename T >
    T GetHeaderIndex( const T header )
    {
        return static_cast< T >( header >> 3 );
    }
}

#endif