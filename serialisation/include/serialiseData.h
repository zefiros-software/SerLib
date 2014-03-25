#pragma once
#ifndef __SERIALISATION_SERIALISEDATA_H__
#define __SERIALISATION_SERIALISEDATA_H__

#include "interface/IPrimitiveData.h"
#include "interface/abstractSerialiser.h"
#include "util.h"

#include <sstream>
#include <stdexcept>

template< typename T >
class SerialiseData
    : public IPrimitiveData
{
public:

    SerialiseData( const uint32_t flags = 0 )
        : mFlags( flags )
    {

    }

    virtual Internal::Type::Type GetType() const
    {
        return Internal::Type::GetEnum< SerialiseData< T > >();
    }

    virtual uint32_t GetFlags() const
    {
        return mFlags;
    }

    virtual void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    const T &GetValue() const
    {
        return mValue;
    }

    void Store( std::string &, Mode::Mode )
    {
        StoreError();
    };

    void Store( uint8_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( uint16_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( uint32_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( uint64_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( int8_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( int16_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( int32_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( int64_t &, Mode::Mode )
    {
        StoreError();
    }

    void Store( float &, Mode::Mode )
    {
        StoreError();
    }

    void Store( double &, Mode::Mode )
    {
        StoreError();
    }

    inline virtual void SerialiseTo( AbstractSerialiser *const serialiser )
    {
        serialiser->Serialise( this );
    }

protected:

    T mValue;
    uint32_t mFlags;

    template< typename V >
    V GetValue() const
    {
        return ( V )mValue;
    }

    template< typename V >
    void SetValue( const V value )
    {
        mValue = ( T )value;
    }

    template< typename U, typename S >
    void ZigZagStore( S &value, Mode::Mode mode )
    {
        bool isSerialising = mode == Mode::Serialise;

        U uVal = isSerialising ? Util::ZigZag< S, U >( value ) : 0;

        Store( uVal, mode );

        if ( !isSerialising )
        {
            value = Util::ZagZig< U, S >( uVal );
        }
    }

    template< typename S, typename U >
    void ZagZigStore( U &value, Mode::Mode mode )
    {
        bool isSerialising = mode == Mode::Serialise;

        S sVal = isSerialising ? Util::ZagZig< U, S >( value ) : 0;

        Store( sVal, mode );

        if ( !isSerialising )
        {
            value = Util::ZigZag< S, U >( sVal );
        }
    }

    template< typename S, typename U >
    void StoreS( U &value, Mode::Mode mode )
    {
        bool isSerialising = mode == Mode::Serialise;

        S sVal = isSerialising ? ( S )value : 0;

        Store( sVal, mode );

        if ( !isSerialising )
        {
            value = ( U )sVal;
        }
    }

	void StoreT( T &value, Mode::Mode mode )
	{
		switch (mode)
		{
		case Mode::Serialise:
			mValue = value;
			break;
		case Mode::Deserialise:
			value = mValue;
			break;
		}
	}

    inline void StoreError() const
    {
        throw std::invalid_argument( "Invalid conversion" );
    }
};


template<>
void SerialiseData< std::string >::Store( std::string &value, Mode::Mode mode )
{
	StoreT( value, mode );
}


template<>
void SerialiseData< uint8_t >::Store( uint8_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< uint16_t >::Store( uint16_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< uint32_t >::Store( uint32_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( uint64_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< int8_t >::Store( int8_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< int16_t >::Store( int16_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< int32_t >::Store( int32_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< int64_t >::Store( int64_t &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< float >::Store( float &value, Mode::Mode mode )
{
	StoreT( value, mode );
}

template<>
void SerialiseData< double >::Store( double &value, Mode::Mode mode )
{
	StoreT( value, mode );
}



template<>
void SerialiseData< uint8_t >::Store( int8_t &value, Mode::Mode mode )
{
	ZigZagStore< uint8_t >( value, mode );
}

template<>
void SerialiseData< uint16_t >::Store( int16_t &value, Mode::Mode mode )
{
	ZigZagStore< uint16_t >( value, mode );
}

template<>
void SerialiseData< uint32_t >::Store( int32_t &value, Mode::Mode mode )
{
	ZigZagStore< uint32_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( int64_t &value, Mode::Mode mode )
{
	ZigZagStore< uint64_t >( value, mode );;
}

template<>
void SerialiseData< int8_t >::Store( uint8_t &value, Mode::Mode mode )
{
	ZagZigStore< int8_t >( value, mode );
}

template<>
void SerialiseData< int16_t >::Store( uint16_t &value, Mode::Mode mode )
{
	ZagZigStore< int16_t >( value, mode );
}

template<>
void SerialiseData< int32_t >::Store( uint32_t &value, Mode::Mode mode )
{
	ZagZigStore< int32_t >( value, mode );
}

template<>
void SerialiseData< int64_t >::Store( uint64_t &value, Mode::Mode mode )
{
	ZagZigStore< int64_t >( value, mode );
}



template<>
void SerialiseData< uint8_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint8_t >( value, mode );
}

template<>
void SerialiseData< uint16_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint16_t >( value, mode );
}

template<>
void SerialiseData< uint32_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint32_t >( value, mode );
}

template<>
void SerialiseData< int8_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint8_t >( value, mode );
}

template<>
void SerialiseData< int16_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint16_t >( value, mode );
}

template<>
void SerialiseData< int32_t >::Store( uint64_t &value, Mode::Mode mode )
{
    StoreS< uint32_t >( value, mode );
}


template<>
void SerialiseData< uint64_t >::Store( uint8_t &value, Mode::Mode mode )
{
    StoreS< uint64_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( uint16_t &value, Mode::Mode mode )
{
    StoreS< uint64_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( uint32_t &value, Mode::Mode mode )
{
    StoreS< uint64_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( int8_t &value, Mode::Mode mode )
{
    ZigZagStore< uint8_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( int16_t &value, Mode::Mode mode )
{
    ZigZagStore< uint16_t >( value, mode );
}

template<>
void SerialiseData< uint64_t >::Store( int32_t &value, Mode::Mode mode )
{
    ZigZagStore< uint32_t >( value, mode );
}


template<>
void SerialiseData< uint32_t >::Store( float &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;

    Store( flexman, mode );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

template<>
void SerialiseData< float >::Store( uint32_t &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    float f = isSerialising ? Util::UInt32ToFloat( value ) : 0;

    Store( f, mode );

    if ( !isSerialising )
    {
        value = Util::FloatToUInt32( f );
    }
}

template<>
void SerialiseData< uint64_t >::Store( double &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;

    Store( flexman, mode );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

template<>
void SerialiseData< double >::Store( uint64_t &value, Mode::Mode mode )
{
    bool isSerialising = mode == Mode::Serialise;

    double d = isSerialising ? Util::UInt64ToDouble( value ) : 0;

    Store( d, mode );

    if ( !isSerialising )
    {
        value = Util::DoubleToUInt64( d );
    }
}

#endif