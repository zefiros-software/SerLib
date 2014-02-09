#include "util.h"




U32 Util::F32ToU32( const F32 f )
{
    S32 exp;
    F32 fi = frexp( f, &exp );
    U32 result = static_cast< U8 >( exp );
    result |= ZigZag< S32, U32 >( static_cast< S32 >( ldexp( fi, 23 ) ) ) << 8;

    return result;
}

F32 Util::U32ToF32( const U32 i )
{
    S32 exp = static_cast< S8 >( i );
    F32 fi = ldexp( ldexp( static_cast< F32 >( ZagZig< U32, S32 >( i >> 8 ) ), -23 ), exp );

    return fi;
}

U32 Util::GetHeaderIndex( U64 header )
{
    return ( U32 )( header >> 3 );
}

Type Util::GetHeaderType( U64 header )
{
    return ( Type )( header & 7 );
}

U64 Util::CreateHeader( U32 index, Type t )
{
    return ( ( ( U64 )index ) << 3 ) | ( ( U64 )t & 7 );
}
