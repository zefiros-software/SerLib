#pragma once
#ifndef __UTIL_H__
#define __UTIL_H__

#include "types.h"
#include <cmath>
#include <vector>
#include <iostream>

namespace Util
{
    U32 F32ToU32( const F32 f );

    F32 U32ToF32( const U32 i );

    template< typename T >
    U8 CalculateVarIntSize( T val )
    {
        U8 size = 1;

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

    U64 CreateHeader( U32 index, Type t );

    Type GetHeaderType( U64 header );

    U32 GetHeaderIndex( U64 header );
}

#endif