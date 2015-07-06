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
#ifndef __SERIALISATION_BINARYSERHEADERMESSAGE_H__
#define __SERIALISATION_BINARYSERHEADERMESSAGE_H__

#include "serialisation/interface/abstractTempArray.h"

#include "serialisation/bufferedStreamWriter.h"
#include "serialisation/arrayInfo.h"
#include "serialisation/util.h"

class BinarySerMessage;

class BinarySerHeaderMessage
{
public:

    explicit BinarySerHeaderMessage( const std::string &fileName )
        : mStreamBuffer( fileName )
    {
    }

    explicit BinarySerHeaderMessage( std::ofstream &stream )
        : mStreamBuffer( stream )
    {
    }

    explicit BinarySerHeaderMessage( std::fstream &stream )
        : mStreamBuffer( stream )
    {
    }

    explicit BinarySerHeaderMessage( std::ostream &stream )
        : mStreamBuffer( stream )
    {
    }

    inline void InitObject()
    {
    }

    inline void FinishObject()
    {
        WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
    }

    inline bool InitObject( uint8_t /*index*/ )
    {
        return false;
    }

    inline void FinishObject( uint8_t /*index*/ )
    {
    }

    inline bool InitParent( uint8_t /*index*/ )
    {
        return false;
    }

    inline void FinishParent( uint8_t /*index*/ )
    {
    }

    inline void InitArrayObject()
    {
    }

    inline void FinishArrayObject()
    {
    }

    inline void ClearBuffer()
    {
        mStreamBuffer.ClearBuffer();
    }

    template< typename TPrimitive >
    void Store( TPrimitive &/*value*/, uint8_t /*index*/ )
    {
    }

    inline size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 )
    {
        const Internal::Type::Type iType = static_cast< Internal::Type::Type >( type );

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
    void StoreContiguous( TPrimitive *begin, size_t size )
    {
        mStreamBuffer.WritePrimitiveBlock( begin, size );
    }

private:

    uint64_t mU64Buffer[ 128 ];
    uint32_t mU32Buffer[ 128 ];

    BufferedStreamWriter mStreamBuffer;

    ArrayInfo mArrayInfo;

    template< typename T >
    void WriteHeader( const T index, Internal::Type::Type type )
    {
        assert( type != Type::Object && "No objects are allowed as member within static objects" );
        const T header = Util::CreateHeader( index, ToBinaryType( type ) );

        mStreamBuffer.WritePrimitive( header );
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

    BinarySerHeaderMessage &operator=( const BinarySerHeaderMessage &bsm );
};

#endif