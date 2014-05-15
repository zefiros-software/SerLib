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
#include "util.h"

#include <vector>

class AbstractSerialiser;
class Message;

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

    typename std::vector< T > &GetValues()
	{
		return mValues;
	}

    inline void Store( std::string &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    };

    inline void Store( uint8_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( uint16_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( uint32_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( uint64_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( int8_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( int16_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( int32_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( int64_t &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( float &, const uint32_t, const Mode::Mode )
    {
        StoreError();
    }

    inline void Store( double &, const uint32_t, const Mode::Mode )
    {
        StoreError();
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
        return Internal::Type::GetEnum< T >();
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

    inline void StoreError() const
    {
        throw std::invalid_argument( "Invalid conversion" );
    }

    template< typename S >
    void StoreT( S &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        if ( mode == Mode::Serialise )
        {
            mValues[ repeatedIndex ] = ( T )value;
        }
        else
        {
            value = ( S )mValues[ repeatedIndex ];
        }
    }

    template< typename U, typename S >
    void ZigZagStore( S &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        if ( mFlags & Internal::Flags::Packed )
        {
            const bool isSerialising = mode == Mode::Serialise;

            U uVal = isSerialising ? Util::ZigZag< S, U >( value ) : 0;

            Store( uVal, repeatedIndex, mode );

            if ( !isSerialising )
            {
                value = Util::ZagZig< U, S >( uVal );
            }
        }
        else
        {
            StoreS< U >( value, repeatedIndex, mode );
        }
    }

    template< typename S, typename U >
    void ZagZigStore( U &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        if ( mFlags & Internal::Flags::Packed )
        {
            const bool isSerialising = mode == Mode::Serialise;

            S sVal = isSerialising ? Util::ZagZig< U, S >( value ) : 0;

            Store( sVal, repeatedIndex, mode );

            if ( !isSerialising )
            {
                value = Util::ZigZag< S, U >( sVal );
            }
        }
        else
        {
            StoreS< S >( value, repeatedIndex, mode );
        }
    }

    template< typename S, typename U >
    void StoreS( U &value, const uint32_t repeatedIndex, const Mode::Mode mode )
    {
        const bool isSerialising = mode == Mode::Serialise;

        S sVal = isSerialising ? static_cast< S >( value ) : 0;

        Store( sVal, repeatedIndex, mode );

        if ( !isSerialising )
        {
            value = ( U )sVal;
        }
    }
};



template<>
class RepeatedData< Message >
    : public AbstractRepeatedData
{
public:

    uint32_t GetFlags() const
    {
        return mFlags;
    }

    void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    uint32_t Count() const
    {
        return mMessages.size();
    }

    void Resize( const size_t size )
    {
        const size_t mesSize = mMessages.size();
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

    Internal::Type::Type GetSubType() const
    {
        return Internal::Type::Variable;
    }

    void SerialiseTo( AbstractSerialiser *const serialiser )
    {
        serialiser->Serialise( this );
    }

    void Dispose()
    {
        PoolHolder::Get().GetPool< RepeatedData< Message > >().Dispose( this );
	}

	inline void Store( std::string &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	};

	inline void Store( uint8_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( uint16_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( uint32_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( uint64_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( int8_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( int16_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( int32_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( int64_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( float &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void Store( double &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

protected:

    uint32_t mFlags;
	std::vector< Message * > mMessages;

	inline void StoreError() const
	{
		throw std::invalid_argument( "Invalid conversion" );
	}
};





template<>
inline void RepeatedData< std::string >::Store( std::string &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    if ( mode == Mode::Serialise )
    {
        mValues[ repeatedIndex ].assign( value );
    }
    else
    {
        value.assign( mValues[ repeatedIndex ] );
    }
}

template<>
inline void RepeatedData< uint8_t >::Store( uint8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint16_t >::Store( uint16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint32_t >::Store( uint32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int8_t >::Store( int8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}


template<>
inline void RepeatedData< int16_t >::Store( int16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int32_t >::Store( int32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int64_t >::Store( int64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< float >::Store( float &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< double >::Store( double &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreT( value, repeatedIndex, mode );
}



template<>
inline void RepeatedData< uint8_t >::Store( int8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint8_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint16_t >::Store( int16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint16_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint32_t >::Store( int32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint32_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( int64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint64_t >( value, repeatedIndex, mode );;
}

template<>
inline void RepeatedData< int8_t >::Store( uint8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< int8_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int16_t >::Store( uint16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZagZigStore< int16_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int32_t >::Store( uint32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZagZigStore< int32_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int64_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZagZigStore< int64_t >( value, repeatedIndex, mode );
}



template<>
inline void RepeatedData< uint8_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint8_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint16_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint16_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint32_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint32_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int8_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint8_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int16_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint16_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< int32_t >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint32_t >( value, repeatedIndex, mode );
}


template<>
inline void RepeatedData< uint64_t >::Store( uint8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint64_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( uint16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint64_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( uint32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    StoreS< uint64_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( int8_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint8_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( int16_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint16_t >( value, repeatedIndex, mode );
}

template<>
inline void RepeatedData< uint64_t >::Store( int32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    ZigZagStore< uint32_t >( value, repeatedIndex, mode );
}


template<>
inline void RepeatedData< uint32_t >::Store( float &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    const bool isSerialising = mode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;

    Store( flexman, repeatedIndex, mode );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

template<>
inline void RepeatedData< float >::Store( uint32_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    const bool isSerialising = mode == Mode::Serialise;

    float f = isSerialising ? Util::UInt32ToFloat( value ) : 0;

    Store( f, repeatedIndex, mode );

    if ( !isSerialising )
    {
        value = Util::FloatToUInt32( f );
    }
}

template<>
inline void RepeatedData< uint64_t >::Store( double &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    const bool isSerialising = mode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;

    Store( flexman, repeatedIndex, mode );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

template<>
inline void RepeatedData< double >::Store( uint64_t &value, const uint32_t repeatedIndex, const Mode::Mode mode )
{
    const bool isSerialising = mode == Mode::Serialise;

    double d = isSerialising ? Util::UInt64ToDouble( value ) : 0;

    Store( d, repeatedIndex, mode );

    if ( !isSerialising )
    {
        value = Util::DoubleToUInt64( d );
    }
}

#endif