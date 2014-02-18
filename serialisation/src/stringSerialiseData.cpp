#include "stringSerialiseData.h"
#include "varint.h"
#include "util.h"

void StringSerialiseData::Store( std::string &str, Mode::Mode mode )
{
    switch ( mode )
    {
    case Mode::Serialise:
        mString = str;
        break;

    case Mode::Deserialise:
        str = mString;
        break;
    }
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
    VarInt< size_t > length;
    length.ReadFromStream( stream );
    mString.resize( length.GetValue() );
    stream.read( &*mString.begin(), length.GetValue() );
}

void StringSerialiseData::WriteToStream( std::ostream &stream ) const
{
    VarInt< size_t > length( mString.length() );
    length.WriteToStream( stream );
    stream.write( &*mString.begin(), mString.size() );
}
