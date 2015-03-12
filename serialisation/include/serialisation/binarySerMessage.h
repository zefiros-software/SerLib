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

    friend struct StoreHelper;

    BinarySerMessage( StreamBuffer< SERIALISERS_BUFFERSIZE > &buffer )
        : mStreamBuffer( buffer ),
          mArrayInfo( Internal::Type::Terminator, 0 )
    {

    }

    void InitObject()
    {

    }

    void FinishObject()
    {
        WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
    }

    void InitObject( uint8_t index )
    {
        WriteHeader( index, Internal::Type::Object );
    }

    void FinishObject( uint8_t /*index*/ )
    {
        FinishObject();
    }

    void InitArrayObject()
    {

    }

    void FinishArrayObject()
    {
        FinishObject();
    }



    void ClearBuffers()
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

    template<>
    void Store( std::string &value, uint8_t index )
    {
        WriteHeader( index, Internal::Type::String );
        WritePrimitive( value );
    }

    template<>
    void Store( float &value, uint8_t index )
    {
        const uint32_t flexman = Util::FloatToUInt32( value );

        Store( flexman, index );
    }

    template<>
    void Store( double &value, uint8_t index )
    {
        const uint64_t flexman = Util::DoubleToUInt64( value );

        Store( flexman, index );
    }

    size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 )
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

    template<>
    void StoreArrayItem< float >( float &value )
    {
        uint32_t flexman = Util::FloatToUInt32( value );

        StoreArrayItem( flexman );
    }

    template<>
    void StoreArrayItem( double &value )
    {
        uint64_t flexman = Util::DoubleToUInt64( value );

        StoreArrayItem( flexman );
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

    Internal::Type::Type ToBinaryType( const Internal::Type::Type type )
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

    template<>
    void WritePrimitive( std::string &value )
    {
        mStreamBuffer.WriteSize( value.length() );
        mStreamBuffer.WriteBytes( value.c_str(), value.length() );
    }


    BinarySerMessage &operator=( const BinarySerMessage &bsm );
};

#endif