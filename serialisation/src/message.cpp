#include "message.h"
#include "repeatedSerialiseData.h"
#include "util.h"
#include <fstream>

Message::Message( Mode mode /*= Mode::Serialise */ )
    : mMode( mode )
{
}

void Message::SetMode( Mode mode )
{
    mMode = mode;
}

Message::Mode Message::GetMode() const
{
    return mMode;
}

Type::Type Message::GetType() const
{
    return Type::Message;
}

size_t Message::Size() const
{
    size_t size = 0;

    for ( auto it = mSerialisables.begin(), end = mSerialisables.end(); it != end; ++it )
    {
        const ISerialiseData *data = it->second;
        size += data->Size();
        size += Util::CalculateVarIntSize( Util::CreateHeader( ( uint64_t )it->first, data->GetType() ) );
    }

    return size;
}

void Message::Store( int8_t &value, const uint32_t index /* = 0 */, const uint32_t /* = 0 */ )
{
    StoreNum< int8_t, CharSerialiseData, Type::Char >( &value, index );
}

void Message::Store( uint8_t &value, const uint32_t index /*= 0*/, const uint32_t /*= 0*/ )
{
    StoreNum< uint8_t, CharSerialiseData, Type::Char >( &value, index );
}

void Message::Store( int16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int16_t, uint16_t, WORDSerialiseData, Type::WORD >( &value, index, flags );
}

void Message::Store( uint16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint16_t, WORDSerialiseData, Type::WORD >( &value, index, flags );
}

void Message::Store( int32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int32_t, uint32_t, DWORDSerialiseData, Type::DWORD >( &value, index, flags );
}

void Message::Store( uint32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( &value, index, flags );
}

void Message::Store( int64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int64_t, uint64_t, DWORDLONGSerialiseData, Type::QWORD >( &value, index, flags );
}

void Message::Store( uint64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint64_t, DWORDLONGSerialiseData, Type::QWORD >( &value, index, flags );
}


void Message::Store( float &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    uint32_t flexman = Util::FloatToUInt32( value );
    Store( flexman, index, flags );

    if ( mMode == Mode::Deserialise )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

void Message::Store( double &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    uint64_t flexman = Util::DoubleToUInt64( value );
    Store( flexman, index, flags );

    if ( mMode == Mode::Deserialise )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}


void Message::Store( std::string &string, const uint32_t index /* = 0 */, const uint32_t /* = 0 */ )
{
    switch ( mMode )
    {
    case Mode::Serialise:
        {
            StringSerialiseData *const data = GetSerialisable< StringSerialiseData, Type::String >( index );
            data->SetValue( string );
        }
        break;

    case Mode::Deserialise:
        {
            auto it = mSerialisables.find( index );
            assert( it != mSerialisables.end() );

            ISerialiseData *const iData = it->second;

            assert( iData->GetType() == Type::String );

            string = static_cast< StringSerialiseData *const >( iData )->GetValue();
        }
        break;
    }
}

void Message::StoreObject( ISerialisable *const serialisable, uint32_t index )
{
    switch ( mMode )
    {
    case Mode::Serialise:
        {
            Message *const message = GetSerialisable< Message, Type::Message >( index );
            message->SetMode( mMode );
            serialisable->OnSerialise( *message );
            break;
        }

    case Mode::Deserialise:
        {
            auto it = mSerialisables.find( index );
            assert( it != mSerialisables.end() );

            ISerialiseData *const iData = it->second;

            assert( iData->GetType() == Type::Message );
            Message *const message = static_cast< Message *const >( iData );
            message->SetMode( mMode );

            serialisable->OnSerialise( *message );
        }
        break;
    }
}

void Message::WriteToFile( const std::string &fileName ) const
{
    std::ofstream stream( fileName, std::ios::binary );

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

    for ( auto it = mSerialisables.begin(), end = mSerialisables.end(); it != end; ++it )
    {
        const ISerialiseData *data = it->second;

        ::VarInt< uint64_t > header( Util::CreateHeader( ( uint64_t )it->first, data->GetType() ) );
        header.WriteToStream( stream );
        data->WriteToStream( stream );
    }
}

void Message::ReadFromStream( std::istream &stream )
{
    ::VarInt< uint64_t > size;
    size.ReadFromStream( stream );
    int64_t end = ( int64_t )stream.tellg() + size.GetValue();

    while ( stream.tellg() < end )
    {
        uint64_t vHeader;

        {
            ::VarInt< uint64_t > header;
            header.ReadFromStream( stream );
            vHeader = header.GetValue();
        }

        ISerialiseData *data = GetSerialisable( Util::GetHeaderIndex( vHeader ), Util::GetHeaderType( vHeader ) );

        data->ReadFromStream( stream );
    }
}

ISerialiseData *const Message::GetSerialisable( const uint32_t index, Type::Type type )
{

    switch ( type )
    {
    case Type::Message:
        return GetSerialisable< Message, Type::Message >( index );
        break;

    case Type::String:
        return GetSerialisable< StringSerialiseData, Type::String >( index );
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
        return GetSerialisable< DWORDLONGSerialiseData, Type::QWORD >( index );
        break;

    case Type::VarInt:
        return GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );
        break;
    }

    return NULL;
}
