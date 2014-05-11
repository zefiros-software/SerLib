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

#include "interface/abstractRepeatedData.h"

#include "poolHolder.h"

#include <stdint.h>
#include <assert.h>
#include <fstream>
#include <vector>
#include <map>

class AbstractSerialiser;
class ISerialisable;

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

    Message( ISerialisable *const serialisable, Mode::Mode mode = Mode::Serialise );

    ~Message();

    void SetMode( Mode::Mode mode );

    Mode::Mode GetMode() const;

    virtual uint32_t GetFlags() const;

    virtual void SetFlags( const uint32_t flags );

    virtual Internal::Type::Type GetType() const;

    void Store( ISerialisable *const value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( std::string &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( uint8_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( uint16_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( uint32_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( uint64_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( int8_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( int16_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( int32_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( int64_t &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( float &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( double &value, const uint32_t index, const uint32_t flags = 0x00 );

    void Store( ISerialisable *const serialisable );

    uint32_t Count( const uint32_t index );

    uint32_t GetMemberCount() const;

    void CreateRepeated( Type::Type type, uint32_t size, const uint32_t index, const uint32_t flags = 0x00 );

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

    uint32_t mMemberCount;
    uint32_t mFlags;
    Mode::Mode mMode;

    virtual void SerialiseTo( AbstractSerialiser *const serialiser );

    virtual void Dispose();
    
    void Store( ISerialisable *const value, const Mode::Mode mode );


    template< typename T >
    T *CreateDataType( const uint32_t flags )
    {
        T *object = PoolHolder::Get().GetPool< T >().Get();
        object->SetFlags( flags );
        return object;
    }

    template< typename T >
    void DisposeDataType( T *object )
    {
        object->SetFlags( 0x00 );
        PoolHolder::Get().GetPool< T >().Dispose( object );
    }

    template< typename DataType, typename T >
    void Store( T &value, const uint32_t index, const uint32_t flags )
    {
        ISerialiseData *data = FindSerialisable( index );

        if ( !data )
        {
            data = CreateDataType< DataType >( flags );
            InsertSerialiseDataAt( data, index );
        }

        static_cast< IPrimitiveData *const >( data )->Store( value, mMode );
    }

    void InsertSerialiseDataAt( ISerialiseData *const data, const uint32_t index )
    {
        size_t size = mSerialisables.size();

        if ( index >= size )
        {
            const size_t newSize = size + 10;
            mSerialisables.resize( newSize >= index ? newSize : index );
        }

        size = mIndexes.size();

        if ( ++mMemberCount == size )
        {
            mIndexes.reserve( size + 10 );
        }

        mSerialisables[ index ] = data;
        mIndexes.push_back( index );
    }

    inline ISerialiseData *FindSerialisable( const uint32_t index )
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

        static_cast< IPrimitiveData * >(
            static_cast< AbstractRepeatedData * >( data )->GetSerialisable( repeatedIndex ) )->Store( value, mMode );
    }
};

#endif
