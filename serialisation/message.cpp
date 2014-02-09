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

Mode Message::GetMode() const
{
    return mMode;
}

Type Message::GetType() const
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
        size += Util::CalculateVarIntSize( Util::CreateHeader( ( U64 )it->first, data->GetType() ) );
    }

    return size;
}

void Message::Store( S8 *const val, const U32 index /* = 0 */, const U32 /* = 0 */ )
{
    StoreNum< S8, CharSerialiseData, Type::Char >( val, index );
}

void Message::Store( U8 *const val, const U32 index /*= 0*/, const U32 /*= 0*/ )
{
    StoreNum< U8, CharSerialiseData, Type::Char >( val, index );
}

void Message::Store( S16 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreSNum< S16, U16, WORDSerialiseData, Type::WORD >( val, index, flags );
}

void Message::Store( U16 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreUNum< U16, WORDSerialiseData, Type::WORD >( val, index, flags );
}

void Message::Store( S32 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreSNum< S32, U32, DWORDSerialiseData, Type::DWORD >( val, index, flags );
}

void Message::Store( U32 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreUNum< U32, DWORDSerialiseData, Type::DWORD >( val, index, flags );
}

void Message::Store( S64 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreSNum< S64, U64, DWORDLONGSerialiseData, Type::DWORDLONG >( val, index, flags );
}

void Message::Store( U64 *const val, const U32 index /*= 0*/, const U32 flags /*= 0*/ )
{
    StoreUNum< U64, DWORDLONGSerialiseData, Type::DWORDLONG >( val, index, flags );
}


void Message::Store( std::string *const string, const U32 index /* = 0 */, const U32 /* = 0 */ )
{
    switch ( mMode )
    {
    case Mode::Serialise:
        {
            StringSerialiseData *const data = GetSerialisable< StringSerialiseData, Type::String >( index );
            data->SetValue( *string );
        }
        break;

    case Mode::Deserialise:
        {
            auto it = mSerialisables.find( index );
            assert( it != mSerialisables.end() );

            ISerialiseData *const iData = it->second;

            assert( iData->GetType() == Type::String );

            *string = static_cast< StringSerialiseData *const >( iData )->GetValue();
        }
        break;
    }
}

void Message::StoreObject( ISerialisable *const serialisable, U32 index )
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
    VarInt< U64 > size( Size() );
    size.WriteToStream( stream );

    for ( auto it = mSerialisables.begin(), end = mSerialisables.end(); it != end; ++it )
    {
        const ISerialiseData *data = it->second;

        VarInt< U64 > header( Util::CreateHeader( ( U64 )it->first, data->GetType() ) );
        header.WriteToStream( stream );
        data->WriteToStream( stream );
    }
}

void Message::ReadFromStream( std::istream &stream )
{
    VarInt< U64 > size;
    size.ReadFromStream( stream );
    S64 end = ( S64 )stream.tellg() + size.GetValue();

    while ( stream.tellg() < end )
    {
        U64 vHeader;

        {
            VarInt< U64 > header;
            header.ReadFromStream( stream );
            vHeader = header.GetValue();
        }

        ISerialiseData *data = GetSerialisable( Util::GetHeaderIndex( vHeader ), Util::GetHeaderType( vHeader ) );

        data->ReadFromStream( stream );
    }
}

ISerialiseData *const Message::GetSerialisable( const U32 index, Type type )
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

    case Type::DWORDLONG:
        return GetSerialisable< DWORDLONGSerialiseData, Type::DWORDLONG >( index );
        break;

    case Type::VarInt:
        return GetSerialisable< VarIntSerialiseData, Type::VarInt >( index );
        break;
    }

    return NULL;
}
