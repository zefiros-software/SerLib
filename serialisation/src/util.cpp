#include "util.h"




uint32_t Util::FloatToUInt32( const float f )
{
    int32_t exp;
    float fi = frexp( f, &exp );
    uint32_t result = static_cast< uint8_t >( exp );
    result |= ZigZag< int32_t, uint32_t >( static_cast< int32_t >( ldexp( fi, 23 ) ) ) << 8;

    return result;
}

float Util::UInt32ToFloat( const uint32_t i )
{
    int32_t exp = static_cast< int8_t >( i );
    float fi = ldexp( ldexp( static_cast< float >( ZagZig< uint32_t, int32_t >( i >> 8 ) ), -23 ), exp );

    return fi;
}

uint64_t Util::DoubleToUInt64( const double f )
{
    int32_t exp;
    double fi = frexp( f, &exp );
    uint64_t result = static_cast< uint8_t >( exp );
    result |= ZigZag< uint64_t, uint64_t >( static_cast< uint64_t >( ldexp( fi, 52 ) ) ) << 11;

    return result;
}

double Util::UInt64ToDouble( const uint64_t i )
{
    int32_t exp = static_cast< int32_t >( ( ( i << 1 ) >> 52 ) );
    double fi = ldexp( ldexp( static_cast< float >( ZagZig< uint64_t, uint64_t >( i >> 11 ) ), -52 ), exp );

    return fi;
}

uint32_t Util::GetHeaderIndex( uint64_t header )
{
    return ( uint32_t )( header >> 3 );
}

Type::Type Util::GetHeaderType( uint64_t header )
{
    return ( Type::Type )( header & 7 );
}

uint64_t Util::CreateHeader( uint32_t index, Type::Type t )
{
    return ( ( ( uint64_t )index ) << 3 ) | ( ( uint64_t )t & 7 );
}
