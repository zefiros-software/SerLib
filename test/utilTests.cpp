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
#include "serialisation/util.h"

#include "gtest/gtest.h"

#include <climits>

float ConvertDeconvertFloat( const float fl )
{
    return Util::UInt32ToFloat( Util::FloatToUInt32( fl ) );
}

double ConvertDeconvertDouble( const double dl )
{
    return Util::UInt64ToDouble( Util::DoubleToUInt64( dl ) );
}

uint32_t HeaderIndexConvert( uint8_t index, Internal::Type::Type t )
{
    return Util::GetHeaderIndex( Util::CreateHeader( index, t ) );
}

Internal::Type::Type HeaderTypeConvert( uint8_t index, Internal::Type::Type t )
{
    return Util::GetHeaderType( Util::CreateHeader( index, t ) );
}


TEST( P( UtilTest ), FloatUInt32Max )
{
    const float fl = std::numeric_limits<float>::max();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( P( UtilTest ), FloatUInt32Min )
{
    const float fl = std::numeric_limits<float>::min();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( P( UtilTest ), FloatUInt32Zebra )
{
    const float fl = ( float )GenerateZebraValue< uint32_t >();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( P( UtilTest ), FloatUInt32ZebraInv )
{
    const float fl = ( float )GenerateInvZebraValue< uint32_t >();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( P( UtilTest ), DoubleInt64Max )
{
    const double dl = std::numeric_limits<double>::max();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( P( UtilTest ), DoubleUInt64Min )
{
    const double dl = std::numeric_limits<double>::min();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( P( UtilTest ), DoubleUInt64Zebra )
{
    const double dl = ( double )GenerateZebraValue< uint64_t >();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( P( UtilTest ), DoubleUInt64ZebraInv )
{
    const double dl = ( double )GenerateInvZebraValue< uint64_t >();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( P( UtilTest ), ZebraHeaderIndexMessage )
{
    const uint8_t ul = GenerateZebraValue< uint8_t >() & 0x1F;
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::String ) );
}

TEST( P( UtilTest ), MinHeaderIndexMessage )
{
    const uint8_t ul = std::numeric_limits<uint8_t>::min();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::String ) );
}

TEST( P( UtilTest ), MaxHeaderIndexMessage )
{
    const uint8_t ul = 0x1F;
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::String ) );
}

TEST( P( UtilTest ), ZebraHeaderIndexRepeated )
{
    const uint8_t ul = GenerateZebraValue< uint8_t >() & 0x1F;
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::Array ) );
}

TEST( P( UtilTest ), MinHeaderIndexRepeated )
{
    const uint8_t ul = std::numeric_limits<uint8_t>::min();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::Array ) );
}

TEST( P( UtilTest ), MaxHeaderIndexRepeated )
{
    const uint8_t ul = 0x1F;
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Internal::Type::Array ) );
}

TEST( P( UtilTest ), MessageHeaderTypeZebraIndex )
{
    const uint8_t ul = GenerateZebraValue<uint8_t>() & 0x1F;
    ASSERT_EQ( Internal::Type::Object, HeaderTypeConvert( ul, Internal::Type::Object ) );
}

TEST( P( UtilTest ), MessageHeaderTypeMaxIndex )
{
    const uint8_t ul = 0x1F;
    ASSERT_EQ( Internal::Type::Object, HeaderTypeConvert( ul, Internal::Type::Object ) );
}

TEST( P( UtilTest ), MessageHeaderTypeMinIndex )
{
    const uint8_t ul = std::numeric_limits<uint8_t>::min();
    ASSERT_EQ( Internal::Type::Object, HeaderTypeConvert( ul, Internal::Type::Object ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeZebraIndex )
{
    const uint8_t ul = GenerateZebraValue<uint8_t>() & 0x1F;
    ASSERT_EQ( Internal::Type::Array, HeaderTypeConvert( ul, Internal::Type::Array ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeMaxIndex )
{
    const uint8_t ul = 0x1F;
    ASSERT_EQ( Internal::Type::Array, HeaderTypeConvert( ul, Internal::Type::Array ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeMinIndex )
{
    const uint8_t ul = std::numeric_limits<uint8_t>::min();
    ASSERT_EQ( Internal::Type::Array, HeaderTypeConvert( ul, Internal::Type::Array ) );
}