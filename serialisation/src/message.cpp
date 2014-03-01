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

#include "interface/ISerialisable.h"
#include "defines.h"
#include "message.h"
#include "varint.h"

Message::Message( Mode::Mode mode /*= Mode::Serialise */ )
    : mMode( mode )
{
}

void Message::SetMode( Mode::Mode mode )
{
    mMode = mode;
}

Mode::Mode Message::GetMode() const
{
    return mMode;
}

size_t Message::Size() const
{
    size_t size = 0;

    for ( std::map< uint32_t, ISerialiseData * >::const_iterator it = mSerialisables.begin(), end = mSerialisables.end();
            it != end; ++it )
    {
        const ISerialiseData *data = it->second;
        size += data->Size();
        size += Util::CalculateVarIntSize( Util::CreateHeader( ( uint64_t )it->first, data->GetType() ) );
    }

    return size;
}

Type::Type Message::GetType() const
{
    return Type::Message;
}

void Message::Store( int8_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreSNum< int8_t, uint8_t, CharSerialiseData, Type::Char >( value, index, flags & ~Packed );
}

void Message::Store( uint8_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreUNum< uint8_t, CharSerialiseData, Type::Char >( value, index, flags & ~Packed );
}

void Message::Store( int16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreSNum< int16_t, uint16_t, WORDSerialiseData, Type::WORD >( value, index, flags );
}

void Message::Store( uint16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreUNum< uint16_t, WORDSerialiseData, Type::WORD >( value, index, flags );
}

void Message::Store( int32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreSNum< int32_t, uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, flags );
}

void Message::Store( uint32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, flags );
}

void Message::Store( int64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreSNum< int64_t, uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, flags );
}

void Message::Store( uint64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    StoreUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, flags );
}

void Message::Store( float &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;
    Store( flexman, index, flags );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

void Message::Store( double &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;
    Store( flexman, index, flags );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

void Message::Store( std::string &string, const uint32_t index /*= 0*/, const uint32_t /*= 0 */ )
{
    StringData *const data = GetSerialisable< StringData, Type::String >( index );

    if ( data )
    {
        data->Store( string, mMode );
    }
}

void Message::Store( ISerialisable *const serialisable, uint32_t index /*= 0 */ )
{
    Message *const message = GetSerialisable< Message, Type::Message >( index );

    if ( message )
    {
        message->SetMode( mMode );
        serialisable->SERIALISATION_CUSTOM_INTERFACE( *message );
    }
}

uint32_t Message::Count( const uint32_t index ) const
{
    std::map< uint32_t, ISerialiseData * >::const_iterator it = mSerialisables.find( index );

    assert( it != mSerialisables.end() );

    ISerialiseData *const data = it->second;

    if ( data->GetType() == Type::Repeated )
    {
        return static_cast< AbstractRepeatedData *const >( data )->GetFieldCount();
    }

    return 1;
}

uint32_t Message::GetMemberCount() const
{
    return ( uint32_t )mSerialisables.size();
}

void Message::CreateRepeated( Type::Type type, uint32_t size, const uint32_t index /*= 0*/,
                                     const uint32_t flags /*= 0 */ )
{
    if ( mMode == Mode::Serialise )
    {
        GetRepeated( index, type, flags )->Resize( size );
    }
}

void Message::StoreRepeated( uint8_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t /*= 0 */ )
{
    StoreRepeated< uint8_t, CharSerialiseData, Type::Char >( value, index, repeatedIndex );
}

void Message::StoreRepeated( int8_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t /*= 0 */ )
{
    StoreRepeated< int8_t, CharSerialiseData, Type::Char >( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint16_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint16_t, WORDSerialiseData, Type::WORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int16_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int16_t, uint16_t, WORDSerialiseData, Type::WORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( uint32_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int32_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int32_t, uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( uint64_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int64_t &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int64_t, uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( float &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;
    StoreRepeatedUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( flexman, index, repeatedIndex, flags );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

void Message::StoreRepeated( double &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;
    StoreRepeatedUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( flexman, index, repeatedIndex, flags );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

void Message::StoreRepeated( std::string &value, const uint32_t index, const uint32_t repeatedIndex,
                                    const uint32_t /*= 0 */ )
{
    StoreRepeated< std::string, StringData, Type::String >( value, index, repeatedIndex );
}

void Message::StoreRepeated( ISerialisable *const serialisable, uint32_t index /*= 0 */ )
{
    assert( false );
}

void Message::WriteToFile( const std::string &fileName ) const
{
    std::ofstream stream( fileName.c_str(), std::ios::binary );

    if ( stream.is_open() )
    {
        WriteToStream( stream );
        stream.close();
    }
}

void Message::WriteToStream( std::ostream &stream ) const
{
    ::VarInt< uint64_t > size( Size() );
    size.WriteToStream( stream );

    for ( std::map< uint32_t, ISerialiseData * >::const_iterator it = mSerialisables.begin(), end = mSerialisables.end();
            it != end; ++it )
    {
        const ISerialiseData *data = it->second;

        Type::Type type = data->GetType();

        {
            ::VarInt< uint64_t > header( Util::CreateHeader( ( uint64_t )it->first, type ) );
            header.WriteToStream( stream );

            if ( type == Type::Repeated )
            {
                const AbstractRepeatedData *const repeatedData = static_cast< const AbstractRepeatedData *const >( data );
                header.SetValue( Util::CreateHeader( repeatedData->GetFieldCount(), repeatedData->GetSubType() ) );
                header.WriteToStream( stream );
            }
        }

        data->WriteToStream( stream );
    }
}

void Message::ReadFromFile( const std::string &fileName )
{
    std::ifstream stream( fileName.c_str(), std::ios::binary );

    if ( stream.is_open() )
    {
        ReadFromStream( stream );
        stream.close();
    }
}

void Message::ReadFromStream( std::istream &stream )
{
    Mode::Mode tempMode = mMode;
    mMode = Mode::Serialise;

    ::VarInt< uint64_t > size;
    size.ReadFromStream( stream );
    int64_t end = ( int64_t )stream.tellg() + size.GetValue();

    while ( stream.tellg() < end )
    {
        uint64_t vHeader;
        ISerialiseData *data = NULL;
        Type::Type type;

        {
            ::VarInt< uint64_t > header;
            header.ReadFromStream( stream );
            vHeader = header.GetValue();

            type = Util::GetHeaderType( vHeader );


            if ( type == Type::Repeated )
            {
                uint32_t index = Util::GetHeaderIndex( vHeader );

                header.ReadFromStream( stream );
                vHeader = header.GetValue();

                data = GetRepeated( index, Util::GetHeaderType( vHeader ) );
                static_cast< AbstractRepeatedData *const >( data )->Resize( Util::GetHeaderIndex( vHeader ) );
            }
            else
            {
                data = GetSerialisable( Util::GetHeaderIndex( vHeader ), type );
            }
        }

        data->ReadFromStream( stream );
    }

    mMode = tempMode;
}

Message *Message::CreateMessage()
{
    return new Message( mMode );
}

ISerialiseData *Message::GetSerialisable( const uint32_t index, Type::Type type )
{
    switch ( type )
    {
    case Type::Repeated:
    case Type::Message:
        return GetSerialisable< Message, Type::Message >( index, &Message::CreateMessage );
        break;

    case Type::String:
        return GetSerialisable< StringData, Type::String >( index );
        break;

    case Type::Char:
        return GetSerialisable< CharSerialiseData, Type::Char >( index );
        break;

    case Type::WORD:
        return GetSerialisable< WORDSerialiseData, Type::WORD >( index );
        break;

    case Type::DWORD:
        return GetSerialisable< DWORDSerialiseData, Type::DWORD >( index );
        break;

    case Type::QWORD:
        return GetSerialisable< QWORDSerialiseData, Type::QWORD >( index );
        break;

    case Type::VarInt:
        return GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );
        break;
    }

    return NULL;
}

AbstractRepeatedData *Message::GetRepeated( const uint32_t index, Type::Type subType, uint32_t flags /*= 0 */ )
{
    AbstractRepeatedData *repeated = NULL;

    bool isVarint = ( ( flags & ( uint32_t )Message::Packed ) && subType >= Type::WORD && subType <= Type::QWORD );

    if ( isVarint )
    {
        repeated = GetSerialisable< RepeatedVarIntData, Type::Repeated >( index );
    }
    else
    {
        switch ( subType )
        {
        case Type::Repeated:
        case Type::Message:
            repeated = GetSerialisable< RepeatedMessage, Type::Repeated >( index );
            break;

        case Type::String:
            repeated = GetSerialisable< RepeatedStringData, Type::Repeated >( index );
            break;

        case Type::Char:
            repeated = GetSerialisable< RepeatedCharData, Type::Repeated >( index );
            break;

        case Type::WORD:
            repeated = GetSerialisable< RepeatedWORDData, Type::Repeated >( index );
            break;

        case Type::DWORD:
            repeated = GetSerialisable< RepeatedDWORDData, Type::Repeated >( index );
            break;

        case Type::QWORD:
            repeated = GetSerialisable< RepeatedQWORDData, Type::Repeated >( index );
            break;

        case Type::VarInt:
            repeated = GetSerialisable< RepeatedVarIntData, Type::Repeated >( index );
            break;
        }
    }

    assert( ( repeated && repeated->GetSubType() == subType ) || ( isVarint && repeated->GetSubType() == Type::VarInt ) );

    return repeated;
}
