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