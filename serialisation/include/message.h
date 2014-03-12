/**
 * Copyright (c) 2014 Mick van Duijn, Koen Visscher and Paul Visscher
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once
#ifndef __SERIALISATION_MESSAGE_H__
#define __SERIALISATION_MESSAGE_H__

#include "varIntData.h"
#include "repeatedData.h"
#include "stringData.h"
#include "numData.h"

#include <stdint.h>
#include <assert.h>
#include <fstream>
#include <map>

class ISerialisable;

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

    void Store( int8_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint8_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int16_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint16_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int32_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint32_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int64_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint64_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( float &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( double &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( std::string &string, const uint32_t index, const uint32_t = 0 );

    void Store( ISerialisable *const serialisable, uint32_t index = 0 );

    void Store( ISerialisable *const serialisable, const std::string &fileName );

    void Store( ISerialisable *const serialisable, std::ostream &stream );

    void Store( ISerialisable *const serialisable, std::istream &stream );

    uint32_t Count( const uint32_t index ) const;

    uint32_t GetMemberCount() const;

    void CreateRepeated( Type::Type type, uint32_t size, const uint32_t index = 0, const uint32_t flags = 0 );

    void StoreRepeated( uint8_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t = 0 );

    void StoreRepeated( int8_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t = 0 );

    void StoreRepeated( uint16_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( int16_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( uint32_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( int32_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( uint64_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( int64_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( float &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( double &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags = 0 );

    void StoreRepeated( std::string &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t = 0 );

    void StoreRepeated( ISerialisable *const serialisable, const uint32_t index, const uint32_t repeatedIndex,
                        const uint32_t = 0 );

    void WriteToFile( const std::string &fileName ) const;

    virtual void WriteToStream( std::ostream &stream ) const;

    void ReadFromFile( const std::string &fileName );

    virtual void ReadFromStream( std::istream &stream );

protected:

    template< typename T >
    T *CreateDataType()
    {
        return new T;
    }

    virtual Message *CreateMessage();

private:

    std::map< uint32_t, ISerialiseData * > mSerialisables;
    std::pair< uint32_t, ISerialiseData * > mCache;
    Mode::Mode mMode;

    template< typename T, typename DataType >
    void Store( T &value, uint32_t index )
    {
        DataType *const data = GetSerialisable< DataType >( index );

        if ( data )
        {
            data->Store( value, mMode );
        }
    }

    template< typename U, Type::Type T >
    void StoreUNum( U &val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            Store< U, VarIntData >( val, index );
        }
        else
        {
            Store< U, NumData< U > >( val, index );
        }
    }

    template< typename S, typename U, Type::Type T >
    void StoreSNum( S &val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            bool isSerialising = mMode == Mode::Serialise;

            U uVal = isSerialising ? Util::ZigZag< S, U >( val ) : 0;

            Store< U, VarIntData >( uVal, index );

            if ( !isSerialising )
            {
                val = Util::ZagZig< U, S >( uVal );
            }
        }
        else
        {
            Store< U, NumData< U > >( ( U & )val, index );
        }
    }

    template< typename DataType >
    ISerialiseData *FindSerialisable( const uint32_t index )
    {
        ISerialiseData *data = NULL;

        if ( mCache.first == index && mCache.second != NULL )
        {
            data = mCache.second;
        }
        else
        {
            typename std::map< uint32_t, ISerialiseData * >::iterator it = mSerialisables.find( index );

            if ( it != mSerialisables.end() )
            {
                data = it->second;

                mCache.first = index;
                mCache.second = data;
            }
        }

        if ( data && data->GetType() != Type::GetEnum< DataType >() )
        {
            assert( false );
            data = NULL;
        }

        return data;
    }

    template< typename DataType >
    DataType *GetSerialisable( const uint32_t index, DataType * ( Message::*creator )() = &Message::CreateDataType< DataType > )
    {
        ISerialiseData *data = FindSerialisable< DataType >( index );

        switch ( mMode )
        {
        case Mode::Serialise:
            {
                if ( !data )
                {
                    data = ( *this.*creator )();
                    mSerialisables[ index ] = data;

                    mCache.first = index;
                    mCache.second = data;
                }
            }
            break;

        case Mode::Deserialise:
            assert( data );
        }

        return static_cast< DataType *const >( data );
    }



    template< typename V, typename DataType >
    void StoreRepeated( V &value, const uint32_t index, const uint32_t repeatedIndex )
    {
        ISerialiseData *data = FindSerialisable< RepeatedData< DataType > >( index );

        AbstractRepeatedData *const repeated = static_cast< AbstractRepeatedData *const >( data );

        assert( repeated->GetSubType() == Type::GetEnum< DataType >() );

        static_cast< DataType *const >( repeated->GetSerialisable( repeatedIndex ) )->Store( value, mMode );
    }

    template< typename V >
    void StoreRepeatedUNum( V &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            StoreRepeated< V, VarIntData >( value, index, repeatedIndex );
        }
        else
        {
            StoreRepeated< V, NumData< V > >( value, index, repeatedIndex );
        }
    }

    template< typename S, typename U >
    void StoreRepeatedSNum( S &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            bool isSerialising = mMode == Mode::Serialise;

            U uVal = isSerialising ? Util::ZigZag< S, U >( value ) : 0;

            StoreRepeated< U, VarIntData >( uVal, index, repeatedIndex );

            if ( !isSerialising )
            {
                value = Util::ZagZig< U, S >( uVal );
            }
        }
        else
        {
            StoreRepeated< S, NumData< U > >( value, index, repeatedIndex );
        }
    }

    ISerialiseData *GetSerialisable( const uint32_t index, Type::Type type );

    AbstractRepeatedData *GetRepeated( const uint32_t index, Type::Type subType, uint32_t flags = 0 );
};

template<>
Type::Type Type::GetEnum< Message >();

#endif
