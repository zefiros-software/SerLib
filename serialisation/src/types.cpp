#include "types.h"

bool Internal::Type::IsInteger( Internal::Type::Type type )
{
	return type >= Type::UInt8 && type <= Type::SInt64;
}

bool Internal::Type::IsSignedInt( Internal::Type::Type type )
{
	return type >= Type::SInt8 && type <= Type::SInt64;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< Message >()
{
	return Type::Variable;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< std::string > >()
{
	return Internal::Type::String;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< uint8_t > >()
{
	return Type::UInt8;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< uint16_t > >()
{
	return Type::UInt16;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< uint32_t > >()
{
	return Type::UInt32;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< uint64_t > >()
{
	return Type::UInt64;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< VarIntData >()
{
	return Type::VarInt;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< int8_t > >()
{
	return Type::SInt8;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< int16_t > >()
{
	return Type::SInt16;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< int32_t > >()
{
	return Type::SInt32;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< int64_t > >()
{
	return Type::SInt64;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< float > >()
{
	return Type::Float;
}

template<>
Internal::Type::Type Internal::Type::GetEnum< SerialiseData< double > >()
{
	return Type::Double;
}
