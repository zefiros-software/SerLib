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

#include <stdint.h>
#include <assert.h>
#include <fstream>
#include <map>

class ISerialisable;
class AbstractSerialiser;

class Message
    : public ISerialiseData
{
	friend class AbstractSerialiser;
	friend class AbstractDeserialiser;

public:

    enum Flags
    {
        Packed = 0x01
    };

    Message( Mode::Mode mode = Mode::Serialise );

	~Message();

    void SetMode( Mode::Mode mode );

    Mode::Mode GetMode() const;

    virtual uint32_t GetFlags() const;

    virtual void SetFlags( const uint32_t flags );

    virtual Internal::Type::Type GetType() const;

    void Store( ISerialisable *const value, const uint32_t index, const uint32_t flags = 0 );

    void Store( std::string &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint8_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint16_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint32_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( uint64_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int8_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int16_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int32_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( int64_t &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( float &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( double &value, const uint32_t index, const uint32_t flags = 0 );

    void Store( ISerialisable *const serialisable, const std::string &fileName );

    void Store( ISerialisable *const serialisable, std::ostream &stream );

    void Store( ISerialisable *const serialisable, std::istream &stream );

    uint32_t Count( const uint32_t index );

    uint32_t GetMemberCount() const;

    void CreateRepeated( Type::Type type, uint32_t size, const uint32_t index, const uint32_t flags = 0 );

    void StoreRepeated( ISerialisable *const value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( std::string &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( uint8_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( uint16_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( uint32_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( uint64_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( int8_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( int16_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( int32_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( int64_t &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( float &value, const uint32_t index, const uint32_t repeatedIndex );

    void StoreRepeated( double &value, const uint32_t index, const uint32_t repeatedIndex );

protected:

    std::vector< uint32_t > mIndexes;
    std::vector< ISerialiseData * > mSerialisables;

    std::pair< uint32_t, ISerialiseData * > mCache;
    uint32_t mFlags;
    Mode::Mode mMode;

    virtual void SerialiseTo( AbstractSerialiser *const serialiser );

    void Store( ISerialisable *const value, Mode::Mode mode );

    template< typename T >
    T *CreateDataType( const uint32_t flags )
    {
        return new T( flags );
    }

    template<>
    Message *CreateDataType< Message >( const uint32_t )
    {
        return CreateMessage();
    }

    virtual Message *CreateMessage();

    template< typename DataType, typename T >
    void Store( T &value, const uint32_t index, const uint32_t flags )
    {
        ISerialiseData *data = FindSerialisable( index );

        if ( !data )
        {
            data = CreateDataType< DataType >( flags );
            InsertSerialiseDataAt( data, index );
        }

        StoreToSerialiseData( data, value );
    }

    void InsertSerialiseDataAt( ISerialiseData *const data, const uint32_t index )
    {
        size_t size = mSerialisables.size();

        if ( index >= size )
        {
            size_t newSize = size + 10;
            mSerialisables.resize( newSize >= index ? newSize : index );
        }

        mIndexes.push_back( index );
        mSerialisables[ index ] =  data;
    }

    void SetCache( ISerialiseData *const data, const uint32_t index )
    {
        mCache.first = index;
        mCache.second = data;
    }

    template< typename T >
    void StoreToSerialiseData( ISerialiseData *const data, T &value )
    {
        switch ( data->GetType() )
        {
        case Internal::Type::Repeated:
        case Internal::Type::Variable:
            static_cast< Message *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::String:
            static_cast< SerialiseData< std::string > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::UInt8:
            static_cast< SerialiseData< uint8_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::UInt16:
            static_cast< SerialiseData< uint16_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::UInt32:
            static_cast< SerialiseData< uint32_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::UInt64:
            static_cast< SerialiseData< uint64_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::VarInt:
            static_cast< VarIntData *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::SInt8:
            static_cast< SerialiseData< int8_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::SInt16:
            static_cast< SerialiseData< int16_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::SInt32:
            static_cast< SerialiseData< int32_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::SInt64:
            static_cast< SerialiseData< int64_t > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::Float:
            static_cast< SerialiseData< float > *const >( data )->Store( value, mMode );
            break;

        case Internal::Type::Double:
            static_cast< SerialiseData< double > *const >( data )->Store( value, mMode );
            break;
        }
    }

    ISerialiseData *FindSerialisable( const uint32_t index )
    {
        if ( index < mSerialisables.size() )
        {
            return mSerialisables[ index ];
        }

        return NULL;
    }

    template< typename V >
    void StoreRepeatedV( V &value, const uint32_t index, const uint32_t repeatedIndex )
    {
        ISerialiseData *data = FindSerialisable( index );

        assert( data->GetType() == Internal::Type::Repeated );

        StoreToSerialiseData( static_cast< AbstractRepeatedData *const >( data )->GetSerialisable( repeatedIndex ), value );
    }
};

#endif
