#include "varIntSerialiseData.h"
#include "util.h"



Type VarIntSerialiseData::GetType() const
{
    return Type::VarInt;
}

size_t VarIntSerialiseData::Size() const
{
    return Util::CalculateVarIntSize( mValue );
}

void VarIntSerialiseData::ReadFromStream( std::istream &stream )
{
    VarInt< U64 > val;
    val.ReadFromStream( stream );
    mValue = val.GetValue();
}

void VarIntSerialiseData::WriteToStream( std::ostream &stream ) const
{
    VarInt< U64 > val( mValue );
    val.WriteToStream( stream );
}
