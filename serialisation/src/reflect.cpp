#include "reflect.h"


const std::string &Reflect::Get( const uint32_t index ) const
{
    Map::const_iterator it = mNames.find( index );
    assert( it != mNames.end() );
    return it->second;
}

void Reflect::Set( const uint32_t index, const std::string &name )
{
    mNames.insert( std::pair< uint32_t, std::string >( index, name ) );
}

bool Reflect::Exists( const uint32_t index ) const
{
    return mNames.find( index ) != mNames.end();
}
