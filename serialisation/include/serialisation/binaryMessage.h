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
#ifndef __SERIALISATION_BINARYMESSAGE_H__
#define __SERIALISATION_BINARYMESSAGE_H__

#include "interface/ISerialisable.h"
#include "interface/IMessage.h"

#include "streamBuffer.h"
#include "arrayInfo.h"
#include "types.h"
#include "util.h"

#include <assert.h>
#include <iostream>
#include <fstream>

class BinarySerMessage
	: public IMessage
{
public:

	friend struct StoreHelper;

    BinarySerMessage( std::iostream &stream )
        : mStreamBuffer( stream ),
          mArrayInfo( Internal::Type::Terminator, 0 )
    {

    }

    BinarySerMessage( const std::string &fileName )
        : mStreamBuffer( fileName ),
          mArrayInfo( Internal::Type::Terminator, 0 )
    {

	}

	void ClearBuffers()
	{
		mStreamBuffer.FlushWriteBuffer();
	}

    template< typename TSerialisable >
    void Store( TSerialisable &value, uint8_t index )
    {
        WriteHeader( index, Internal::Type::Object );

        Store( value );
    }

    void Store( std::string &value, uint8_t index )
    {
        WriteHeader( index, Internal::Type::String );
        WriteString( value );
    }

    void Store( uint8_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( uint16_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( uint32_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( uint64_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( int8_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( int16_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( int32_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( int64_t &value, uint8_t index )
    {
        StorePrimitive( value, index );
    }

    void Store( float &value, uint8_t index )
    {
        const uint32_t flexman = Util::FloatToUInt32( value );

        StorePrimitive( flexman, index );
    }

    void Store( double &value, uint8_t index )
    {
        const uint64_t flexman = Util::DoubleToUInt64( value );

        StorePrimitive( flexman, index );
    }

//     void Store( ISerialisable &serialisable )
//     {
//         Store< ISerialisable >( serialisable );
//     }

    template< typename TSerialisable >
    void Store( TSerialisable &value )
    {
        value.SERIALISATION_CUSTOM_INTERFACE( *this );

        WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
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

	template< typename TSerialisable >
    void StoreArrayItem( TSerialisable &value )
    {
        Store( value );
    }

    void StoreArrayItem( std::string &value )
    {
        WriteString( value );
    }

    void StoreArrayItem( uint8_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( uint16_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( uint32_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( uint64_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( int8_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( int16_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( int32_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( int64_t &value )
    {
        WritePrimitive( value );
    }

    void StoreArrayItem( float &value )
    {
        uint32_t flexman = Util::FloatToUInt32( value );

        WritePrimitive( flexman );
    }

    void StoreArrayItem( double &value )
    {
        uint64_t flexman = Util::DoubleToUInt64( value );

        WritePrimitive( flexman );
    }

private:

    StreamBuffer< SERIALISERS_BUFFERSIZE > mStreamBuffer;

    ArrayInfo mArrayInfo;

    template< typename T >
    void WriteHeader( const T index, Internal::Type::Type type )
    {
        if ( Internal::Type::IsSignedInt( type ) )
        {
            type = static_cast< Internal::Type::Type >( type - Internal::Type::SInt8 + Internal::Type::UInt8 );
        }
        else if ( type == Internal::Type::Float )
        {
            type = Internal::Type::UInt32;
        }
        else if ( type == Internal::Type::Double )
        {
            type = Internal::Type::UInt64;
        }

        T header = Util::CreateHeader( index, type );
        WritePrimitive( header );
    }

    template< typename TPrimitive >
    void StorePrimitive( TPrimitive &value, uint8_t index )
    {
        const Internal::Type::Type type = Internal::Type::GetEnum< TPrimitive >();
        WriteHeader( index, type );
        WritePrimitive( value );
    }

    template< typename TPrimitive >
    void WritePrimitive( TPrimitive &value )
    {
        mStreamBuffer.WriteBytes( &value, sizeof( TPrimitive ) );
    }

    void WriteString( std::string &value )
    {
        mStreamBuffer.WriteSize( value.length() );
        mStreamBuffer.WriteBytes( value.c_str(), value.length() );
    }
};

#endif