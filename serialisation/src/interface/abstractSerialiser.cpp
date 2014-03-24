#include "interface/abstractSerialiser.h"
#include "message.h"

void AbstractSerialiser::Serialise( ISerialiseData *const data )
{
	data->SerialiseTo( this );
}

std::pair< uint32_t, ISerialiseData * > AbstractSerialiser::GetFromMessage( Message *const message, const uint32_t index )
{
	uint32_t i = message->mIndexes[ index ];
	return std::pair< uint32_t, ISerialiseData * >( i, message->FindSerialisable( i ) );
}
