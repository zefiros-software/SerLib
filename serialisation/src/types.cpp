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
