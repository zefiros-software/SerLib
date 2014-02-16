#include "util.h"


uint32_t Util::FloatToUInt32( const float f )
{
    int32_t exp;
    float fi = frexp( f, &exp );

    int32_t negative = exp < 0 ? 1 : 0;

    uint32_t result = ( abs( exp ) & 0xFF ) << 1;
    result |= ZigZag< int32_t, uint32_t >( static_cast< int32_t >( ldexp( fi, 22 ) ) ) << 9;
    result |= negative;

    return result;
}

float Util::UInt32ToFloat( const uint32_t i )
{
    int32_t exp = ( i & 0x1FF ) >> 1;

    int32_t negative = ( i & 0x01 );

    exp = ( negative == 1 ) ? -exp : exp;

    float fi = ldexp( ldexp( static_cast< float >( ZagZig< uint32_t, int32_t >( i >> 9 ) ), -22 ), exp );

    return fi;
}

uint64_t Util::DoubleToUInt64( const double f )
{
    int32_t exp;
    double fi = frexp( f, &exp );

    int32_t negative = exp < 0 ? 1 : 0;

    uint64_t result = ( uint64_t )( abs( exp ) & 0x7FF ) << 1;
    result |= ZigZag< int64_t, uint64_t >( static_cast< int64_t >( ldexp( fi, 51 ) ) ) << 12;
    result |= negative;

    return result;
}

double Util::UInt64ToDouble( const uint64_t i )
{
    int32_t exp = ( i & 0xFFF ) >> 1;

    int32_t negative = ( i & 0x01 );

    exp = ( negative == 1 ) ? -exp : exp;

    double fi = ldexp( ldexp( static_cast< double >( ZagZig< uint64_t, int64_t >( i >> 12 ) ), -51 ), exp );

    return fi;
}

uint32_t Util::GetHeaderIndex( uint64_t header )
{
    return ( uint32_t )( header >> 3 );
}

Type::Type Util::GetHeaderType( uint64_t header )
{
    return ( Type::Type )( header & 0x07 );
}

uint64_t Util::CreateHeader( uint32_t index, Type::Type t )
{
    return ( ( uint64_t )index << 3 ) | ( ( uint64_t )t & 0x07 );
}