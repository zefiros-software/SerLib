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

TEST( UtilTest, FloatUInt32Max )
{
    const float fl = std::numeric_limits<float>::max();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( UtilTest, FloatUInt32Min )
{
    const float fl = std::numeric_limits<float>::min();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( UtilTest, FloatUInt32Zebra )
{
    const float fl = ( float )GenerateZebraValue< uint32_t >();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}

TEST( UtilTest, FloatUInt32ZebraInv )
{
    const float fl = ( float )GenerateInvZebraValue< uint32_t >();
    EXPECT_FLOAT_EQ( fl, ConvertDeconvertFloat( fl ) );
}


TEST( UtilTest, DoubleInt64Max )
{
    const double dl = std::numeric_limits<double>::max();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( UtilTest, DoubleUInt64Min )
{
    const double dl = std::numeric_limits<double>::min();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( UtilTest, DoubleUInt64Zebra )
{
    const double dl = ( double )GenerateZebraValue< uint64_t >();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}

TEST( UtilTest, DoubleUInt64ZebraInv )
{
    const double dl = ( double )GenerateInvZebraValue< uint64_t >();
    ASSERT_DOUBLE_EQ( dl, ConvertDeconvertDouble( dl ) );
}