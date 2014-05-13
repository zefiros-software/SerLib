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
#ifndef __SERIALISATION_REPEATEDDATA_H__
#define __SERIALISATION_REPEATEDDATA_H__

#include "interface/abstractRepeatedData.h"
#include "interface/ISerialisable.h"

#include "serialiseData.h"
#include "poolHolder.h"

#include <vector>

class AbstractSerialiser;

template< typename T >
class RepeatedData
    : public AbstractRepeatedData
{
public:

    RepeatedData( const uint32_t flags = 0x00 )
        : mFlags( flags )
    {
    }

    ~RepeatedData()
    {
    }

    void Store( T &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        if ( mode == Mode::Serialise )
        {
            mValues[ repeatedIndex ] = value;
        }
        else
        {
            value = mValues[ repeatedIndex ];
        }
    }

    void StoreVector( typename std::vector< T > &vector, const Mode::Mode mode )
    {
        if ( mode == Mode::Serialise )
        {
            size_t size = vector.size();
            mValues.resize( size );
            memcpy( &mValues.front(), &vector.front(), size * sizeof( T ) );
        }
        else
        {
            size_t size = mValues.size();
            vector.resize( size );
            memcpy( &vector.front(), &mValues.front(), size * sizeof( T ) );
        }
    }

    virtual uint32_t GetFlags() const
    {
        return mFlags;
    }

    virtual void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    virtual Internal::Type::Type GetSubType() const
    {
        return Internal::Type::GetEnum< SerialiseData< T > >();
    }

    virtual void Resize( const size_t size )
    {
        mValues.resize( size );
    }

    virtual uint32_t Count() const
    {
        return ( uint32_t )mValues.size();
    }

    virtual void SerialiseTo( AbstractSerialiser *const serialiser )
    {
        serialiser->Serialise( this );
    }

    void Dispose()
    {
        PoolHolder::Get().GetPool< RepeatedData< T > >().Dispose( this );
    }

protected:

    std::vector< T > mValues;
    uint32_t mFlags;
};

template<>
class RepeatedData< Message >
{
public:

    RepeatedData( const uint32_t flags = 0x00 )
        : mFlags( flags )
    {
    }

    ~RepeatedData()
    {
        DisposeMessages();
    }

    void Store( ISerialisable &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        Message *const message = mMessages[ repeatedIndex ];
        message->SetMode( mode );
        value.SERIALISATION_CUSTOM_INTERFACE( *message );
    }

    void StoreVector( std::vector< ISerialisable > &vector, const Mode::Mode mode )
    {
        for ( uint32_t i = 0, end = vector.size(); i < end; ++i )
        {
            Store( vector[ i ], i, mode );
        }
    }

    virtual uint32_t GetFlags() const
    {
        return mFlags;
    }

    virtual void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    virtual Internal::Type::Type GetSubType() const
    {
        return Internal::Type::Variable;
    }

    virtual void Resize( const size_t size )
    {
        size_t mesSize = mMessages.size();
        ObjectPool< Message > &pool = PoolHolder::Get().GetPool< Message >();

        if ( size > mesSize )
        {
            pool.GetMultiple( mMessages, size - mesSize );
        }
        else
        {
            pool.DisposeMultiple( mMessages, mMessages.begin() + mesSize, mMessages.end() );
        }
    }

    virtual uint32_t Count() const
    {
        return ( uint32_t )mMessages.size();
    }

    virtual void SerialiseTo( AbstractSerialiser *const serialiser )
    {
        serialiser->Serialise( this );
    }

    void Dispose()
    {
        DisposeMessages();

        PoolHolder::Get().GetPool< RepeatedData< Message > >().Dispose( this );
    }

    void DisposeMessages()
    {
        ObjectPool< Message > &pool = PoolHolder::Get().GetPool< Message >();

        pool.DisposeMultiple( mMessages, mMessages.begin(), mMessages.end() );
    }

protected:

    std::vector< Message * > mMessages;
    uint32_t mFlags;

};

#endif