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
#ifndef __SERIALISATION_BINARYSERMESSAGE_H__
#define __SERIALISATION_BINARYSERMESSAGE_H__

#include "interface/abstractTempArray.h"
#include "interface/ISerialisable.h"
#include "interface/IMessage.h"

#include "streamBuffer.h"
#include "tempObject.h"
#include "arrayInfo.h"
#include "types.h"
#include "util.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <stack>

class BinarySerMessage
{
public:

    BinarySerMessage( StreamBuffer< SERIALISERS_BUFFERSIZE > &buffer )
        : mStreamBuffer( buffer ),
          mArrayInfo( Internal::Type::Terminator, 0 )
    {

    }

    inline void InitObject()
    {

    }

    inline void FinishObject()
    {
        WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
    }

    inline bool InitObject( uint8_t index )
    {
        WriteHeader( index, Internal::Type::Object );
        return true;
    }

    inline void FinishObject( uint8_t /*index*/ )
    {
        FinishObject();
    }

    inline void InitArrayObject()
    {

    }

    inline void FinishArrayObject()
    {
        FinishObject();
    }



    inline void ClearBuffers()
    {
        mStreamBuffer.FlushWriteBuffer();
    }

    template< typename TPrimitive >
    void Store( TPrimitive &value, uint8_t index )
    {
        const Internal::Type::Type type = Internal::Type::GetEnum< TPrimitive >();
        WriteHeader( index, type );
        WritePrimitive( value );
    }

    inline size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 )
    {
        const Internal::Type::Type iType = static_cast< Internal::Type::Type >( type );

        mArrayInfo.Set( iType, size );
        WriteHeader( index, Internal::Type::Array );
        WriteHeader( flags, iType );
        mStreamBuffer.WriteSize( size );

        return size;
    }

    template< typename TPrimitive >
    void StoreArrayItem( TPrimitive &value )
    {
        WritePrimitive( value );
    }

    template< typename TPrimitive >
    void StoreVector( std::vector< TPrimitive > &container, uint8_t index, uint8_t flags )
    {
        size_t size = container.size();
        CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< TPrimitive >() ), size, index, flags );
        mStreamBuffer.WriteBytes( &container.at( 0 ), size * sizeof( TPrimitive ) );
    }

private:

    StreamBuffer< SERIALISERS_BUFFERSIZE > &mStreamBuffer;

    ArrayInfo mArrayInfo;

    template< typename T >
    void WriteHeader( const T index, Internal::Type::Type type )
    {
        T header = Util::CreateHeader( index, ToBinaryType( type ) );
        WritePrimitive( header );
    }

    inline Internal::Type::Type ToBinaryType( const Internal::Type::Type type )
    {
        Internal::Type::Type binaryType = type;

        if ( Internal::Type::IsSignedInt( type ) )
        {
            binaryType = static_cast< Internal::Type::Type >( type - Internal::Type::SInt8 + Internal::Type::UInt8 );
        }
        else if ( type == Internal::Type::Float )
        {
            binaryType = Internal::Type::UInt32;
        }
        else if ( type == Internal::Type::Double )
        {
            binaryType = Internal::Type::UInt64;
        }

        return binaryType;
    }

    template< typename TPrimitive >
    void WritePrimitive( TPrimitive &value )
    {
        mStreamBuffer.WriteBytes( &value, sizeof( TPrimitive ) );
    }


    BinarySerMessage &operator=( const BinarySerMessage &bsm );
};

template<>
inline void BinarySerMessage::WritePrimitive( std::string &value )
{
    mStreamBuffer.WriteSize( value.length() );
    mStreamBuffer.WriteBytes( value.c_str(), value.length() );
}


template<>
inline void BinarySerMessage::StoreArrayItem< float >( float &value )
{
    uint32_t flexman = Util::FloatToUInt32( value );

    StoreArrayItem( flexman );
}

template<>
inline void BinarySerMessage::StoreArrayItem( double &value )
{
    uint64_t flexman = Util::DoubleToUInt64( value );

    StoreArrayItem( flexman );
}





template<>
inline void BinarySerMessage::StoreVector( std::vector< float > &container, uint8_t index, uint8_t flags )
{
    size_t size = container.size();
    CreateArray( Type::Float, size, index, flags );

    uint32_t intBuffer[ 128 ];

    for ( size_t i = 0; i < size; i += 128 )
    {
        size_t blockSize = static_cast< size_t >( size - i );
        blockSize = blockSize > 128 ? 128 : blockSize;
        float *firstFloat = &container[ i ];

        for ( size_t k = 0; k < blockSize; ++k )
        {
            intBuffer[ k ] = Util::FloatToUInt32( firstFloat[ k ] );
        }

        mStreamBuffer.WriteBytes( intBuffer, blockSize * sizeof( uint32_t ) );
    }
}

template<>
inline void BinarySerMessage::StoreVector( std::vector< double > &container, uint8_t index, uint8_t flags )
{
    size_t size = container.size();
    CreateArray( Type::Double, size, index, flags );

    uint64_t intBuffer[ 128 ];

    for ( size_t i = 0; i < size; i += 128 )
    {
        size_t blockSize = static_cast< size_t >( size - i );
        double *firstFloat = &container[ i ];

        for ( size_t k = 0; k < blockSize; ++k )
        {
            intBuffer[ k ] = Util::DoubleToUInt64( firstFloat[ k ] );
        }

        mStreamBuffer.WriteBytes( intBuffer, blockSize * sizeof( uint32_t ) );
    }
}





template<>
inline void BinarySerMessage::Store( std::string &value, uint8_t index )
{
    WriteHeader( index, Internal::Type::String );
    WritePrimitive( value );
}

template<>
inline void BinarySerMessage::Store( float &value, uint8_t index )
{
    uint32_t flexman = Util::FloatToUInt32( value );

    Store( flexman, index );
}

template<>
inline void BinarySerMessage::Store( double &value, uint8_t index )
{
    uint64_t flexman = Util::DoubleToUInt64( value );

    Store( flexman, index );
}


#endif
