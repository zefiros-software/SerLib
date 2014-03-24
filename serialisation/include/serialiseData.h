#pragma once
#ifndef __SERIALISATION_SERIALISEDATA_H__
#define __SERIALISATION_SERIALISEDATA_H__

#include "interface/ISerialiseData.h"
#include "interface/abstractSerialiser.h"

#include <sstream>
#include <stdexcept>

template< typename T >
class SerialiseData
    : public ISerialiseData
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

    void Store( T &value, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            mValue = value;
            break;

        case Mode::Deserialise:
            value = mValue;
            break;
        }
    }

    template< typename V >
    void Store( V &, Mode::Mode )
    {
        std::stringstream ss;
        ss << "Storing '" << typeid( V ).name() << "' in '" << typeid( SerialiseData< T > ).name() << "' is not supported.";
        std::cerr << ss.str();
        throw std::invalid_argument( ss.str().c_str() );
    }

    virtual void SerialiseTo( AbstractSerialiser *const serialiser )
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

        StoreV( sVal, mode );

        if ( !isSerialising )
        {
            value = Util::ZigZag< S, U >( sVal );
        }
    }

    template< typename T, typename U >
    void StoreT( U &value, Mode::Mode mode )
    {
        bool isSerialising = mode == Mode::Serialise;

        T tVal = isSerialising ? ( T )value : 0;

        Store( tVal, mode );

        if ( !isSerialising )
        {
            value = ( U )tVal;
        }
    }

    template< typename V >
    void StoreV( V &value, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            SetValue( value );
            break;

        case Mode::Deserialise:
            value = GetValue< V >();
            break;
        }
    }
};

template<>
void SerialiseData< std::string >::Store( std::string &value, Mode::Mode mode )
{
    switch ( mode )
    {
    case Mode::Serialise:
        mValue.assign( value );
        break;

    case Mode::Deserialise:
        value.assign( mValue );
        break;
    }
}

template<> template<>
void SerialiseData< uint8_t >::Store< int8_t >( int8_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint16_t >::Store< int16_t >( int16_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint32_t >::Store< int32_t >( int32_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< int64_t >( int64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int8_t >::Store< uint8_t >( uint8_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int16_t >::Store< uint16_t >( uint16_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int32_t >::Store< uint32_t >( uint32_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int64_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );


template<> template<>
void SerialiseData< uint8_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint16_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint32_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int8_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int16_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< int32_t >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );


template<> template<>
void SerialiseData< uint64_t >::Store< uint8_t >( uint8_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< uint16_t >( uint16_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< uint32_t >( uint32_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< int8_t >( int8_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< int16_t >( int16_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< int32_t >( int32_t &value, Mode::Mode mode );


template<> template<>
void SerialiseData< uint32_t >::Store< float >( float &value, Mode::Mode mode );

template<> template<>
void SerialiseData< float >::Store< uint32_t >( uint32_t &value, Mode::Mode mode );

template<> template<>
void SerialiseData< uint64_t >::Store< double >( double &value, Mode::Mode mode );

template<> template<>
void SerialiseData< double >::Store< uint64_t >( uint64_t &value, Mode::Mode mode );

#endif