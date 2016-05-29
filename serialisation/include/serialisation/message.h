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

#include "serialisation/bufferedStreamWriter.h"
#include "serialisation/binaryDeserMessage.h"
#include "serialisation/binarySerMessage.h"
#include "serialisation/messageAdapter.h"
#include "serialisation/messageHelper.h"

#include <sstream>

namespace MessageHelper {};

class Message
{
public:

    template< typename TSerialisable >
    friend void MessageHelper::Store( Message &message, TSerialisable &serialisable, bool clearBuffers );

    enum Flags
    {
        Packed = Internal::Flags::Packed
    };

    explicit Message( const std::string &fileName, Format::Format format, Mode::Mode mode = Mode::Serialise,
                      bool useBuffer = true );

    explicit Message( std::stringstream &stream, Format::Format format, Mode::Mode mode = Mode::Serialise,
                      bool useBuffer = true );

    explicit Message( std::iostream &stream, Format::Format format, Mode::Mode mode = Mode::Serialise,
                      bool useBuffer = true );

    explicit Message( std::fstream &stream, Format::Format format, Mode::Mode mode = Mode::Serialise,
                      bool useBuffer = true );

    explicit Message( std::ifstream &stream, Format::Format format, bool useBuffer = true );

    explicit Message( std::istream &stream, Format::Format format, bool useBuffer = true );

    explicit Message( std::ofstream &stream, Format::Format format, bool useBuffer = true );

    explicit Message( std::ostream &stream, Format::Format format, bool useBuffer = true );

    ~Message();

    void ClearBuffers();

    Mode::Mode GetMode();

    Format::Format GetFormat();

    template< typename TSerialisable >
    void Store( TSerialisable &serialisable, uint8_t index )
    {
        SERIALISATION_ASSERT_INDEX_IN_RANGE( index );

        if ( mInternalMessage->InitObject( index ) )
        {
            MessageHelper::SERIALISATION_CUSTOM_INTERFACE( serialisable, *this );

            mInternalMessage->FinishObject( index );
        }
    }

    template< typename TParent >
    void StoreParent( TParent &parent, uint8_t index )
    {
        SERIALISATION_ASSERT_PARENT_INDEX_IN_RANGE( index );

        if ( mInternalMessage->InitParent( index ) )
        {
            MessageHelper::StoreParent( parent, *this );

            mInternalMessage->FinishParent( index );
        }
    }

    void Store( ISerialisable &serialisable, uint8_t index );

    void Store( std::string &value, uint8_t index );

    void Store( uint8_t &value, uint8_t index );

    void Store( uint16_t &value, uint8_t index );

    void Store( uint32_t &value, uint8_t index );

    void Store( uint64_t &value, uint8_t index );

    void Store( int8_t &value, uint8_t index );

    void Store( int16_t &value, uint8_t index );

    void Store( int32_t &value, uint8_t index );

    void Store( int64_t &value, uint8_t index );

    void Store( float &value, uint8_t index );

    void Store( double &value, uint8_t index );

    template< size_t Size, typename TSerialisable >
    void StoreContainer( TSerialisable( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        const size_t size = CreateArray( Type::Object, Size, index, flags );

        if ( size <= Size )
        {
            for ( TSerialisable *it = container, end = it + size; it != end; ++it )
            {
                StoreArrayItem( *it );
            }
        }
        else
        {
            for ( TSerialisable *it = container, end = it + Size; it != end; ++it )
            {
                StoreArrayItem( *it );
            }

            TSerialisable dummy();

            for ( size_t i = Size; i < size; ++i )
            {
                StoreArrayItem( dummy );
            }
        }
    }

    template< size_t Size >
    void StoreContainer( uint8_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( uint16_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( uint32_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( uint64_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( int8_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( int16_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( int32_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( int64_t( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( float( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }

    template< size_t Size >
    void StoreContainer( double( &container )[ Size ], uint8_t index, uint8_t flags = 0x00 )
    {
        StorePrimitveCArray( container, index, flags );
    }



    template< typename TContainer >
    void StoreContainer( TContainer &container, uint8_t index, uint8_t flags = 0x00 )
    {
        const Type::Type type = static_cast< Type::Type >( Internal::Type::GetEnum< typename TContainer::value_type >() );
        const size_t size = CreateArray( type, container.size(), index, flags );
        container.resize( size );

        for ( typename TContainer::iterator it = container.begin(), end = container.end(); it != end; ++it )
        {
            StoreArrayItem( *it );
        }
    }

    void StoreContainer( std::vector< uint8_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< uint16_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< uint32_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< uint64_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< int8_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< int16_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< int32_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< int64_t > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< float > &container, uint8_t index, uint8_t flags = 0x00 );

    void StoreContainer( std::vector< double > &container, uint8_t index, uint8_t flags = 0x00 );

#if defined SERIALISATION_SUPPORT_STDARRAY
    template< size_t Size, typename TSerialisable >
    void StoreContainer( std::array< TSerialisable, Size > &container, uint8_t index, uint8_t flags = 0x00 )
    {
        const size_t size = CreateArray( Type::Object, Size, index, flags );

        if ( size <= Size )
        {
            for ( TSerialisable *it = container, end = it + size; it != end; ++it )
            {
                StoreArrayItem( *it );
            }
        }
        else
        {
            for ( TSerialisable *it = container, end = it + Size; it != end; ++it )
            {
                StoreArrayItem( *it );
            }

            TSerialisable dummy{};

            for ( size_t i = Size; i < size; ++i )
            {
                StoreArrayItem( dummy );
            }
        }
    }

    template< size_t Size >
    void StoreContainer( std::array< uint8_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< uint16_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< uint32_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< uint64_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< int8_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< int16_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< int32_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< int64_t, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< float, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }

    template< size_t Size >
    void StoreContainer( std::array< double, Size > &container, uint8_t index, uint8_t flag = 0x00 )
    {
        StoreFixedSize< Size >( container.data(), index, flag );
    }
#endif

private:

    InternalMessage *mInternalMessage;

    Mode::Mode mMode;
    Format::Format mFormat;

    template< typename TSerialisable >
    void Store( TSerialisable &serialisable )
    {
        mInternalMessage->InitObject();

        MessageHelper::SERIALISATION_CUSTOM_INTERFACE( serialisable, *this );

        mInternalMessage->FinishObject();
    }

    template< typename TArgument >
    InternalMessage *CreateInternalMessage( Format::Format format, Mode::Mode mode, TArgument &argument, bool useBuffer )
    {
        InternalMessage *iMessage = NULL;

        if ( mode == Mode::Serialise )
        {
            iMessage = CreateSerMessage( format, argument, useBuffer );
        }
        else
        {
            iMessage = CreateDeserMessage( format, argument, useBuffer );
        }

        return iMessage;
    }

    template< typename TArgument >
    InternalMessage *CreateSerMessage( Format::Format format, TArgument &argument, bool useBuffer )
    {
        if ( useBuffer )
        {
            return CreateSerMessage< BufferedStreamWriter >( format, argument );
        }
        else
        {
            return CreateSerMessage< StreamWriter >( format, argument );
        }
    }

    template< typename TStreamWriter, typename TArgument  >
    InternalMessage *CreateSerMessage( Format::Format format, TArgument &argument )
    {
        InternalMessage *iMessage = NULL;

        switch ( format )
        {
        case Format::Binary:
            iMessage = new MessageAdapter< BinarySerMessage< TStreamWriter > >( argument );
            break;

        default:
            assert( false && "Something went terribly haywire..." );
            break;
        }

        return iMessage;
    }

    template< typename TArgument >
    InternalMessage *CreateDeserMessage( Format::Format format, TArgument &argument, bool useBuffer )
    {
        if ( useBuffer )
        {
            return CreateDeserMessage< BufferedStreamReader >( format, argument );
        }
        else
        {
            return CreateDeserMessage< StreamReader >( format, argument );
        }
    }

    template< typename TStreamReader, typename TArgument  >
    InternalMessage *CreateDeserMessage( Format::Format format, TArgument &argument )
    {
        InternalMessage *iMessage = NULL;

        switch ( format )
        {
        case Format::Binary:
            iMessage = new MessageAdapter< BinaryDeserMessage< TStreamReader > >( argument );
            break;

        default:
            assert( false && "Something went terribly haywire..." );
            break;
        }

        return iMessage;
    }

    template< typename TMessage >
    void DeleteInternalMessage( TMessage *message )
    {
        delete message;
    }

    template< typename TSerialisable >
    inline void StoreArrayItem( TSerialisable &value )
    {
        mInternalMessage->InitArrayObject();

        MessageHelper::SERIALISATION_CUSTOM_INTERFACE( value, *this );

        mInternalMessage->FinishArrayObject();
    }

    void StoreArrayItem( ISerialisable &value );

    void StoreArrayItem( std::string &value );

    void StoreArrayItem( uint8_t &value );

    void StoreArrayItem( uint16_t &value );

    void StoreArrayItem( uint32_t &value );

    void StoreArrayItem( uint64_t &value );

    void StoreArrayItem( int8_t &value );

    void StoreArrayItem( int16_t &value );

    void StoreArrayItem( int32_t &value );

    void StoreArrayItem( int64_t &value );

    void StoreArrayItem( float &value );

    void StoreArrayItem( double &value );

    size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 );

    template< typename TPrimitive >
    void StorePrimitiveVector( std::vector< TPrimitive > &container, uint8_t index, uint8_t flags )
    {
        const Type::Type type = static_cast< Type::Type >( Internal::Type::GetEnum< TPrimitive >() );
        const size_t size = mInternalMessage->CreateArray( type, container.size(), index, flags );
        container.resize( size );

        if ( size > 0 )
        {
            mInternalMessage->StoreContiguous( &container[0], size );
        }
    }

    template< size_t Size, typename TPrimitive >
    void StorePrimitveCArray( TPrimitive( &container )[ Size ], uint8_t index, uint8_t flags )
    {
        StoreFixedSize< Size, TPrimitive >( container, index, flags );
    }

    template< size_t Size, typename TPrimitive >
    void StoreFixedSize( TPrimitive *container, uint8_t index, uint8_t flags )
    {
        const Type::Type type = static_cast< Type::Type >( Internal::Type::GetEnum< TPrimitive >() );
        const size_t size = CreateArray( type, Size, index, flags );

        assert( size == Size && "The array you tried to Deserialise with seems to be the wrong size" );
        mInternalMessage->StoreContiguous( container, size );
    }
};

namespace MessageHelper
{
    template< typename TSerialisable >
    void Store( Message &message, TSerialisable &serialisable, bool clearBuffers /*= true*/ )
    {
        message.Store( serialisable );

        if ( clearBuffers )
        {
            message.ClearBuffers();
        }
    }
}

#ifndef SERIALISATION_NO_HEADER_ONLY
#   include "../../src/message.cpp"
#endif

#endif
