#include "message.h"
#include "repeatedSerialiseData.h"
#include "util.h"
#include <fstream>

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

Type::Type Message::GetType() const
{
    return Type::Message;
}

size_t Message::Size() const
{
    size_t size = 0;

    for ( std::map< uint32_t, ISerialiseData * >::const_iterator it = mSerialisables.begin(), end = mSerialisables.end(); it != end; ++it )
    {
        const ISerialiseData *data = it->second;
        size += data->Size();
        size += Util::CalculateVarIntSize( Util::CreateHeader( ( uint64_t )it->first, data->GetType() ) );
    }

    return size;
}

void Message::Store( int8_t &value, const uint32_t index /* = 0 */, const uint32_t /* = 0 */ )
{
    StoreSNum< int8_t, uint8_t, CharSerialiseData, Type::Char >( value, index, 0 );
}

void Message::Store( uint8_t &value, const uint32_t index /*= 0*/, const uint32_t /*= 0*/ )
{
    StoreUNum< uint8_t, CharSerialiseData, Type::Char >( value, index, 0 );
}

void Message::Store( int16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int16_t, uint16_t, WORDSerialiseData, Type::WORD >( value, index, flags );
}

void Message::Store( uint16_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint16_t, WORDSerialiseData, Type::WORD >( value, index, flags );
}

void Message::Store( int32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int32_t, uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, flags );
}

void Message::Store( uint32_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, flags );
}

void Message::Store( int64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreSNum< int64_t, uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, flags );
}

void Message::Store( uint64_t &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    StoreUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, flags );
}


void Message::Store( float &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;
    Store( flexman, index, flags );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

void Message::Store( double &value, const uint32_t index /*= 0*/, const uint32_t flags /*= 0*/ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;
    Store( flexman, index, flags );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}


void Message::Store( std::string &string, const uint32_t index /* = 0 */, const uint32_t /* = 0 */ )
{
    StringSerialiseData *const data = GetSerialisable< StringSerialiseData, Type::String >( index );

    if ( data )
    {
        data->Store( string, mMode );
    }
}

void Message::StoreObject( ISerialisable *const serialisable, uint32_t index )
{
    Message *const message = GetSerialisable< Message, Type::Message >( index );

    if ( message )
    {
        message->SetMode( mMode );
        serialisable->OnSerialise( *message );
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

    for ( std::map< uint32_t, ISerialiseData * >::const_iterator it = mSerialisables.begin(), end = mSerialisables.end(); it != end; ++it )
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

ISerialiseData *Message::GetSerialisable( const uint32_t index, Type::Type type )
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
        return GetSerialisable< QWORDSerialiseData, Type::QWORD >( index );
        break;

    case Type::VarInt:
        return GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );
        break;
    }

    return NULL;
}

AbstractRepeatedData *Message::GetRepeated( const uint32_t index, Type::Type subType, uint32_t flags )
{
    AbstractRepeatedData *repeated = NULL;

    bool isVarint = ( ( flags & ( uint32_t ) Message::Packed ) && subType >= Type::WORD && subType <= Type::QWORD );

    if ( isVarint )
    {
        repeated = GetSerialisable< RepeatedVarIntData, Type::Repeated >( index );
    }
    else
    {
        switch ( subType )
        {
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

    assert( repeated && repeated->GetSubType() == subType || ( isVarint && repeated->GetSubType() == Type::VarInt ) );

    return repeated;
}

void Message::CreateRepeated( Type::Type type, uint32_t size, const uint32_t index /*= 0*/,
                              const uint32_t flags /*= 0 */ )
{
    if ( mMode == Mode::Serialise )
    {
        GetRepeated( index, type, flags )->Resize( size );
    }
}

void Message::StoreRepeated( uint8_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t )
{
    StoreRepeated< uint8_t, CharSerialiseData, Type::Char >( value, index, repeatedIndex );
}

void Message::StoreRepeated( int8_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t )
{
    StoreRepeated< int8_t, CharSerialiseData, Type::Char >( value, index, repeatedIndex );
}

void Message::StoreRepeated( uint16_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint16_t, WORDSerialiseData, Type::WORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int16_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int16_t, uint16_t, WORDSerialiseData, Type::WORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( uint32_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int32_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int32_t, uint32_t, DWORDSerialiseData, Type::DWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( uint64_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( int64_t &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    StoreRepeatedSNum< int64_t, uint64_t, QWORDSerialiseData, Type::QWORD >( value, index, repeatedIndex, flags );
}

void Message::StoreRepeated( float &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint32_t flexman = isSerialising ? Util::FloatToUInt32( value ) : 0;
    StoreRepeatedUNum< uint32_t, DWORDSerialiseData, Type::DWORD >( flexman, index, repeatedIndex, flags );

    if ( !isSerialising )
    {
        value = Util::UInt32ToFloat( flexman );
    }
}

void Message::StoreRepeated( double &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t flags /*= 0 */ )
{
    bool isSerialising = mMode == Mode::Serialise;

    uint64_t flexman = isSerialising ? Util::DoubleToUInt64( value ) : 0;
    StoreRepeatedUNum< uint64_t, QWORDSerialiseData, Type::QWORD >( flexman, index, repeatedIndex, flags );

    if ( !isSerialising )
    {
        value = Util::UInt64ToDouble( flexman );
    }
}

void Message::StoreRepeated( std::string &value, const uint32_t index, const uint32_t repeatedIndex, const uint32_t )
{
    StoreRepeated< std::string, StringSerialiseData, Type::String >( value, index, repeatedIndex );
}
