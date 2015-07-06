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

#include "serialisation/bufferedStreamReader.h"
#include "serialisation/tempPrimitive.h"
#include "serialisation/tempObject.h"
#include "serialisation/tempArray.h"
#include "serialisation/arrayInfo.h"
#include "serialisation/types.h"
#include "serialisation/util.h"

#include <cstring>
#include <stack>

template< typename TStreamReader >
class BinaryDeserMessage
{
public:

    explicit BinaryDeserMessage( const std::string &fileName )
        : mStreamReader( fileName ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    explicit BinaryDeserMessage( std::ifstream &stream )
        : mStreamReader( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    explicit BinaryDeserMessage( std::fstream &stream )
        : mStreamReader( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    explicit BinaryDeserMessage( std::istream &stream )
        : mStreamReader( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    inline void ClearBuffer()
    {
        mStreamReader.ClearBuffer();
    }

    inline void InitObject()
    {
        mObjectHistory.push( mCurrentObject );
        mCurrentObject = NULL;

        mTerminatorRead = false;
    }

    inline bool InitObject( uint8_t index )
    {
        ITempData *const data = mCurrentObject ? mCurrentObject->TryRemoveData( index ) : NULL;

        if ( data )
        {
            InitObject();

            mCurrentObject = static_cast< TempObject * >( data );
            mTerminatorRead = true;
        }
        else
        {
            const Internal::Type::Type type = ReadUntil( index );

            if ( !mTerminatorRead )
            {
                assert( type == Internal::Type::Object );
                InitObject();
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    inline void FinishObject()
    {
        if ( !mTerminatorRead )
        {
            ReadAll();
        }

        if ( mCurrentArray )
        {
            DeleteTempData( mCurrentArray );
            mCurrentArray = NULL;
        }

        if ( mCurrentObject )
        {
            for ( size_t i = 0, size = mCurrentObject->GetSize(); i < size; ++i )
            {
                DeleteTempData( mCurrentObject->PopBack() );
            }

            DeleteTempData( mCurrentObject );
        }

        mCurrentObject = mObjectHistory.top();
        mObjectHistory.pop();

        mTerminatorRead = mCurrentObject ? mCurrentObject->GetTerminatorRead() : false;
    }

    inline void FinishObject( uint8_t /*index*/ )
    {
        FinishObject();
    }

    inline bool InitParent( uint8_t index )
    {
        return InitObject( index + 28 );
    }

    inline void FinishParent( uint8_t index )
    {
        FinishObject( index + 28 );
    }

    inline void InitArrayObject()
    {
        InitObject();

        if ( mCurrentArray )
        {
            static_cast< TempArray< TempObject * > * >( mCurrentArray )->PopFront( mCurrentObject );
            mTerminatorRead = true;
        }

        mArrayHistory.push( mCurrentArray );
        mCurrentArray = NULL;
    }

    inline void FinishArrayObject()
    {
        FinishObject();

        mCurrentArray = mArrayHistory.top();
        mArrayHistory.pop();

        --mArrayInfo.remainingCount;
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

    inline size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags );

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

    template< typename TPrimitive >
    void StoreContiguous( TPrimitive *begin, size_t size )
    {
        if ( mCurrentArray )
        {
            memcpy( begin, static_cast< TempArray< uint32_t > * >( mCurrentArray )->GetData(), size * sizeof( TPrimitive ) );
        }
        else
        {
            mStreamReader.ReadPrimitiveBlock( begin, size );
        }

        mArrayInfo.remainingCount -= size;
    }

    inline void StoreContiguous( float *begin, size_t size )
    {
        if ( mCurrentArray )
        {
            const uint32_t *const intBuffer = static_cast< TempArray< uint32_t > * >( mCurrentArray )->GetData();

            for ( size_t i = 0; i < size; ++i )
            {
                begin[ i ] = Util::UInt32ToFloat( intBuffer[ i ] );
            }
        }
        else
        {
            uint32_t intBuffer[ 128 ];

            for ( size_t i = 0; i < size; i += 128 )
            {
                size_t blockSize = static_cast< size_t >( size - i );
                blockSize = blockSize > 128 ? 128 : blockSize;

                mStreamReader.ReadPrimitiveBlock( intBuffer, blockSize );

                for ( size_t k = 0; k < blockSize; ++k )
                {
                    begin[ k ] = Util::UInt32ToFloat( intBuffer[ k ] );
                }
            }
        }

        mArrayInfo.remainingCount -= size;
    }

    inline void StoreContiguous( double *begin, size_t size )
    {
        if ( mCurrentArray )
        {
            const uint64_t *const intBuffer = static_cast< TempArray< uint64_t > * >( mCurrentArray )->GetData();

            for ( size_t i = 0; i < size; ++i )
            {
                begin[ i ] = Util::UInt64ToDouble( intBuffer[ i ] );
            }
        }
        else
        {
            uint64_t intBuffer[ 128 ];

            for ( size_t i = 0; i < size; i += 128 )
            {
                size_t blockSize = static_cast< size_t >( size - i );
                blockSize = blockSize > 128 ? 128 : blockSize;

                mStreamReader.ReadPrimitiveBlock( intBuffer, blockSize );

                for ( size_t k = 0; k < blockSize; ++k )
                {
                    begin[ k ] = Util::UInt64ToDouble( intBuffer[ k ] );
                }
            }
        }

        mArrayInfo.remainingCount -= size;
    }

private:

    TStreamReader mStreamReader;

    std::stack< TempObject * > mObjectHistory;
    std::stack< AbstractTempArray * > mArrayHistory;

    ArrayInfo mArrayInfo;

    AbstractTempArray *mCurrentArray;
    TempObject *mCurrentObject;

    bool mTerminatorRead;

    BinaryDeserMessage( BinaryDeserMessage & );

    BinaryDeserMessage &operator=( const BinaryDeserMessage &bdm );

    Internal::Type::Type ReadUntil( uint8_t index );

    inline void ReadAll();

    template< typename TPrimitive >
    void ReadFromStream( TPrimitive &value )
    {
        mStreamReader.ReadPrimitive( value );
    }

    inline void ReadFromStream( std::string &value )
    {
        size_t size = mStreamReader.ReadSize();

        value.resize( size, ' ' );

        if ( size > 0 )
        {
            mStreamReader.ReadBytes( &value.at( 0 ), size );
        }
    }

    inline void ReadFromStream( float &value )
    {
        uint32_t flexman;
        mStreamReader.ReadPrimitive( flexman );
        value = Util::UInt32ToFloat( flexman );
    }

    inline void ReadFromStream( double &value )
    {
        uint64_t flexman;
        mStreamReader.ReadPrimitive( flexman );
        value = Util::UInt64ToDouble( flexman );
    }

    template< typename T >
    void ReadHeader( T &index, Internal::Type::Type &type )
    {
        T header;

        ReadFromStream( header );

        index = Util::GetHeaderIndex( header );
        type = Util::GetHeaderType( header );
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

    template< typename TPrimitive >
    void ReadFromTemp( TPrimitive &value, ITempData *data )
    {
        value = static_cast< TempPrimitive< TPrimitive > * >( data )->GetValue();
    }

    inline void ReadFromTemp( float &value, ITempData *data )
    {
        uint32_t flexman;
        ReadFromTemp( flexman, data );
        value = Util::UInt32ToFloat( flexman );
    }

    inline void ReadFromTemp( double &value, ITempData *data )
    {
        uint64_t flexman;
        ReadFromTemp( flexman, data );
        value = Util::UInt64ToDouble( flexman );
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

        mCurrentObject = mObjectHistory.top();
        mObjectHistory.pop();

        mTerminatorRead = mCurrentObject ? mCurrentObject->GetTerminatorRead() : false;

        return temp;
    }

    template< typename TPrimitive >
    inline ITempData *ReadTempPrimitiveArray( size_t size )
    {
        TempArray< TPrimitive > *tArray = CreateTempData< TempArray< TPrimitive > >();

        tArray->Resize( size );

        mStreamReader.ReadPrimitiveBlock( tArray->GetData(), size );

        return tArray;
    }

    ITempData *ReadTempStringArray( size_t size )
    {
        TempArray< std::string > *tArray = CreateTempData< TempArray< std::string > >();
        tArray->Resize( size );

        for ( std::string *it = tArray->GetData(), *end = it + size; it != end; ++it )
        {
            const size_t strSize = mStreamReader.ReadSize();

            it->resize( strSize, ' ' );

            if ( strSize > 0 )
            {
                mStreamReader.ReadBytes( &it->at( 0 ), strSize );
            }
        }

        return tArray;
    }

    inline ITempData *ReadTempArray();

    inline ITempData *ReadTempObject()
    {
        InitObject();

        mCurrentObject = CreateTempData< TempObject >();

        ReadAll();

        TempObject *obj = mCurrentObject;
        mCurrentObject = NULL;

        mCurrentObject = mObjectHistory.top();
        mObjectHistory.pop();

        mTerminatorRead = mCurrentObject ? mCurrentObject->GetTerminatorRead() : false;

        return obj;
    }

    template< typename TPrimitive >
    ITempData *ReadTempPrimitive()
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
    inline void ReadValue( TPrimitive &value, uint8_t index )
    {
        const Internal::Type::Type type = ReadUntil( index );

        if ( !mTerminatorRead )
        {
            const Internal::Type::Type expected = Internal::Type::GetEnum< TPrimitive >();
            assert( Internal::Type::AreCompatible( type, expected ) &&
                    "Whoops, seems like you tried to Deserialise with the wrong type" );

            ReadFromStream( value );
        }
    }
};

template< typename TStreamReader >
inline Internal::Type::Type BinaryDeserMessage< TStreamReader >::ReadUntil( uint8_t index )
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

        if ( mCurrentObject )
        {
            mCurrentObject->SetTerminatorRead();
        }
    }

    return type;
}

template< typename TStreamReader >
inline void BinaryDeserMessage< TStreamReader >::ReadAll()
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

    if ( mCurrentObject )
    {
        mCurrentObject->SetTerminatorRead();
    }
}

template< typename TStreamReader >
inline ITempData *BinaryDeserMessage< TStreamReader >::ReadTempArray()
{
    ITempData *data = NULL;

    Internal::Type::Type type;
    uint8_t flags;
    ReadHeader( flags, type );

    size_t size = mStreamReader.ReadSize();

    switch ( type )
    {
    case Internal::Type::Object:
        data = ReadTempObjectArray( size );
        break;

    case Internal::Type::String:
        data = ReadTempStringArray( size );
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

template< typename TStreamReader >
inline size_t BinaryDeserMessage< TStreamReader >::CreateArray( Type::Type type, size_t size, uint8_t index,
                                                                uint8_t flags )
{
    if ( mCurrentArray )
    {
        DeleteTempData( mCurrentArray );
        mCurrentArray = NULL;
    }

    AbstractTempArray *temp = mCurrentObject ? static_cast< AbstractTempArray * >( mCurrentObject->TryRemoveData(
                                                                                       index ) ) : NULL;

    if ( temp )
    {
        mCurrentArray = temp;
        mArrayInfo.type = temp->GetSubType();
        return mArrayInfo.remainingCount = temp->GetRemainingCount();
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
            size = mStreamReader.ReadSize();

            mArrayInfo.type = rType;
            mArrayInfo.remainingCount = size;
        }
        else
        {
            return 0;
        }
    }

    return size;
}

#endif