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

Type StringSerialiseData::GetType() const
{
    return Type::String;
}

size_t StringSerialiseData::Size() const
{
    return mString.size() + Util::CalculateVarIntSize( mString.size() );
}

void StringSerialiseData::ReadFromStream( std::istream &stream )
{
    VarInt< U32 > length;
    length.ReadFromStream( stream );
    mString.resize( length.GetValue() );
    stream.read( &mString.front(), length.GetValue() );
}

void StringSerialiseData::WriteToStream( std::ostream &stream ) const
{
    VarInt< U32 > length( mString.length() );
    length.WriteToStream( stream );
    stream.write( &mString.front(), mString.size() );
}
