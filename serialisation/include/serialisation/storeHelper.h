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

struct StoreHelper
{
    template< typename Container >
    static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags = 0x00 )
    {
        message.StoreContainer( container, index, flags );
    }

    template< typename Container >
    static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags = 0x00 )
    {
        ContainerHelper< Container, Container::value_type >::StoreContainer( message, container, index, flags );
    }

    template< typename Container, typename TObject >
    struct ContainerHelper
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            size_t size = container.size();

            size = message.CreateArray( Type::Object, size, index, flags );

            container.resize( size );

            if ( message.GetMode() == Mode::Serialise )
            {
                for ( Container::iterator it = container.begin(), end = container.end(); it != end; ++it )
                {
                    message.StoreArrayItem( *it );
                }
            }
            else
            {
                if ( message.mCurrentArray )
                {
                    //                    TempArray< ISerialisable * > *const tArray = static_cast< TempArray< ISerialisable * > * >( message.mCurrentArray );

                    for ( Container::iterator it = container.begin(), end = container.end(); it != end; ++it )
                    {
                        //tArray->PopFront( &*it );
                    }
                }
                else if ( message.GetMode() == Internal::Mode::Deserialise && !message.mCurrentObject->GetTerminatorRead() )
                {
                    for ( Container::iterator it = container.begin(), end = container.end(); it != end; ++it )
                    {
                        message.StoreArrayItem( *it );
                    }
                }
            }
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, uint8_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, uint16_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, uint32_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper< Container, uint32_t >::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, uint64_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, int8_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, int16_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, int32_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container >
    struct ContainerHelper< Container, int64_t >
    {
        static void StoreContainer( Message &message, Container &container, uint8_t index, uint8_t flags )
        {
            PrimitiveContainerHelper::StoreContainer( message, container, index, flags );
        }
    };

    template< typename Container, typename Primitive >
    struct PrimitiveContainerHelper
    {
        static void StoreContainer( Message &message, Container &container, uint32_t index, uint8_t flags )
        {
            size_t size = static_cast< uint32_t >( container.size() );

            size = message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< Primitive >() ), size, index, flags );

            container.resize( size );

            if ( message.GetMode() == Mode::Serialise )
            {
                message.mStreamBuffer.WriteBytes( &container.front(), size * sizeof( Primitive ) );
            }
            else
            {
                if ( message.mCurrentArray )
                {
                    memcpy( &container.front(), static_cast< TempArray< Primitive > * >( message.mCurrentArray )->GetData(),
                            size * sizeof( Primitive ) );
                    message.mCurrentArray->Resize( 0 );
                }
                else if ( message.GetMode() == Internal::Mode::Deserialise && !message.mCurrentObject->GetTerminatorRead() )
                {
                    message.mStreamBuffer.ReadBytes( &container.front(), size * sizeof( Primitive ) );
                }
            }
        }
    };
};

#endif