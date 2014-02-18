#include "helper.h"

#include "util.h"

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

uint32_t HeaderIndexConvert( uint32_t index, Type::Type t )
{
    return Util::GetHeaderIndex( Util::CreateHeader( index, t ) );
}

Type::Type HeaderTypeConvert( uint32_t index, Type::Type t )
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
    const uint32_t ul = GenerateZebraValue< uint32_t >();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::String ) );
}

TEST( P( UtilTest ), MinHeaderIndexMessage )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::min();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::String ) );
}

TEST( P( UtilTest ), MaxHeaderIndexMessage )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::max();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::String ) );
}

TEST( P( UtilTest ), ZebraHeaderIndexRepeated )
{
    const uint32_t ul = GenerateZebraValue< uint32_t >();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::Repeated ) );
}

TEST( P( UtilTest ), MinHeaderIndexRepeated )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::min();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::Repeated ) );
}

TEST( P( UtilTest ), MaxHeaderIndexRepeated )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::max();
    ASSERT_EQ( ul, HeaderIndexConvert( ul, Type::Repeated ) );
}

TEST( P( UtilTest ), MessageHeaderTypeZebraIndex )
{
    const uint32_t ul = GenerateZebraValue<uint32_t>();
    ASSERT_EQ( Type::Message, HeaderTypeConvert( ul, Type::Message ) );
}

TEST( P( UtilTest ), MessageHeaderTypeMaxIndex )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::max();
    ASSERT_EQ( Type::Message, HeaderTypeConvert( ul, Type::Message ) );
}

TEST( P( UtilTest ), MessageHeaderTypeMinIndex )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::min();
    ASSERT_EQ( Type::Message, HeaderTypeConvert( ul, Type::Message ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeZebraIndex )
{
    const uint32_t ul = GenerateZebraValue<uint32_t>();
    ASSERT_EQ( Type::Repeated, HeaderTypeConvert( ul, Type::Repeated ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeMaxIndex )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::max();
    ASSERT_EQ( Type::Repeated, HeaderTypeConvert( ul, Type::Repeated ) );
}

TEST( P( UtilTest ), RepeatedHeaderTypeMinIndex )
{
    const uint32_t ul = std::numeric_limits<uint32_t>::min();
    ASSERT_EQ( Type::Repeated, HeaderTypeConvert( ul, Type::Repeated ) );
}