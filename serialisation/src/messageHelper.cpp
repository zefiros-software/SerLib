#include "serialisation/messageHelper.h"

SERIALISATION_INLINE void MessageHelper::Store( Message &message, ISerialisable &serialisable )
{
    Store< ISerialisable >( message, serialisable );
}