#pragma once
#ifndef __SERIALISATION_STORE_H__
#define __SERIALISATION_STORE_H__

#include <algorithm>

namespace Store
{
    template< typename Container >
    void StoreContainer( Message &message, Container &container, const uint32_t index, const uint32_t flags = 0x00 )
    {
		typedef Container::value_type T;

        uint32_t size = ( uint32_t )container.size();
        message.CreateRepeated( ( Type::Type )Internal::Type::GetEnum< T >(), size, index, flags );

        std::vector< T > &values = static_cast< RepeatedData< T > * >( message.FindSerialisable( index ) )->GetValues();

        Mode::Mode mode = message.GetMode();

        if ( mode == Mode::Serialise )
        {
            std::copy( container.begin(), container.end(), values.begin() );
        }
        else
        {
            std::copy( values.begin(), values.end(), container.begin() );
        }
    };
};

#endif