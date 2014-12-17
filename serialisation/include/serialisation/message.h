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

#include "interface\ISerialisable.h"

#include "intermediateData.h"
#include "streamBuffer.h"
#include "repeatedInfo.h"
#include "types.h"
#include "util.h"

#include <iostream>
#include <assert.h>
#include <fstream>
#include <vector>

class Message
    : public IIntermediateData
{
public:

    enum Flags
    {
        Packed = Internal::Flags::Packed
    };

    Message( std::iostream &stream, Mode::Mode mode = Mode::Serialise )
        : mMode( ( Internal::Mode::Mode )mode ), mStreamBuffer( *new StreamBuffer< SERIALISERS_BUFFERSIZE >( stream ) ),
          mRemainingCount( 0 ),  mStreamBufferCreated( &mStreamBuffer ), mRepeatedInfo( Internal::Type::Repeated, 0, 0 )
    {
    }

    Message( std::string &fileName, Mode::Mode mode = Mode::Serialise )
        : mMode( ( Internal::Mode::Mode )mode ), mStreamBuffer( *new StreamBuffer< SERIALISERS_BUFFERSIZE >( fileName ) ),
          mRemainingCount( 0 ), mStreamBufferCreated( &mStreamBuffer ), mRepeatedInfo( Internal::Type::Repeated, 0, 0 )
    {

    }

    Message( ISerialisable &serialisable, std::iostream &stream, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( *new StreamBuffer< SERIALISERS_BUFFERSIZE >( stream ) ), mMode( ( Internal::Mode::Mode )mode ),
          mRemainingCount( 0 ), mStreamBufferCreated( &mStreamBuffer ), mRepeatedInfo( Internal::Type::Repeated, 0, 0 )
    {
        Store( serialisable );
    }

    ~Message()
    {
        if ( mStreamBufferCreated )
        {
            delete mStreamBufferCreated;
        }
    }

    void SetMode( Mode::Mode mode )
    {
        mMode = ( Internal::Mode::Mode )mode;
    }

    Mode::Mode GetMode() const
    {
        return ( Mode::Mode )mMode;
    }

    uint32_t GetFlags() const
    {
        return mFlags;
    }

    void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    Internal::Type::Type GetType() const
    {
        return Internal::Type::Variable;
    }

    void Store( ISerialisable &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( std::string &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( uint8_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( uint16_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( uint32_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( uint64_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( int8_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( int16_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( int32_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( int64_t &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( float &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( double &value, const uint8_t index, const uint32_t flags = 0x00 )
    {
        StoreValue( value, index, flags );
    }

    void Store( ISerialisable &serialisable )
    {
        bool isSerialising = mMode == Internal::Mode::Serialise;
        bool isDeserialising = mMode == Internal::Mode::Deserialise;

        if ( isSerialising )
        {
            mMode = Internal::Mode::PrePass;
            serialisable.OnStore( *this );

            mMode = Internal::Mode::Serialise;

            WriteToStream( mRemainingCount, 0 );
        }
        else if ( isDeserialising )
        {
            ReadFromStream( mRemainingCount );
        }

        serialisable.OnStore( *this );

        if ( isSerialising )
        {
            mStreamBuffer.FlushWriteBuffer();
        }
        else if ( isDeserialising )
        {
            mStreamBuffer.ClearReadBuffer();
        }
    }

    uint32_t CreateRepeated( Type::Type type, uint32_t size, const uint8_t index, const uint32_t flags )
    {
        Internal::Type::Type iType = ( Internal::Type::Type )type;

        switch ( mMode )
        {
        case Internal::Mode::Serialise:
            {
                mRepeatedInfo.Set( iType, size, flags );
                WriteHeader( index, Internal::Type::Repeated );
                WriteHeader( size, iType );
            }
            break;

		case Internal::Mode::Deserialise:
			{
				std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it = SearchBuffer( index );

				if ( it != mIntermediateData.end() )
				{
					ReadFromIntermediate( value, it );
					delete it->second;
					mIntermediateData.erase( it );
				}
				else
				{

				}
			}
            break;

        case Internal::Mode::PrePass:
            break;

        case Internal::Mode::Intermediate:
            break;
        }
    }

    void StoreRepeated( ISerialisable &value )
    {
        WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( std::string &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( uint8_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( uint16_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( uint32_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( uint64_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( int8_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( int16_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( int32_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( int64_t &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( float &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

    void StoreRepeated( double &value )
	{
		WriteToStream( value, mRepeatedInfo.Flags );
    }

private:

    Internal::Mode::Mode mMode;

    StreamBuffer< SERIALISERS_BUFFERSIZE > &mStreamBuffer;
    StreamBuffer< SERIALISERS_BUFFERSIZE > *mStreamBufferCreated;

    uint32_t mFlags;
    int8_t mRemainingCount;

    RepeatedInfo mRepeatedInfo;

    std::vector< std::pair< uint8_t, IIntermediateData * > > mIntermediateData;

    Message( ISerialisable &serialisable, StreamBuffer< SERIALISERS_BUFFERSIZE > &streamBuffer, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( streamBuffer ), mMode( ( Internal::Mode::Mode )mode ), mRemainingCount( 0 ), mStreamBufferCreated( NULL ),
          mRepeatedInfo( Internal::Type::Repeated, 0, 0 )
    {
        Store( serialisable );
    }

    Message( StreamBuffer< SERIALISERS_BUFFERSIZE > &streamBuffer, Mode::Mode mode = Mode::Serialise )
        : mStreamBuffer( streamBuffer ), mMode( ( Internal::Mode::Mode )mode ), mRemainingCount( 0 ), mStreamBufferCreated( NULL ),
          mRepeatedInfo( Internal::Type::Repeated, 0, 0 )
    {
    }

    template< typename T >
    void StoreValue( T &value, const uint8_t index, const uint32_t flags )
    {
        Internal::Type::Type type = Internal::Type::GetEnum< T >();

        switch ( mMode )
        {
        case Internal::Mode::Serialise:
            {
                WriteHeader( index, type );
                WriteToStream( value, flags );
                --mRemainingCount;
            }
            break;

        case Internal::Mode::Deserialise:
            {
                std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it = SearchBuffer( index );

                if ( it != mIntermediateData.end() )
                {
                    ReadFromIntermediate( value, it );
                    delete it->second;
                    mIntermediateData.erase( it );
                }
                else
                {
                    ReadValue( value, index );
                }
            }
            break;

        case Internal::Mode::Intermediate:
            {
                std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it = SearchBuffer( index );
                ReadFromIntermediate( value, it );
                delete it->second;
                mIntermediateData.erase( it );
            }
            break;

        case Internal::Mode::PrePass:
            ++mRemainingCount;
            break;

        default:
            break;
        }
    }

    template< typename T >
    void WriteToStream( T value, const uint32_t )
    {
        mStreamBuffer.WriteBytes( &value, sizeof( T ) );
    }

    void WriteToStream( std::string &value, const uint32_t )
    {
        WriteToStream( ( uint32_t )value.length(), 0x00 );
        mStreamBuffer.WriteBytes( value.c_str(), value.length() );
    }

    void WriteToStream( ISerialisable &value, const uint32_t )
    {
        Message( value, mStreamBuffer, Mode::Serialise );
    }

    void WriteToStream( float value, const uint32_t flags )
    {
        uint32_t flexman = Util::FloatToUInt32( value );
        WriteToStream( flexman, flags );
    }

    void WriteToStream( double value, const uint32_t flags )
    {
        uint64_t flexman = Util::DoubleToUInt64( value );
        WriteToStream( flexman, flags );
    }

    void WriteHeader( const uint8_t index, Internal::Type::Type type )
    {
        if ( Internal::Type::IsSignedInt( type ) )
        {
            type = ( Internal::Type::Type )( type - Internal::Type::SInt8 + Internal::Type::UInt8 );
        }
        else if ( type == Internal::Type::Float )
        {
            type = Internal::Type::UInt32;
        }
        else if ( type == Internal::Type::Double )
        {
            type = Internal::Type::Double;
        }

        uint8_t header = Util::CreateHeader( index, type );
        WriteToStream( header, 0x00 );
    }

    void ReadHeader( uint8_t &index, Internal::Type::Type &type )
    {
        uint8_t header;
        ReadFromStream( header );
        index = Util::GetHeaderIndex( header );
        type = Util::GetHeaderType( header );
    }

    std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator SearchBuffer( const uint32_t index )
    {
        for ( std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it = mIntermediateData.begin(),
                end = mIntermediateData.end();
                it != end; ++it )
        {
            if ( it->first == index )
            {
                return it;
            }
        }

        return mIntermediateData.end();
    }

    template< typename T >
    void ReadValue( T &value, const uint8_t index )
    {
        uint8_t rIndex;
        Internal::Type::Type type;

        ReadHeader( rIndex, type );

        while ( rIndex != index )
        {
            ReadToIntermediate( rIndex, type );
            ReadHeader( rIndex, type );
            --mRemainingCount;
        }

        ReadFromStream( value );
        --mRemainingCount;
    }

    template< typename T >
    void ReadFromIntermediate( T &value, std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it )
    {
        value = static_cast< IntermediateData< T > * >( it->second )->GetValue();
    }

    template<>
    void ReadFromIntermediate( ISerialisable &value, std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it )
    {
        Message &message = *static_cast< Message * >( it->second );
        message.Store( value );
    }

    void ReadFromIntermediate( float &value, std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it )
    {
        uint32_t flexman = static_cast< IntermediateData< uint32_t > * >( it->second )->GetValue();
        value = Util::UInt32ToFloat( flexman );
    }

    void ReadFromIntermediate( double &value, std::vector< std::pair< uint8_t, IIntermediateData * > >::iterator it )
    {
        uint64_t flexman = static_cast< IntermediateData< uint64_t > * >( it->second )->GetValue();
        value = Util::UInt64ToDouble( flexman );
    }

    template< typename T >
    void ReadFromStream( T &value )
    {
        mStreamBuffer.ReadBytes( &value, sizeof( T ) );
    }

    template<>
    void ReadFromStream( ISerialisable &value )
    {
        Message( value, mStreamBuffer, Mode::Deserialise );
    }

    void ReadFromStream( std::string &value )
    {
        uint32_t size;
        ReadFromStream( size );

        std::vector< char > buffer( size );

        char *firstByte = &buffer.front();

        mStreamBuffer.ReadBytes( firstByte, size );
        value = std::string( firstByte, size );
    }

    void ReadFromStream( float &value )
    {
        uint32_t flexman;
        ReadFromStream( flexman );
        value = Util::UInt32ToFloat( flexman );
    }

    void ReadFromStream( double &value )
    {
        uint64_t flexman;
        ReadFromStream( flexman );
        value = Util::UInt64ToDouble( flexman );
    }

    void ReadToIntermediate( const uint8_t index, const Internal::Type::Type type )
    {
        IIntermediateData *data = NULL;

        switch ( type )
        {
        case Internal::Type::Variable:
            data = new Message( mStreamBuffer, Mode::Deserialise );
            break;

        case Internal::Type::String:
            data = new IntermediateData< std::string >();
            break;

        case Internal::Type::UInt8:
            data = new IntermediateData< uint8_t >();
            break;

        case Internal::Type::UInt16:
            data = new IntermediateData< uint16_t >();
            break;

        case Internal::Type::UInt32:
            data = new IntermediateData< uint32_t >();
            break;

        case Internal::Type::UInt64:
            data = new IntermediateData< uint64_t >();
            break;
        }

        data->ReadFrom( mStreamBuffer );
        mIntermediateData.push_back( std::pair< uint8_t, IIntermediateData * >( index, data ) );
    }

    void ReadFrom( StreamBuffer< SERIALISERS_BUFFERSIZE > &streamBuffer )
    {
        std::swap( mStreamBuffer, streamBuffer );

        ReadFromStream( mRemainingCount );

        uint8_t index;
        Internal::Type::Type type;

        for ( int8_t i = 0; i < mRemainingCount; ++i )
        {
            ReadHeader( index, type );
            ReadToIntermediate( index, type );
        }

        mRemainingCount = 0;
        mMode = Internal::Mode::Intermediate;

        std::swap( mStreamBuffer, streamBuffer );
    }
};

#endif