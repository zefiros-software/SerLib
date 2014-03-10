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
#ifndef __SERIALIZATION_MESSAGE_H__
#define __SERIALIZATION_MESSAGE_H__

#include "varIntSerialiseData.h"
#include "repeatedData.h"
#include "stringData.h"
#include "numData.h"
#include "util.h"

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

    void Store( std::string &string, const uint32_t index = 0, const uint32_t = 0 );

    void Store( ISerialisable *const serialisable, uint32_t index = 0 );

    void Store( ISerialisable *const serialisable, const std::string &fileName );

    template< typename S >
    void Store( ISerialisable *const serialisable, S &stream )
    {
        Store( serialisable, 0 );

        if ( mMode == Mode::Serialise )
        {
            WriteToStream( stream );
        }
        else
        {
            ReadFromStream( stream );
        }
    }

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

    typedef NumData< uint8_t, Type::Char >      CharSerialiseData;
    typedef NumData< uint16_t, Type::WORD >     WORDSerialiseData;
    typedef NumData< uint32_t, Type::DWORD >    DWORDSerialiseData;
    typedef NumData< uint64_t, Type::QWORD >    QWORDSerialiseData;

    typedef RepeatedData< Message, Type::Message >              RepeatedMessage;
    typedef RepeatedData< StringData, Type::String >            RepeatedStringData;
    typedef RepeatedData< CharSerialiseData, Type::Char >       RepeatedCharData;
    typedef RepeatedData< WORDSerialiseData, Type::WORD >       RepeatedWORDData;
    typedef RepeatedData< DWORDSerialiseData, Type::DWORD >     RepeatedDWORDData;
    typedef RepeatedData< QWORDSerialiseData, Type::QWORD >     RepeatedQWORDData;
    typedef RepeatedData< VarIntSerialiseData, Type::VarInt >   RepeatedVarIntData;

    std::map< uint32_t, ISerialiseData * > mSerialisables;
    Mode::Mode mMode;

    template< typename U, typename DataType, Type::Type T >
    void StoreUNum( U &val, const uint32_t index, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
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
        if ( flags & ( uint32_t )Packed )
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
    DataType *GetSerialisable( const uint32_t index,
                               DataType * ( Message::*creator )() = &Message::CreateDataType< DataType > )
    {
        ISerialiseData *data = NULL;

        typename std::map< uint32_t, ISerialiseData * >::iterator it = mSerialisables.find( index );

        if ( it != mSerialisables.end() )
        {
            data = it->second;

            assert( data->GetType() == T );
        }

        switch ( mMode )
        {
        case Mode::Serialise:
            {
                if ( !data )
                {
                    data = ( *this.*creator )();
                    mSerialisables[ index ] = data;
                }
            }
            break;

        case Mode::Deserialise:
            assert( data );
        }

        return static_cast< DataType *const >( data );
    }

    template< typename V, typename DataType, Type::Type T >
    void StoreRepeated( V &value, const uint32_t index, const uint32_t repeatedIndex )
    {
        typename std::map< uint32_t, ISerialiseData * >::iterator it = mSerialisables.find( index );
        assert( it != mSerialisables.end() );

        ISerialiseData *const data = it->second;

        assert( data->GetType() == Type::Repeated );

        AbstractRepeatedData *const repeated = static_cast< AbstractRepeatedData *const >( data );

        assert( repeated->GetSubType() == T );


        static_cast< DataType *const >( repeated->GetSerialisable( repeatedIndex ) )->Store( value, mMode );
    }

    template< typename V, typename DataType, Type::Type T >
    void StoreRepeatedUNum( V &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            StoreRepeated< V, VarIntSerialiseData, Type::VarInt >( value, index, repeatedIndex );
        }
        else
        {
            StoreRepeated< V, DataType, T >( value, index, repeatedIndex );
        }
    }

    template< typename S, typename U, typename DataType, Type::Type T >
    void StoreRepeatedSNum( S &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags )
    {
        if ( flags & ( uint32_t )Packed )
        {
            bool isSerialising = mMode == Mode::Serialise;

            U uVal = isSerialising ? Util::ZigZag< S, U >( value ) : 0;
            StoreRepeated< U, VarIntSerialiseData, Type::VarInt >( uVal, index, repeatedIndex );

            if ( !isSerialising )
            {
                value = Util::ZagZig< U, S >( uVal );
            }
        }
        else
        {
            StoreRepeated< S, DataType, T >( value, index, repeatedIndex );
        }
    }

    ISerialiseData *GetSerialisable( const uint32_t index, Type::Type type );

    AbstractRepeatedData *GetRepeated( const uint32_t index, Type::Type subType, uint32_t flags = 0 );
};

#endif
