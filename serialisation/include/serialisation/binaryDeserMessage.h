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
#ifndef __SERIALISATION_BINARYDESERMESSAGE_H__
#define __SERIALISATION_BINARYDESERMESSAGE_H__


#include "interface\abstractTempArray.h"

#include "streamBuffer.h"
#include "tempObject.h"
#include "arrayInfo.h"
#include "defines.h"
#include "types.h"

#include <assert.h>
#include <stack>

class BinaryDeserMessage
{
public:

    BinaryDeserMessage( StreamBuffer<SERIALISERS_BUFFERSIZE > &buffer )
        : mStreamBuffer( buffer ),
          mArrayInfo( Internal::Type::Terminator, 0 ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {

    }

    void InitObject()
    {
        if ( mCurrentObject )
        {
            mBufferUsedHistory.push( true );
            mBufferHistory.push( mCurrentObject );
        }
        else
        {
            mBufferUsedHistory.push( false );
        }
    }

    void FinishObject()
    {
        if ( !mTerminatorRead )
        {
            ReadAll();
        }

        if ( mCurrentArray )
        {
            DeleteTempData( mCurrentArray );
        }

        if ( mCurrentObject )
        {
            for ( size_t i = 0, size = mCurrentObject->GetSize(); i < size; ++i )
            {
                DeleteTempData( mCurrentObject->PopBack() );
            }

            DeleteTempData( mCurrentObject );
            mCurrentObject = NULL;
        }

        if ( mBufferUsedHistory.top() )
        {
            mCurrentObject = mBufferHistory.top();
            mBufferHistory.pop();
        }

        mBufferUsedHistory.pop();
        mTerminatorRead = false;
    }

    void InitObject( uint8_t index )
    {
        const Internal::Type::Type type = ReadUntil( index );

        if ( !mTerminatorRead )
        {
            assert( type == Internal::Type::Object );
            InitObject();
        }
    }

    void FinishObject( uint8_t /*index*/ )
    {
        FinishObject();
    }

    void InitArrayObject()
    {
        InitObject();
    }

    void FinishArrayObject()
    {
        FinishObject();
    }

    template< typename TPrimitive >
    void Store( TPrimitive &value, uint8_t index )
    {
        ITempData *const data = mCurrentObject ? mCurrentObject->TryRemoveData( index ) : NULL;

        if ( data )
        {
            ReadFromTemp( value, data );
            DeleteTempData( data );
        }
        else
        {
            ReadValue( value, index );
        }
    }

    size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags )
    {
        if ( mCurrentArray )
        {
            DeleteTempData( mCurrentArray );
            mCurrentArray = NULL;
        }

        AbstractTempArray *temp = mCurrentObject ? static_cast< AbstractTempArray * >( mCurrentObject->TryRemoveData( index ) ) : NULL;

        if ( temp )
        {
            mCurrentArray = temp;
            mArrayInfo.Set( temp->GetSubType(), temp->GetRemainingCount() );
        }
        else
        {
            const Internal::Type::Type readType = ReadUntil( index );

            if ( !mTerminatorRead )
            {
                assert( readType == Internal::Type::Array );
                Internal::Type::Type rType;
                ReadHeader( flags, rType );
                assert( Internal::Type::AreCompatible( rType, static_cast< Internal::Type::Type >( type ) ) );
                size = mStreamBuffer.ReadSize();
                mArrayInfo.Set( rType, size );
            }
        }

        return size;
    }

    template< typename TPrimitive >
    void StoreArrayItem( TPrimitive &value )
    {
        if ( mCurrentArray )
        {
            static_cast< TempArray< TPrimitive > * >( mCurrentArray )->PopFront( value );
        }
        else
        {
            ReadFromStream( value );
        }

        --mArrayInfo.remainingCount;
    }

private:

    StreamBuffer< SERIALISERS_BUFFERSIZE > &mStreamBuffer;

    std::stack< TempObject * > mBufferHistory;
    std::stack< bool > mBufferUsedHistory;

    ArrayInfo mArrayInfo;

    AbstractTempArray *mCurrentArray;
    TempObject *mCurrentObject;

    bool mTerminatorRead;

    template< typename TPrimitive >
    void ReadFromTemp( TPrimitive &value, ITempData *data )
    {
        value = static_cast< TempPrimitive< TPrimitive > * >( data )->GetValue();
    }

    template< typename T >
    void ReadHeader( T &index, Internal::Type::Type &type )
    {
        T header;
        ReadFromStream( header );
        index = Util::GetHeaderIndex( header );
        type = Util::GetHeaderType( header );
    }

    inline ITempData *ReadTempObjectArray( size_t size )
    {
        TempArray< TempObject * > *temp = CreateTempData< TempArray< TempObject * > >();

        InitObject();

        for ( uint32_t i = 0; i < size; ++i )
        {
            mCurrentObject = CreateTempData< TempObject >();
            ReadAll();
            temp->PushBack( mCurrentObject );
        }

        mCurrentObject = NULL;
        FinishObject();

        return temp;
    }

    template< typename TPrimitive >
    inline ITempData *ReadTempPrimitiveArray( size_t size )
    {
        TempArray< TPrimitive > *tArray = CreateTempData< TempArray< TPrimitive > >();
        tArray->Resize( size );
        mStreamBuffer.ReadBytes( tArray->GetData(), size * sizeof( TPrimitive ) );
        return tArray;
    }

    template<>
    inline ITempData *ReadTempPrimitiveArray< std::string >( size_t size )
    {
        TempArray< std::string > *tArray = CreateTempData< TempArray< std::string > >();
        tArray->Resize( size );

        std::vector< char > strVec;

        size_t strSize;

        for ( std::string *it = tArray->GetData(), *end = it + size; it != end; ++it )
        {
            strSize = mStreamBuffer.ReadSize();
            strVec.resize( strSize );
            mStreamBuffer.ReadBytes( &strVec[0], strSize );
            *it = std::string( &strVec[0], strSize );
        }

        return tArray;
    }

    inline ITempData *ReadTempArray()
    {
        ITempData *data = NULL;

        Internal::Type::Type type;
        uint8_t flags;
        ReadHeader( flags, type );

        size_t size = mStreamBuffer.ReadSize();

        switch ( type )
        {
        case Internal::Type::Object:
            data = ReadTempObjectArray( size );
            break;

        case Internal::Type::String:
            data = ReadTempPrimitiveArray< std::string >( size );
            break;

        case Internal::Type::UInt8:
            data = ReadTempPrimitiveArray< uint8_t >( size );
            break;

        case Internal::Type::UInt16:
            data = ReadTempPrimitiveArray< uint16_t >( size );
            break;

        case Internal::Type::UInt32:
            data = ReadTempPrimitiveArray< uint32_t >( size );
            break;

        case Internal::Type::UInt64:
            data = ReadTempPrimitiveArray< uint64_t >( size );
            break;

        default:
            assert( false && "Something went terribly haywire..." );
            break;
        }

        return data;
    }

    inline ITempData *ReadTempObject()
    {
        InitObject();
        mCurrentObject = CreateTempData< TempObject >();
        ReadAll();
        TempObject *obj = mCurrentObject;
        mCurrentObject = NULL;
        FinishObject();
        return obj;
    }

    template< typename TPrimitive >
    inline ITempData *ReadTempPrimitive()
    {
        TempPrimitive< TPrimitive > *const temp = CreateTempData< TempPrimitive< TPrimitive > >();

        TPrimitive value;
        ReadFromStream( value );
        temp->SetValue( value );

        return temp;
    }

    inline void ReadToTemp( uint8_t index, Internal::Type::Type type )
    {
        if ( !mCurrentObject )
        {
            mCurrentObject = CreateTempData< TempObject >();
        }

        ITempData *data = NULL;

        switch ( type )
        {
        case Internal::Type::Array:
            data = ReadTempArray();
            break;

        case Internal::Type::Object:
            data = ReadTempObject();
            break;

        case Internal::Type::String:
            data = ReadTempPrimitive< std::string >( );
            break;

        case Internal::Type::UInt8:
            data = ReadTempPrimitive< uint8_t >( );
            break;

        case Internal::Type::UInt16:
            data = ReadTempPrimitive< uint16_t >( );
            break;

        case Internal::Type::UInt32:
            data = ReadTempPrimitive< uint32_t >( );
            break;

        case Internal::Type::UInt64:
            data = ReadTempPrimitive< uint64_t >( );
            break;

        default:
            assert( false && "Something went terribly haywire..." );
            break;
        }

        mCurrentObject->AddData( index, data );
    }

    template< typename TPrimitive >
    void ReadValue( TPrimitive &value, uint8_t index )
    {
        Internal::Type::Type type = ReadUntil( index );

        if ( !mTerminatorRead )
        {
            const Internal::Type::Type expected = Internal::Type::GetEnum< TPrimitive >();
            assert( Internal::Type::AreCompatible( type, expected ) );

            ReadFromStream( value );
        }
    }

    template< typename TPrimitive >
    void ReadFromStream( TPrimitive &value )
    {
        mStreamBuffer.ReadBytes( &value, sizeof( TPrimitive ) );
    }

    template<>
    void ReadFromStream( std::string &value )
    {
        const size_t size = mStreamBuffer.ReadSize();
        mStreamBuffer.ReadBytes( &value, size );
    }

    template<>
    void ReadFromStream( float &value )
    {
        uint32_t flexman;
        mStreamBuffer.ReadBytes( &flexman, sizeof( uint32_t ) );
        value = Util::UInt32ToFloat( flexman );
    }

    template<>
    void ReadFromStream( double &value )
    {
        uint64_t flexman;
        mStreamBuffer.ReadBytes( &flexman, sizeof( uint64_t ) );
        value = Util::UInt64ToDouble( flexman );
    }

    template< typename T >
    T *CreateTempData()
    {
        return new T();
    }

    template< typename T >
    void DeleteTempData( T *data )
    {
        delete data;
    }

    Internal::Type::Type ReadUntil( uint8_t index )
    {
        uint8_t rIndex;
        Internal::Type::Type type;

        ReadHeader( rIndex, type );

        while ( rIndex != index && type != Internal::Type::Terminator )
        {
            ReadToTemp( rIndex, type );
            ReadHeader( rIndex, type );
        }

        if ( type == Internal::Type::Terminator )
        {
            mTerminatorRead = true;
        }

        return type;
    }

    void ReadAll()
    {

        uint8_t rIndex;
        Internal::Type::Type type;

        ReadHeader( rIndex, type );

        while ( type != Internal::Type::Terminator )
        {
            ReadToTemp( rIndex, type );
            ReadHeader( rIndex, type );
        }

        mTerminatorRead = true;
    }


    BinaryDeserMessage &operator=( const BinaryDeserMessage &bdm );
};

#endif