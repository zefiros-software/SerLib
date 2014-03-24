#include "serialiseData.h"
#include "interface/abstractSerialiser.h"
#include "util.h"

template<> template<>
void SerialiseData< uint8_t >::Store< int8_t >( int8_t &value, Mode::Mode mode )
{
	ZigZagStore< uint8_t >( value, mode );
}

template<> template<>
void SerialiseData< uint16_t >::Store< int16_t >( int16_t &value, Mode::Mode mode )
{
    ZigZagStore< uint16_t >( value, mode );
}

template<> template<>
void SerialiseData< uint32_t >::Store< int32_t >( int32_t &value, Mode::Mode mode )
{
    ZigZagStore< uint32_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< int64_t >( int64_t &value, Mode::Mode mode )
{
    ZigZagStore< uint64_t >( value, mode );;
}

template<> template<>
void SerialiseData< int8_t >::Store< uint8_t >( uint8_t &value, Mode::Mode mode )
{
    ZagZigStore< int8_t >( value, mode );
}

template<> template<>
void SerialiseData< int16_t >::Store< uint16_t >( uint16_t &value, Mode::Mode mode )
{
    ZagZigStore< int16_t >( value, mode );
}

template<> template<>
void SerialiseData< int32_t >::Store< uint32_t >( uint32_t &value, Mode::Mode mode )
{
    ZagZigStore< int32_t >( value, mode );
}

template<> template<>
void SerialiseData< int64_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
    ZagZigStore< int64_t >( value, mode );
}

template<> template<>
void SerialiseData< uint8_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint8_t >( value, mode );
}

template<> template<>
void SerialiseData< uint16_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint16_t >( value, mode );
}

template<> template<>
void SerialiseData< uint32_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint32_t >( value, mode );
}

template<> template<>
void SerialiseData< int8_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint8_t >( value, mode );
}

template<> template<>
void SerialiseData< int16_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint16_t >( value, mode );
}

template<> template<>
void SerialiseData< int32_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
	StoreT< uint32_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< uint8_t >( uint8_t &value, Mode::Mode mode )
{
	StoreT< uint64_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< uint16_t >( uint16_t &value, Mode::Mode mode )
{
	StoreT< uint64_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< uint32_t >( uint32_t &value, Mode::Mode mode )
{
	StoreT< uint64_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< int8_t >( int8_t &value, Mode::Mode mode )
{
	ZigZagStore< uint8_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< int16_t >( int16_t &value, Mode::Mode mode )
{
    ZigZagStore< uint16_t >( value, mode );
}

template<> template<>
void SerialiseData< uint64_t >::Store< int32_t >( int32_t &value, Mode::Mode mode )
{
	ZigZagStore< uint32_t >( value, mode );
}

template<> template<>
void SerialiseData< uint32_t >::Store< float >( float &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;

    Store( flexman, mode );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

template<> template<>
void SerialiseData< float >::Store< uint32_t >( uint32_t &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    float f = isSerialising ? Util::UInt32ToFloat( value ) : 0;

    Store( f, mode );

    if ( !isSerialising )
    {
        value = Util::FloatToUInt32( f );
    }
}

template<> template<>
void SerialiseData< uint64_t >::Store< double >( double &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;

    Store( flexman, mode );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

template<> template<>
void SerialiseData< double >::Store< uint64_t >( uint64_t &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    double d = isSerialising ? Util::UInt64ToDouble( value ) : 0;

    Store( d, mode );

    if ( !isSerialising )
    {
        value = Util::DoubleToUInt64( d );
    }
}
