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

    Message( Mode::Mode mode = Mode::Serialise );

    void SetMode( Mode::Mode mode );
    Mode::Mode GetMode() const;

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
    Mode::Mode mMode;

    template< typename U, typename DataType, Type::Type T >
    void StoreUNum( U &val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Flags::Packed )
        {
            VarIntSerialiseData *const data = GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );

            if ( data )
            {
                data->Store( val, mMode );
            }
        }
        else
        {
            DataType *const data = GetSerialisable< DataType, T >( index );

            if ( data )
            {
                data->Store( val, mMode );
            }
        }
    }

    template< typename S, typename U, typename DataType, Type::Type T >
    void StoreSNum( S &val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Flags::Packed )
        {
            VarIntSerialiseData *const data = GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );

            if ( data )
            {
                bool isSerialising = mMode == Mode::Serialise;

                U uVal = isSerialising ? Util::ZigZag< S, U >( val ) : 0;
                data->Store( uVal, mMode );

                if ( !isSerialising )
                {
                    val = Util::ZagZig< U, S >( uVal );
                }
            }
        }
        else
        {
            DataType *const data = GetSerialisable< DataType, T >( index );

            if ( data )
            {
                data->Store( val, mMode );
            }
        }
    }

    template< typename DataType, Type::Type T >
    DataType *const GetSerialisable( const uint32_t index )
    {
        ISerialiseData *data = NULL;

        auto it = mSerialisables.find( index );

        if ( it != mSerialisables.end() )
        {
            data = it->second;

            assert( data->GetType() == T );
        }

        switch ( mMode )
        {
        case Mode::Serialise:
            {
                if ( data && data->GetType() != T )
                {
                    delete data;
                    data = new DataType();
                    mSerialisables.erase( it );
                    mSerialisables[ index ] = data;
                }
                else if ( !data )
                {
                    data = new DataType();
                    mSerialisables[ index ] = data;
                }
            }
            break;

        case Mode::Deserialise:
            assert( data );
        }

        return static_cast< DataType *const >( data );
    }

    ISerialiseData *const GetSerialisable( const uint32_t index, Type::Type type );
};

#endif