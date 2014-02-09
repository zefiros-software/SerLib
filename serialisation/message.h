#pragma once
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <unordered_map>
#include "ISerialisable.h"
#include "serialiseData.h"
#include <assert.h>
#include <iostream>

class Message
    : public ISerialiseData
{
public:

    Message( Mode mode = Mode::Serialise );

    void SetMode( Mode mode );
    Mode GetMode() const;

    virtual size_t Size() const;

    virtual Type GetType() const;

    void Store( S8 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( U8 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( S16 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( U16 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( S32 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( U32 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( S64 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( U64 *const val, const U32 index = 0, const U32 flags = 0 );
    void Store( std::string *const string, const U32 index = 0, const U32 flags = 0 );

    void StoreObject( ISerialisable *const serialisable, U32 index = 0 );

    void CreateRepeated( Type type, U32 size, const U32 index = 0, const U32 flags = 0 );

    void WriteToFile( const std::string &fileName ) const;
    virtual void WriteToStream( std::ostream &stream ) const;

    void ReadFromFile( const std::string &fileName );
    virtual void ReadFromStream( std::istream &stream );

private:

    std::unordered_map< size_t, ISerialiseData * > mSerialisables;
    Mode mMode;

    template< typename ValueType, typename DataType, Type T >
    void StoreNum( ValueType *const val, const U32 index )
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
                ISerialiseData *const data = GetDeserialisable( index );

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

    template< typename U, typename DataType, Type T >
    void StoreUNum( U *const val, const U32 index, const U32 flags )
    {
        if ( flags & ( U32 )Flags::VarInt )
        {
            StoreUPacked< U >( val, index );
        }
        else
        {
            StoreNum< U, DataType, T >( val, index );
        }
    }

    template< typename S, typename U, typename DataType, Type T >
    void StoreSNum( S *const val, const U32 index, const U32 flags )
    {
        if ( flags & ( U32 )Flags::VarInt )
        {
            StoreSPacked< S, U >( val, index );
        }
        else
        {
            StoreNum< S, DataType, T >( val, index );
        }
    }

    template< typename U >
    void StoreUPacked( U *const val, const U32 index )
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
    void StoreSPacked( S *const val, const U32 index )
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

    template< typename DataType, Type T >
    DataType *const GetSerialisable( const U32 index )
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

    template< Type T >
    ISerialiseData *const GetDeserialisable( const U32 index )
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

    ISerialiseData *const GetSerialisable( const U32 index, Type type );

};

#endif