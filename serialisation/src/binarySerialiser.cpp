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

#include "message.h"
#include "binarySerialiser.h"
#include "repeatedData.h"
#include "varIntData.h"

BinarySerialiser::BinarySerialiser( std::ostream &stream )
    : mStream( &stream ),
      mBufferIndex( 0 ),
      mBufferSize( sizeof( mBuffer ) )
{
}

void BinarySerialiser::SerialiseMessage( Message &message )
{
    Serialise( &message );
    FlushBuffer();
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
    const uint32_t count = message->GetMemberCount();
    WriteVarInt( count );

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
    const std::string &value = data->GetValue();
    const size_t size = value.size();
    WriteVarInt( size );
    WriteBytes( value.c_str(), size );
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
    WriteVarInt( Util::CreateHeader( index, type ) );
}