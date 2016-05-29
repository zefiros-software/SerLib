#include "serialisation/defines.h"
#include "serialisation/types.h"

SERIALISATION_INLINE bool Internal::Type::IsInteger( Type type )
{
    return ( type >= Type::UInt8 ) && ( type <= SInt64 );
}

SERIALISATION_INLINE bool Internal::Type::IsSignedInt( Type type )
{
    return ( type >= Type::SInt8 ) && ( type <= Type::SInt64 );
}

SERIALISATION_INLINE bool Internal::Type::AreCompatible( const Type type, const Type type2 )
{
    if ( type == type2 )
    {
        return true;
    }

    if ( ( IsSignedInt( type ) && type - SInt8 == type2 - UInt8 ) || ( IsSignedInt( type2 ) &&
                                                                       type2 - SInt8 == type - UInt8 ) )
    {
        return true;
    }


    if ( ( type == Float && type2 == UInt32 ) || ( type2 == Float && type == UInt32 ) )
    {
        return true;
    }

    if ( ( type == Double && type2 == UInt64 ) || ( type2 == Double && type == UInt64 ) )
    {
        return true;
    }

    if ( IsInteger( type ) && IsInteger( type2 ) )
    {
        return true;
    }

    return false;
}