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

#pragma once
#ifndef __SERIALISATION_TYPES_H__
#define __SERIALISATION_TYPES_H__

#include <stdint.h>

class VarIntData;
class StringData;
class Message;

template< typename T >
class NumData;

namespace Type
{
    // underlying type is uint8_t
    enum Type
    {
        Message   = 0x00,
        String    = 0x01,
        Char      = 0x02,
        WORD      = 0x03,
        DWORD     = 0x04,
        QWORD     = 0x05,
        VarInt    = 0x06,
        Repeated  = 0x07
    };

    template< typename T >
    Type GetEnum()
    {
        return Type::Repeated;
	}

	template<>
	Type GetEnum< StringData >();

	template<>
	Type GetEnum< VarIntData >();

	template<> 
	Type GetEnum< NumData< uint8_t > >();

	template<>
	Type GetEnum< NumData< uint16_t > >();

	template<>
	Type GetEnum< NumData< uint32_t > >();

	template<>
	Type GetEnum< NumData< uint64_t > >();
}

namespace Mode
{
    enum Mode
    {
        Serialise = 0x00,
        Deserialise = 0x01
    };
}

#endif