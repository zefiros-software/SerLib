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

#include "interface/ISerialisable.h"

#include "tempPrimitive.h"
#include "streamBuffer.h"
#include "tempObject.h"
#include "arrayInfo.h"
#include "tempArray.h"
#include "types.h"
#include "util.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

class Message
{
public:

    enum Flags
    {
        Packed = Internal::Flags::Packed
    };

    Message( std::iostream &stream, Mode::Mode mode = Mode::Serialise )
        : mMode( static_cast< Internal::Mode::Mode >( mode ) ),
          mStreamBuffer( stream ),
          mArrayInfo( Internal::Type::Terminator, 0 ),
          mCurrentObject( NULL ),
          mCurrentArray( NULL ),
          mBufferNonEmpty( false )
    {
    }

    Message( std::string &fileName, Mode::Mode mode = Mode::Serialise )
        : mMode( static_cast< Internal::Mode::Mode >( mode ) ),
          mStreamBuffer( fileName ),
          mArrayInfo( Internal::Type::Terminator, 0 ),
          mCurrentObject( NULL ),
          mCurrentArray( NULL ),
          mBufferNonEmpty( false )
    {

    }

    Message( ISerialisable &serialisable, std::iostream &stream, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( stream ),
          mMode( static_cast< Internal::Mode::Mode >( mode ) ),
          mArrayInfo( Internal::Type::Array, 0 ),
          mCurrentObject( NULL ),
          mCurrentArray( NULL ),
          mBufferNonEmpty( false )
    {
        Store( serialisable );
    }

    ~Message()
    {
        mStreamBuffer.Close();
    }

    void SetMode( Mode::Mode mode )
    {
        mMode = static_cast<  Internal::Mode::Mode >( mode );
    }

    Mode::Mode GetMode() const
    {
        return static_cast<  Mode::Mode >( mMode );
    }

    void ClearBuffers()
    {
        mStreamBuffer.FlushWriteBuffer();
        mStreamBuffer.ClearReadBuffer();
    }

    void Store( ISerialisable &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( std::string &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( uint8_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( uint16_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( uint32_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( uint64_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( int8_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( int16_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( int32_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( int64_t &value, uint8_t index )
    {
        StoreValue( value, index );
    }

    void Store( float &value, uint8_t index )
    {
        const bool isSerialising = mMode == Mode::Serialise;

        uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;

        StoreValue( flexman, index );

        if ( !isSerialising )
        {
            value = Util::UInt32ToFloat( flexman );
        }
    }

    void Store( double &value, uint8_t index )
    {
        const bool isSerialising = mMode == Mode::Serialise;

        uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;

        StoreValue( flexman, index );

        if ( !isSerialising )
        {
            value = Util::UInt64ToDouble( flexman );
        }
    }

    void Store( ISerialisable &serialisable )
    {
        TempObject *const temp = CreateTempData< TempObject >();

        Store( serialisable, temp );

        DeleteTempData( temp );
    }

    template< typename TSerialisable >
    void Store( TSerialisable &serialisable )
    {
        TempObject *const temp = CreateTempData< TempObject >();

        Store( serialisable, temp );

        DeleteTempData( temp );
    }

    uint32_t CreateArray( Type::Type type, uint32_t size, uint8_t index )
    {
        const Internal::Type::Type iType = static_cast< Internal::Type::Type >( type );

        if ( mMode == Internal::Mode::Serialise )
        {
            mArrayInfo.Set( iType, size );
            WriteHeader( index, Internal::Type::Array );
            WriteHeader( index, iType );
            WriteToStream( size );
        }
        else
        {
            if ( mCurrentArray )
            {
                DeleteTempData( mCurrentArray );
                mCurrentArray = NULL;
            }

            AbstractTempArray *temp = mBufferNonEmpty ? static_cast< AbstractTempArray * >( mCurrentObject->TryRemoveData(
                                          index ) ) : NULL;

            if ( temp )
            {
                mCurrentArray = temp;
                mArrayInfo.Set( temp->GetSubType(), temp->GetRemainingCount() );
            }
            else
            {
                if ( mMode == Internal::Mode::Deserialise && !mCurrentObject->GetTerminatorRead() )
                {
                    const Internal::Type::Type type = ReadUntil( index );

                    if ( type != Internal::Type::Terminator )
                    {
                        Internal::Type::Type rType;
                        ReadHeader( index, rType );
                        ReadFromStream( size );
                        mArrayInfo.Set( rType, size );
                    }
                }
            }
        }

        return size;
    }

    void StoreArrayItem( ISerialisable &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( std::string &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( uint8_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( uint16_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( uint32_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( uint64_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( int8_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( int16_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( int32_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( int64_t &value )
    {
        StoreArrayValue( value );
    }

    void StoreArrayItem( float &value )
    {
        const bool isSerialising = mMode == Mode::Serialise;

        uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;

        StoreArrayValue( flexman );

        if ( !isSerialising )
        {
            value = Util::UInt32ToFloat( flexman );
        }
    }

    void StoreArrayItem( double &value )
    {
        const bool isSerialising = mMode == Mode::Serialise;

        uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;

        StoreArrayValue( flexman );

        if ( !isSerialising )
        {
            value = Util::UInt64ToDouble( flexman );
        }
    }

private:

    StreamBuffer< SERIALISERS_BUFFERSIZE > mStreamBuffer;

    std::stack< TempObject * > mTempBuffer;

    ArrayInfo mArrayInfo;

    AbstractTempArray *mCurrentArray;
    TempObject *mCurrentObject;

    Internal::Mode::Mode mMode;

    bool mBufferNonEmpty;

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

    template< typename TObject >
    void Store( TObject &serialisable, TempObject *obj )
    {
        mTempBuffer.push( mCurrentObject );
        mCurrentObject = obj;

        if ( mCurrentObject )
        {
            mBufferNonEmpty = mCurrentObject->IsNonEmpty();
        }
        else
        {
            mBufferNonEmpty = false;
        }

        serialisable.SERIALISATION_CUSTOM_INTERFACE( *this );

        if ( mMode == Internal::Mode::Serialise )
        {
            WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
        }
        else if ( mMode == Internal::Mode::Deserialise )
        {
            if ( !mCurrentObject->GetTerminatorRead() )
            {
                ReadAll();
            }
        }

        mCurrentObject = mTempBuffer.top();
        mTempBuffer.pop();

        if ( mCurrentArray )
        {
            DeleteTempData( mCurrentArray );
            mCurrentArray = NULL;
        }

        if ( mCurrentObject )
        {
            mBufferNonEmpty = mCurrentObject->IsNonEmpty();
        }
        else
        {
            mBufferNonEmpty = false;
        }
    }

    void Store( ISerialisable &serialisable, TempObject *obj )
    {
        mTempBuffer.push( mCurrentObject );
        mCurrentObject = obj;

        if ( mCurrentObject )
        {
            mBufferNonEmpty = mCurrentObject->IsNonEmpty();
        }
        else
        {
            mBufferNonEmpty = false;
        }

        serialisable.SERIALISATION_CUSTOM_INTERFACE( *this );

        if ( mMode == Internal::Mode::Serialise )
        {
            WriteHeader( static_cast<  uint8_t >( 0 ), Internal::Type::Terminator );
        }
        else if ( mMode == Internal::Mode::Deserialise )
        {
            if ( !mCurrentObject->GetTerminatorRead() )
            {
                ReadAll();
            }
        }

        mCurrentObject = mTempBuffer.top();
        mTempBuffer.pop();

        if ( mCurrentArray )
        {
            DeleteTempData( mCurrentArray );
            mCurrentArray = NULL;
        }

        if ( mCurrentObject )
        {
            mBufferNonEmpty = mCurrentObject->IsNonEmpty();
        }
        else
        {
            mBufferNonEmpty = false;
        }
    }

    template< typename T >
    void StoreValue( T &value, uint8_t index )
    {
        const Internal::Type::Type type = Internal::Type::GetEnum< T >();

        if ( mMode == Internal::Mode::Serialise )
        {
            WriteHeader( index, type );
            WriteToStream( value );
        }
        else
        {
            ITempData *const data = mBufferNonEmpty ? mCurrentObject->TryRemoveData( index ) : NULL;

            if ( data )
            {
                ReadFromTemp( value, data );
                DeleteTempData( data );
            }
            else if ( mMode == Internal::Mode::Deserialise && !mCurrentObject->GetTerminatorRead() )
            {
                ReadValue( value, index );
            }
        }
    }

    template< typename T >
    void StoreArrayValue( T &value )
    {
        if ( mMode == Internal::Mode::Serialise )
        {
            WriteToStream( value );
        }
        else
        {
            if ( mCurrentArray )
            {
                static_cast< TempArray< T > * >( mCurrentArray )->PopFront( value );
            }
            else if ( mMode == Internal::Mode::Deserialise && !mCurrentObject->GetTerminatorRead() )
            {
                ReadFromStream( value );
            }
        }

        --mArrayInfo.remainingCount;
    }

    void StoreArrayValue( ISerialisable &value )
    {
        if ( mMode == Internal::Mode::Serialise )
        {
            WriteToStream( value );
        }
        else
        {
            if ( mCurrentArray )
            {
                TempObject *temp;
                static_cast< TempArray< TempObject * > * >( mCurrentArray )->PopFront( temp );

                Store( value, temp );

                DeleteTempData( temp );
            }
            else if ( mMode == Internal::Mode::Deserialise && !mCurrentObject->GetTerminatorRead() )
            {
                ReadFromStream( value );
            }
        }

        --mArrayInfo.remainingCount;
    }

    template< typename T >
    void WriteToStream( T value )
    {
        mStreamBuffer.WriteBytes( &value, sizeof( T ) );
    }

    void WriteToStream( std::string &value )
    {
        WriteToStream( static_cast<  uint32_t  >( value.length() ) );
        mStreamBuffer.WriteBytes( value.c_str(), value.length() );
    }

    void WriteToStream( ISerialisable &value )
    {
        Store( value );
    }

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
        WriteToStream( header );
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
    void ReadValue( T &value, uint8_t index )
    {
        const Internal::Type::Type type = ReadUntil( index );

        if ( type != Internal::Type::Terminator )
        {
            ReadFromStream( value );
        }
    }

    Internal::Type::Type ReadUntil( uint8_t index )
    {
        uint8_t rIndex;
        Internal::Type::Type type;

        ReadHeader( rIndex, type );

        if ( rIndex != index )
        {
            mBufferNonEmpty = true;
        }

        while ( rIndex != index && type != Internal::Type::Terminator )
        {
            ReadToTemp( rIndex, type );
            ReadHeader( rIndex, type );
        }

        if ( type == Internal::Type::Terminator )
        {
            mCurrentObject->SetTerminatorRead();
        }

        return type;
    }

    void ReadAll()
    {
        uint8_t index;
        Internal::Type::Type type;

        ReadHeader( index, type );

        while ( type != Internal::Type::Terminator )
        {
            ReadToTemp( index, type );
            ReadHeader( index, type );
        }

        mCurrentObject->SetTerminatorRead();

        mBufferNonEmpty = true;
    }

    template< typename T >
    void ReadFromTemp( T &value, ITempData *const data )
    {
        value = static_cast< TempPrimitive< T > * >( data )->GetValue();
    }

    void ReadFromTemp( ISerialisable &value, ITempData *const data )
    {
        mMode = Internal::Mode::DeserialiseTemp;
        Store( value, static_cast< TempObject * >( data ) );
        mMode = Internal::Mode::Deserialise;
    }

    template< typename T >
    void ReadFromStream( T &value )
    {
        mStreamBuffer.ReadBytes( &value, sizeof( T ) );
    }

    void ReadFromStream( ISerialisable &value )
    {
        Store( value );
    }

    void ReadFromStream( std::string &value )
    {
        uint32_t size;
        ReadFromStream( size );

        value.resize( size, ' ' );

        mStreamBuffer.ReadBytes( &value.front(), size );
    }

    void ReadToTemp( uint8_t index, Internal::Type::Type type )
    {
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
        }

        mCurrentObject->AddData( index, data );
    }

    template< typename T >
    ITempData *ReadTempPrimitive()
    {
        TempPrimitive< T > *const temp = CreateTempData< TempPrimitive< T > >();

        T value;
        ReadFromStream( value );
        temp->SetValue( value );

        return temp;
    }

    TempObject *ReadTempObject()
    {
        mTempBuffer.push( mCurrentObject );
        mCurrentObject = CreateTempData< TempObject >();
        mBufferNonEmpty = true;

        ReadAll();

        TempObject *const temp = mCurrentObject;
        mCurrentObject = mTempBuffer.top();
        mTempBuffer.pop();

        return temp;
    }

    ITempData *ReadTempArray();

    template< typename T >
    ITempData *ReadTempArrayPrimitive( uint32_t size )
    {
        TempArray< T > *temp = CreateTempData< TempArray< T > >();
        temp->Resize( size );

        T *first = temp->GetData();
        mStreamBuffer.ReadBytes( first, sizeof( T ) * size );

        return temp;
    }

    ITempData *ReadTempArrayObject( uint32_t size )
    {
        TempArray< TempObject * > *temp = CreateTempData< TempArray< TempObject * > >();

        for ( uint32_t i = 0; i < size; ++i )
        {
            TempObject *obj = ReadTempObject();
            temp->PushBack( obj );
        }

        return temp;
    }
};

template<>
ITempData *Message::ReadTempArrayPrimitive< std::string >( uint32_t size )
{
    TempArray< std::string > *temp = CreateTempData< TempArray< std::string > >();
    std::vector< char > strVec;

    uint32_t strSize;

    for ( uint32_t i = 0; i < size; ++i )
    {
        ReadFromStream( strSize );
        strVec.resize( strSize );
        mStreamBuffer.ReadBytes( &strVec[0], strSize );
        temp->PushBack( std::string( &strVec[0], strSize ) );
    }

    return temp;
}

ITempData *Message::ReadTempArray()
{
    ITempData *data = NULL;

    uint32_t size;
    Internal::Type::Type type;

    ReadHeader( size, type );

    switch ( type )
    {
    case Internal::Type::Object:
        data = ReadTempArrayObject( size );
        break;

    case Internal::Type::String:
        data = ReadTempArrayPrimitive< std::string >( size );
        break;

    case Internal::Type::UInt8:
        data = ReadTempArrayPrimitive< uint8_t >( size );
        break;

    case Internal::Type::UInt16:
        data = ReadTempArrayPrimitive< uint16_t >( size );
        break;

    case Internal::Type::UInt32:
        data = ReadTempArrayPrimitive< uint32_t >( size );
        break;

    case Internal::Type::UInt64:
        data = ReadTempArrayPrimitive< uint64_t >( size );
        break;
    }

    return data;
}

#endif