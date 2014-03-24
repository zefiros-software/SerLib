#include "binarySerialiser.h"
#include "util.h"
#include "varint.h"
#include "varIntData.h"
#include "message.h"
#include "repeatedData.h"


BinarySerialiser::BinarySerialiser( std::ostream &stream )
    : mStream( &stream )
{

}

void BinarySerialiser::SerialiseMessage( Message &message )
{
    Serialise( &message );
}

void BinarySerialiser::Prepare( ISerialiseData *const data, const uint32_t index, const uint32_t flags )
{
    Internal::Type::Type type = data->GetType();

    if ( ( flags & Message::Packed ) &&
            Internal::Type::IsInteger( type ) &&
            type != Internal::Type::UInt8 &&
            type != Internal::Type::SInt8 )
    {
        type = Internal::Type::VarInt;
    }
    else if ( Internal::Type::IsSignedInt( type ) )
    {
        type = ( Internal::Type::Type )( type - Internal::Type::SInt8 + Internal::Type::UInt8 );
    }
    else if ( type == Internal::Type::Float )
    {
        type = Internal::Type::UInt32;
    }
    else if ( type == Internal::Type::Double )
    {
        type = Internal::Type::UInt64;
    }

    WriteHeader( index, type );

    if ( type == Internal::Type::Repeated )
    {
        AbstractRepeatedData *const repeated = static_cast< AbstractRepeatedData * >( data );
        Prepare( repeated->GetSerialisable( 0 ), repeated->Count(), flags );
    }
}

void BinarySerialiser::Serialise( Message *const message )
{
    uint32_t count = message->GetMemberCount();
    VarInt< uint32_t >( count ).WriteToStream( *mStream );

    for ( uint32_t i = 0; i < count; ++i )
    {
        std::pair< uint32_t, ISerialiseData * > data = AbstractSerialiser::GetFromMessage( message, i );

        ISerialiseData *sData = data.second;
        Prepare( sData, data.first, sData->GetFlags() );

        AbstractSerialiser::Serialise( data.second );
    }
}

void BinarySerialiser::Serialise( SerialiseData< std::string > *const data )
{
    VarInt< size_t > size( data->GetValue().size() );
    size.WriteToStream( *mStream );

    const std::string &value = data->GetValue();
    mStream->write( value.c_str(), value.length() );
}

void BinarySerialiser::Serialise( SerialiseData< uint8_t > *const data )
{
    SerialiseNum( data->GetValue(), data->GetFlags() & ~Message::Packed );
}

void BinarySerialiser::Serialise( SerialiseData< uint16_t > *const data )
{
    SerialiseNum( data->GetValue(), data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< uint32_t > *const data )
{
    SerialiseNum( data->GetValue(), data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< uint64_t > *const data )
{
    SerialiseNum( data->GetValue(), data->GetFlags() );
}

void BinarySerialiser::Serialise( VarIntData *const data )
{
    SerialiseNum( data->GetValue(), data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< int8_t > *const data )
{
    uint8_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() & ~Message::Packed );
}

void BinarySerialiser::Serialise( SerialiseData< int16_t > *const data )
{
    uint16_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< int32_t > *const data )
{
    uint32_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< int64_t > *const data )
{
    uint64_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() );
}

void BinarySerialiser::Serialise( SerialiseData< float > *const data )
{
    uint32_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() & ~Message::Packed );
}

void BinarySerialiser::Serialise( SerialiseData< double > *const data )
{
    uint64_t value;
    data->Store( value, Mode::Deserialise );
    SerialiseNum( value, data->GetFlags() & ~Message::Packed );
}

void BinarySerialiser::Serialise( AbstractRepeatedData *const data )
{
    for ( uint32_t i = 0, end = data->Count(); i < end; ++i )
    {
        AbstractSerialiser::Serialise( data->GetSerialisable( i ) );
    }
}

void BinarySerialiser::WriteHeader( uint32_t index, Internal::Type::Type type )
{
    VarInt< uint64_t > vi( Util::CreateHeader( index, type ) );
    vi.WriteToStream( *mStream );
}