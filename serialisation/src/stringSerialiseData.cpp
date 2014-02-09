#include "stringSerialiseData.h"
#include "varint.h"
#include "util.h"



void StringSerialiseData::SetValue( const std::string &str )
{
    mString = str;;
}

const std::string & StringSerialiseData::GetValue() const
{
    return mString;
}

Type::Type StringSerialiseData::GetType() const
{
    return Type::String;
}

size_t StringSerialiseData::Size() const
{
    return mString.size() + Util::CalculateVarIntSize( mString.size() );
}

void StringSerialiseData::ReadFromStream( std::istream &stream )
{
    VarInt< uint32_t > length;
    length.ReadFromStream( stream );
    mString.resize( length.GetValue() );
    stream.read( &mString.front(), length.GetValue() );
}

void StringSerialiseData::WriteToStream( std::ostream &stream ) const
{
    VarInt< uint32_t > length( mString.length() );
    length.WriteToStream( stream );
    stream.write( &mString.front(), mString.size() );
}
