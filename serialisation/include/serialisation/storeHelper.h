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
#ifndef __SERIALISATION_STOREHELPER_H__
#define __SERIALISATION_STOREHELPER_H__

class BinarySerMessage;

struct StoreHelper
{
    template< typename Container, typename TMessage >
    static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags = 0x00 )
    {
        ContainerHelper< Container, typename TMessage, Container::value_type >::StoreContainer( message, container, index, flags );
    }

    template< typename Container, typename TMessage, typename TSerialisable >
    struct ContainerHelper
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            container.resize( message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< Container::value_type >() ),
                                                   container.size(), index, flags ) );

            for ( Container::iterator it = container.begin(), end = container.end(); it != end; ++it )
            {
                message.StoreArrayItem( *it );
            }
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, uint8_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, uint8_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, uint16_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, uint16_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, uint32_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, uint32_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, uint64_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, uint64_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, int8_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, int8_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, int16_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, int16_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, int32_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, int32_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename TMessage >
    struct ContainerHelper< Container, TMessage, int64_t >
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, int64_t, TMessage >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename Primitive, typename TMessage >
    struct PrimitiveContainerHelper
    {
        static void StoreContainer( TMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            container.resize( message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< Container::value_type >() ),
                                                   container.size(), index, flags ) );

            for ( Container::iterator it = container.begin(), end = container.end(); it != end; ++it )
            {
                message.StoreArrayItem( *it );
            }
        }
    };

    template< typename Container, typename Primitive >
    struct PrimitiveContainerHelper< Container, Primitive, BinarySerMessage >
    {
        static void StoreContainer( BinarySerMessage &message, Container &container, uint8_t index, uint8_t flags )
        {
            message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< Primitive >() ), container.size(), index, flags );

            message.mStreamBuffer.WriteBytes( &container.front(), container.size() * sizeof( Primitive ) );
        }
    };

    //          size_t size = static_cast< uint32_t >( container.size() );
    //
    //          size = message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< Primitive >() ), size, index, flags );
    //
    //          container.resize( size );
    //
    //          if ( message.GetMode() == Mode::Serialise )
    //          {
    //              message.mStreamBuffer.WriteBytes( &container.front(), size * sizeof( Primitive ) );
    //          }
    //          else
    //          {
    //              if ( message.mCurrentArray )
    //              {
    //                  memcpy( &container.front(), static_cast< TempArray< Primitive > * >( message.mCurrentArray )->GetData(),
    //                      size * sizeof( Primitive ) );
    //                  message.mCurrentArray->Resize( 0 );
    //              }
    //              else if ( message.GetMode() == Internal::Mode::Deserialise && !message.mCurrentObject->GetTerminatorRead() )
    //              {
    //                  message.mStreamBuffer.ReadBytes( &container.front(), size * sizeof( Primitive ) );
    //              }
    //          }
    //         }
    //     };
};

#endif