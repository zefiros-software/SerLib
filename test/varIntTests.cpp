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

#include "helper.h"

#include "serialisation/writeBuffer.h"
#include "serialisation/readBuffer.h"

#include "gtest/gtest.h"

#include <climits>
#include <sstream>

template< typename T >
T VarIntConvert( T value )
{
    std::stringstream ss;

    WriteBuffer wb( ss );
    wb.WriteSize( value );
	wb.ClearBuffer();

    ReadBuffer rb( ss );
    return static_cast< T >( rb.ReadSize() );
}

#define TestVarInt( type )                                      \
    TEST( P( VarIntTest ), type ## Zebra )                      \
    {                                                           \
        const type ul = GenerateZebraValue< type >();           \
        ASSERT_EQ( ul, VarIntConvert(ul) );                     \
    }                                                           \
    \
    TEST( P( VarIntTest ), type ## Min )                        \
    {                                                           \
        const type ul = std::numeric_limits<uint8_t>::min();    \
        ASSERT_EQ( ul, VarIntConvert(ul) );                     \
    }                                                           \
    \
    TEST( P( VarIntTest ), type ## Max )                        \
    {                                                           \
        const type ul = std::numeric_limits<type>::max();    \
        ASSERT_EQ( ul, VarIntConvert( ul ) );                   \
    }                                                           \
    \
    TEST( P( VarIntTest), type ## Random )                      \
    {                                                           \
        for(uint32_t i = 0; i < 1000; ++i )                     \
        {                                                       \
            const type ul = static_cast< type >( std::rand() ); \
            ASSERT_EQ( ul, VarIntConvert(ul));                  \
        }                                                       \
    }

TestVarInt( uint8_t );
TestVarInt( uint16_t );
TestVarInt( uint32_t );
TestVarInt( size_t );