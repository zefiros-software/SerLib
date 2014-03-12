#include "types.h"
#include "numData.h"

template<>
Type::Type Type::GetEnum< NumData< uint8_t > >()
{
	return Type::Char;
}

template<>
Type::Type Type::GetEnum< VarIntData >()
{
	return Type::VarInt;
}

template<>
Type::Type Type::GetEnum< StringData >()
{
	return Type::String;
}

template<>
Type::Type Type::GetEnum< Message >()
{
	return Type::Message;
}

template<>
Type::Type Type::GetEnum< NumData< uint16_t > >()
{
	return Type::WORD;
}

template<>
Type::Type Type::GetEnum< NumData< uint32_t > >()
{
	return Type::DWORD;
}

template<>
Type::Type Type::GetEnum< NumData< uint64_t > >()
{
	return Type::QWORD;
}