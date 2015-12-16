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

#include "serialisation/serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <climits>

#define TestSinglePrimitive( name, type, init1, init2 )  TestPrimitive( SinglePrimitive, name, type, 1, init1, init2 )
#define TestSingleFPrimitive( name, type, init1, init2 ) TestFPrimitive( SinglePrimitive, name, type, 1, init1, init2 )
#define TestSingleDPrimitive( name, type, init1, init2 ) TestDPrimitive( SinglePrimitive, name, type, 1, init1, init2 )

#define TestPrimitive( test, name, type, index, init1, init2 )          \
    TEST( P(test), type ## name ## _stream )                            \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        EXPECT_EQ( c1.mMember, c2.mMember );                            \
        EXPECT_EQ( init1, c2.mMember );                                 \
    }                                                                   \
    TEST( P(test), type ## name ## _file )                              \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseFile( c1, c2 );                       \
        EXPECT_EQ( c1.mMember, c2.mMember );                            \
        EXPECT_EQ( init1, c2.mMember );                                 \
    }                                                                   \
    TEST( P(test), type ## name ## _backwards )                         \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        std::string file = TEST_FILE( test, type ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );            \
        EXPECT_EQ( c1.mMember, c2.mMember );                            \
        EXPECT_EQ( init1, c2.mMember );                                 \
    }

#define TestFPrimitive( test, name, type, index, init1, init2 )         \
    TEST( P(test), type ## name ## _stream )                            \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        ASSERT_FLOAT_EQ( c1.mMember, c2.mMember );                      \
        ASSERT_FLOAT_EQ( init1, c2.mMember );                           \
    }                                                                   \
    TEST( P(test), type ## name ## _file )                              \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseFile( c1, c2 );                       \
        ASSERT_FLOAT_EQ( c1.mMember, c2.mMember );                      \
        ASSERT_FLOAT_EQ( init1, c2.mMember );                           \
    }                                                                   \
    TEST( P(test), type ## name ## _backwards )                         \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        std::string file = TEST_FILE( test, type ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );            \
        ASSERT_FLOAT_EQ( c1.mMember, c2.mMember );                      \
        ASSERT_FLOAT_EQ( init1, c2.mMember );                           \
    }

#define TestDPrimitive( test, name, type, index, init1, init2 )         \
    TEST( P(test), type ## name ## _stream )                            \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        ASSERT_DOUBLE_EQ( c1.mMember, c2.mMember );                     \
        ASSERT_DOUBLE_EQ( init1, c2.mMember );                          \
    }                                                                   \
    TEST( P(test), type ## name ## _file )                              \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        SimpleSerialiseDeserialiseFile( c1, c2 );                       \
        ASSERT_DOUBLE_EQ( c1.mMember, c2.mMember );                     \
        ASSERT_DOUBLE_EQ( init1, c2.mMember );                          \
    }                                                                   \
    TEST( P(test), type ## name ## _backwards )                         \
    {                                                                   \
        Primitive< type > c1( init1 ), c2( init2 );                     \
        std::string file = TEST_FILE( test, type ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );            \
        ASSERT_DOUBLE_EQ( c1.mMember, c2.mMember );                     \
        ASSERT_DOUBLE_EQ( init1, c2.mMember );                          \
    }

#define TestArrayHeader( test, name, type, index )                      \
    TEST( P(test), Type ## name ## _stream )                            \
    {                                                                   \
        ArrayHeader< type > c1, c2;                                     \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
    }                                                                   \
    TEST( P(test), Type ## name ## _file )                              \
    {                                                                   \
        ArrayHeader< type > c1, c2;                                     \
        SimpleSerialiseDeserialiseFile( c1, c2 );                       \
    }                                                                   \
    TEST( P(test), Type ## name ## _backwards )                         \
    {                                                                   \
        ArrayHeader< type > c1, c2;                                     \
        std::string file = TEST_FILE( test, type ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );            \
    }


namespace
{
    template< typename T >
    class Primitive
        : public ISerialisable
    {
    public:

        Primitive( const T &value )
            : mMember( value )
        {
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember, 1 );
        }

        T mMember;
    };

    template< typename T >
    class ArrayHeader
        : public ISerialisable
    {
    public:

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.CreateArray( static_cast< Type::Type >( Internal::Type::GetEnum< T >() ), 0, 5 );
        }
    };

    TestSinglePrimitive( Max, uint8_t, std::numeric_limits<uint8_t>::max(), 1 );
    TestSinglePrimitive( Max, uint16_t, std::numeric_limits<uint16_t>::max(), 1 );
    TestSinglePrimitive( Max, uint32_t, std::numeric_limits<uint32_t>::max(), 1 );
    TestSinglePrimitive( Max, uint64_t, std::numeric_limits<uint64_t>::max(), 1 );

    TestSinglePrimitive( Min, uint8_t, std::numeric_limits<uint8_t>::min(), 1 );
    TestSinglePrimitive( Min, uint16_t, std::numeric_limits<uint16_t>::min(), 1 );
    TestSinglePrimitive( Min, uint32_t, std::numeric_limits<uint32_t>::min(), 1 );
    TestSinglePrimitive( Min, uint64_t, std::numeric_limits<uint64_t>::min(), 1 );

    TestSinglePrimitive( Zebra, uint8_t, GenerateZebraValue<uint8_t>(), 1 );
    TestSinglePrimitive( Zebra, uint16_t, GenerateZebraValue<uint16_t>(), 1 );
    TestSinglePrimitive( Zebra, uint32_t, GenerateZebraValue<uint32_t>(), 1 );
    TestSinglePrimitive( Zebra, uint64_t, GenerateZebraValue<uint64_t>(), 1 );

    TestSinglePrimitive( ZebraInv, uint8_t, GenerateInvZebraValue<uint8_t>(), 1 );
    TestSinglePrimitive( ZebraInv, uint16_t, GenerateInvZebraValue<uint16_t>(), 1 );
    TestSinglePrimitive( ZebraInv, uint32_t, GenerateInvZebraValue<uint32_t>(), 1 );
    TestSinglePrimitive( ZebraInv, uint64_t, GenerateInvZebraValue<uint64_t>(), 1 );

    TestSinglePrimitive( Max, int8_t, std::numeric_limits<int8_t>::max(), 1 );
    TestSinglePrimitive( Max, int16_t, std::numeric_limits<int16_t>::max(), 1 );
    TestSinglePrimitive( Max, int32_t, std::numeric_limits<int32_t>::max(), 1 );
    TestSinglePrimitive( Max, int64_t, std::numeric_limits<int64_t>::max(), 1 );

    TestSinglePrimitive( Min, int8_t, std::numeric_limits<int8_t>::min(), 1 );
    TestSinglePrimitive( Min, int16_t, std::numeric_limits<int16_t>::min(), 1 );
    TestSinglePrimitive( Min, int32_t, std::numeric_limits<int32_t>::min(), 1 );
    TestSinglePrimitive( Min, int64_t, std::numeric_limits<int64_t>::min(), 1 );

    TestSinglePrimitive( Zebra, int8_t, GenerateZebraValue<int8_t>(), 1 );
    TestSinglePrimitive( Zebra, int16_t, GenerateZebraValue<int16_t>(), 1 );
    TestSinglePrimitive( Zebra, int32_t, GenerateZebraValue<int32_t>(), 1 );
    TestSinglePrimitive( Zebra, int64_t, GenerateZebraValue<int64_t>(), 1 );

    TestSinglePrimitive( ZebraInv, int8_t, GenerateInvZebraValue<int8_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int16_t, GenerateInvZebraValue<int16_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int32_t, GenerateInvZebraValue<int32_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int64_t, GenerateInvZebraValue<int64_t>(), 1 );

    TestSingleFPrimitive( Max, float, std::numeric_limits<float>::max(), 1.0f );
    TestSingleDPrimitive( Max, double, std::numeric_limits<double>::max(), 1.0 );

    TestSingleFPrimitive( Min, float, std::numeric_limits<float>::min(), 1.0f );
    TestSingleDPrimitive( Min, double, std::numeric_limits<double>::min(), 1.0 );

    TestSingleFPrimitive( Zebra, float, ( float )GenerateZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitive( Zebra, double, ( double )GenerateZebraValue<int64_t>(), 1.0 );

    TestSingleFPrimitive( ZebraInv, float, ( float )GenerateInvZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitive( ZebraInv, double, ( double )GenerateInvZebraValue<int64_t>(), 1.0 );
}