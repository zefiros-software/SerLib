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


#include "interface/abstractTempArray.h"

#include "tempPrimitive.h"
#include "readBuffer.h"
#include "tempObject.h"
#include "tempArray.h"
#include "arrayInfo.h"
#include "defines.h"
#include "types.h"
#include "util.h"

#include <assert.h>
#include <stack>

class BinaryDeserMessage
{
public:

    BinaryDeserMessage( const std::string &fileName )
        : mStreamBuffer( fileName ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    BinaryDeserMessage( std::ifstream &stream )
        : mStreamBuffer( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    BinaryDeserMessage( std::fstream &stream )
        : mStreamBuffer( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    BinaryDeserMessage( std::istream &stream )
        : mStreamBuffer( stream ),
          mCurrentArray( NULL ),
          mCurrentObject( NULL ),
          mTerminatorRead( false )
    {
        mArrayInfo.type = Internal::Type::Terminator;
        mArrayInfo.remainingCount = 0;
    }

    inline void ClearBuffer()
    {
        mStreamBuffer.ClearBuffer();
    }

    inline void InitObject()
    {
        mObjectHistory.push( mCurrentObject );
        mCurrentObject = NULL;

        mTerminatorRead = false;
    }

    inline bool InitObject( uint8_t index );

    inline void FinishObject();

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
    void Store( TPrimitive &value, uint8_t index );

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
            mStreamBuffer.ReadPrimitiveBlock( begin, size );
        }

        mArrayInfo.remainingCount -= size;
    }

private:

    ReadBuffer mStreamBuffer;

    std::stack< TempObject * > mObjectHistory;
    std::stack< AbstractTempArray * > mArrayHistory;

    ArrayInfo mArrayInfo;

    AbstractTempArray *mCurrentArray;
    TempObject *mCurrentObject;

    bool mTerminatorRead;


    Internal::Type::Type ReadUntil( uint8_t index );

    inline void ReadAll();

    template< typename TPrimitive >
    void ReadFromStream( TPrimitive &value )
    {
        mStreamBuffer.ReadPrimitive( value );
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

        mStreamBuffer.ReadPrimitiveBlock( tArray->GetData(), size );

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
    void ReadValue( TPrimitive &value, uint8_t index );

    BinaryDeserMessage &operator=( const BinaryDeserMessage &bdm );
};

inline Internal::Type::Type BinaryDeserMessage::ReadUntil( uint8_t index )
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

inline void BinaryDeserMessage::ReadAll()
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

template<>
inline void BinaryDeserMessage::ReadFromStream( std::string &value )
{
    size_t size = mStreamBuffer.ReadSize();

    value.resize( size, ' ' );

    if ( size > 0 )
    {
        mStreamBuffer.ReadBytes( &value.at( 0 ), size );
    }
}

template<>
inline void BinaryDeserMessage::ReadFromStream( float &value )
{
    uint32_t flexman;
    mStreamBuffer.ReadPrimitive( flexman );
    value = Util::UInt32ToFloat( flexman );
}

template<>
inline void BinaryDeserMessage::ReadFromStream( double &value )
{
    uint64_t flexman;
    mStreamBuffer.ReadPrimitive( flexman );
    value = Util::UInt64ToDouble( flexman );
}

template< typename TPrimitive >
inline void BinaryDeserMessage::ReadValue( TPrimitive &value, uint8_t index )
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

template<>
inline ITempData *BinaryDeserMessage::ReadTempPrimitiveArray< std::string >( size_t size )
{
    TempArray< std::string > *tArray = CreateTempData< TempArray< std::string > >();
    tArray->Resize( size );

    std::vector< char > strVec;
    size_t strSize;

    for ( std::string *it = tArray->GetData(), *end = it + size; it != end; ++it )
    {
        strSize = mStreamBuffer.ReadSize();

        it->resize( strSize, ' ' );

        if ( strSize > 0 )
        {
            mStreamBuffer.ReadBytes( &it->at( 0 ), strSize );
        }
    }

    return tArray;
}

inline ITempData *BinaryDeserMessage::ReadTempArray()
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

template<>
inline void BinaryDeserMessage::ReadFromTemp( float &value, ITempData *data )
{
    uint32_t flexman;
    ReadFromTemp( flexman, data );
    value = Util::UInt32ToFloat( flexman );
}

template<>
inline void BinaryDeserMessage::ReadFromTemp( double &value, ITempData *data )
{
    uint64_t flexman;
    ReadFromTemp( flexman, data );
    value = Util::UInt64ToDouble( flexman );
}

template< typename TPrimitive >
void BinaryDeserMessage::Store( TPrimitive &value, uint8_t index )
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


inline size_t BinaryDeserMessage::CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags )
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
            size = mStreamBuffer.ReadSize();

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

template<>
inline void BinaryDeserMessage::StoreContiguous( float *begin, size_t size )
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

            mStreamBuffer.ReadPrimitiveBlock( intBuffer, blockSize );

            for ( size_t k = 0; k < blockSize; ++k )
            {
                begin[ k ] = Util::UInt32ToFloat( intBuffer[ k ] );
            }
        }
    }

    mArrayInfo.remainingCount -= size;
}

template<>
inline void BinaryDeserMessage::StoreContiguous( double *begin, size_t size )
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

            mStreamBuffer.ReadPrimitiveBlock( intBuffer, blockSize );

            for ( size_t k = 0; k < blockSize; ++k )
            {
                begin[ k ] = Util::UInt64ToDouble( intBuffer[ k ] );
            }
        }
    }

    mArrayInfo.remainingCount -= size;
}

inline void BinaryDeserMessage::FinishObject()
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

inline bool BinaryDeserMessage::InitObject( uint8_t index )
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

#endif