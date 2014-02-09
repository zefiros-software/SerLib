#pragma once
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "ISerialisable.h"
#include "serialiseData.h"
#include "macro.h"

#ifdef CPP11
#include <unordered_map>
#else
#include <map>
#endif

#include <assert.h>
#include <iostream>

class Message
    : public ISerialiseData
{
public:

    enum Flags
    {
        Packed = 0x01
    };

    enum Mode
    {
        Serialise = 0x00,
        Deserialise = 0x01
    };

    Message( Mode mode = Mode::Serialise );

    void SetMode( Mode mode );
    Mode GetMode() const;

    virtual size_t Size() const;

    virtual Type::Type GetType() const;

    void Store( int8_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( uint8_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( int16_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( uint16_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( int32_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( uint32_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( int64_t &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( uint64_t &value, const uint32_t index = 0, const uint32_t flags = 0 );

    void Store( float &value, const uint32_t index = 0, const uint32_t flags = 0 );
    void Store( double &value, const uint32_t index = 0, const uint32_t flags = 0 );

    void Store( std::string &string, const uint32_t index = 0, const uint32_t flags = 0 );

    void StoreObject( ISerialisable *const serialisable, uint32_t index = 0 );

    void CreateRepeated( Type::Type type, uint32_t size, const uint32_t index = 0, const uint32_t flags = 0 );

    void WriteToFile( const std::string &fileName ) const;
    virtual void WriteToStream( std::ostream &stream ) const;

    void ReadFromFile( const std::string &fileName );
    virtual void ReadFromStream( std::istream &stream );

private:

#ifdef CPP11
    typedef std::unordered_map< size_t, ISerialiseData * > Map;
#else
    typedef std::map< size_t, ISerialiseData * > Map;
#endif


    Map mSerialisables;
    Mode mMode;

    template< typename ValueType, typename DataType, Type::Type T >
    void StoreNum( ValueType *const val, const uint32_t index )
    {
        switch ( mMode )
        {
        case Mode::Serialise:
            {
                DataType *data = GetSerialisable< DataType, T >( index );

                data->SetValue( *val );
            }
            break;

        case Mode::Deserialise:
            {
                ISerialiseData *const data = GetDeserialisable< T >( index );

                if ( !data )
                {
                    assert( false );
                    return;
                }

                *val = static_cast< DataType *const >( data )->GetValue();
            }
            break;
        }
    }

    template< typename U, typename DataType, Type::Type T >
    void StoreUNum( U *const val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Flags::Packed )
        {
            StoreUPacked< U >( val, index );
        }
        else
        {
            StoreNum< U, DataType, T >( val, index );
        }
    }

    template< typename S, typename U, typename DataType, Type::Type T >
    void StoreSNum( S *const val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Flags::Packed )
        {
            StoreSPacked< S, U >( val, index );
        }
        else
        {
            StoreNum< S, DataType, T >( val, index );
        }
    }

    template< typename U >
    void StoreUPacked( U *const val, const uint32_t index )
    {
        switch ( mMode )
        {
        case Mode::Serialise:
            {
                VarIntSerialiseData *data = GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );

                data->SetValue< U >( *val );
            }
            break;

        case Mode::Deserialise:
            {
                ISerialiseData *const data = GetDeserialisable< Type::VarInt >( index );

                if ( !data )
                {
                    assert( false );
                    return;
                }

                *val = static_cast< VarIntSerialiseData *const >( data )->GetValue< U >();
            }
            break;
        }
    }

    template< typename S, typename U >
    void StoreSPacked( S *const val, const uint32_t index )
    {
        switch ( mMode )
        {
        case Mode::Serialise:
            {
                U uVal = Util::ZigZag< S, U >( *val );
                StoreUPacked< U >( &uVal, index );
            }
            break;

        case Mode::Deserialise:
            {
                ISerialiseData *const data = GetDeserialisable< Type::VarInt >( index );

                if ( !data )
                {
                    assert( false );
                    return;
                }

                *val = Util::ZagZig< U, S >( static_cast< VarIntSerialiseData *const >( data )->GetValue< U >() );
            }
            break;
        }
    }

    template< typename DataType, Type::Type T >
    DataType *const GetSerialisable( const uint32_t index )
    {
        DataType *data = NULL;

        auto it = mSerialisables.find( index );

        if ( it != mSerialisables.end() )
        {
            ISerialiseData *const sData = it->second;

            if ( sData->GetType() == T )
            {
                data = static_cast< DataType *const >( sData );
            }
            else
            {
                assert( false );
                delete sData;
                data = new DataType();
                mSerialisables.erase( it );
                mSerialisables[ index ] = data;
            }
        }
        else
        {
            data = new DataType();
            mSerialisables[ index ] = data;
        }

        return data;
    }

    template< Type::Type T >
    ISerialiseData *const GetDeserialisable( const uint32_t index )
    {
        ISerialiseData *data = NULL;
        auto it = mSerialisables.find( index );

        if ( it != mSerialisables.end() )
        {
            data = it->second;

            assert( data->GetType() == T );
        }

        return data;
    }

    ISerialiseData *const GetSerialisable( const uint32_t index, Type::Type type );
};

#endif