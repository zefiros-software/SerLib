#include "serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <climits>

#define TestSinglePrimitive( name, type, init1, init2 )  TestPrimitive( SinglePrimitive, name, type, 1, init1, init2, 0x00 )
#define TestSingleFPrimitive( name, type, init1, init2 ) TestFPrimitive( SinglePrimitive, name, type, 1, init1, init2, 0x00 )
#define TestSingleDPrimitive( name, type, init1, init2 ) TestDPrimitive( SinglePrimitive, name, type, 1, init1, init2, 0x00 )

#define TestSinglePrimitivePacked( name, type, init1, init2 )  TestPrimitive( SinglePrimitivePacked, name, type, 1, init1, init2, Message::Packed )
#define TestSingleFPrimitivePacked( name, type, init1, init2 ) TestFPrimitive( SinglePrimitivePacked, name, type, 1, init1, init2, Message::Packed )
#define TestSingleDPrimitivePacked( name, type, init1, init2 ) TestDPrimitive( SinglePrimitivePacked, name, type, 1, init1, init2, Message::Packed )

#define TestPrimitive( test, name, type, index, init1, init2, flag )    \
    TEST( test, type ## name )                                          \
    {                                                                   \
        Primitive< type, flag > c1( init1 ), c2( init2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        EXPECT_EQ( c1.mMember, c2.mMember );                            \
        EXPECT_EQ( init1, c1.mMember );                                 \
    }

#define TestFPrimitive( test, name, type, index, init1, init2, flag )   \
    TEST( test, type ## name )                                          \
    {                                                                   \
        Primitive< type, flag > c1( init1 ), c2( init2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        ASSERT_FLOAT_EQ( c1.mMember, c2.mMember );                      \
        ASSERT_FLOAT_EQ( init1, c1.mMember );                           \
    }

#define TestDPrimitive( test, name, type, index, init1, init2, flag )   \
    TEST( test, type ## name )                                          \
    {                                                                   \
        Primitive< type, flag > c1( init1 ), c2( init2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        ASSERT_DOUBLE_EQ( c1.mMember, c2.mMember );                     \
        ASSERT_DOUBLE_EQ( init1, c1.mMember );                          \
    }

namespace
{
    template< typename T, uint32_t Flag = 0x00 >
    class Primitive
        : public ISerialisable
    {
    public:

        Primitive( const T &value )
            : mMember( value )
        {
        }

        void OnSerialise( Message &message )
        {
            message.Store( mMember, 1, Flag );
        }

        T mMember;
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

    TestSinglePrimitive( Zebra, int8_t, GenerateZebraValue<int8_t>(), 1 );
    TestSinglePrimitive( Zebra, int16_t, GenerateZebraValue<int16_t>(), 1 );
    TestSinglePrimitive( Zebra, int32_t, GenerateZebraValue<int32_t>(), 1 );
    TestSinglePrimitive( Zebra, int64_t, GenerateZebraValue<int64_t>(), 1 );

    TestSinglePrimitive( ZebraInv, int8_t, GenerateInvZebraValue<int8_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int16_t, GenerateInvZebraValue<int16_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int32_t, GenerateInvZebraValue<int32_t>(), 1 );
    TestSinglePrimitive( ZebraInv, int64_t, GenerateInvZebraValue<int64_t>(), 1 );

    TestSinglePrimitive( Min, int8_t, std::numeric_limits<int8_t>::min(), 1 );
    TestSinglePrimitive( Min, int16_t, std::numeric_limits<int16_t>::min(), 1 );
    TestSinglePrimitive( Min, int32_t, std::numeric_limits<int32_t>::min(), 1 );
    TestSinglePrimitive( Min, int64_t, std::numeric_limits<int64_t>::min(), 1 );

    TestSingleFPrimitive( Max, float, std::numeric_limits<float>::max(), 1.0f );
    TestSingleDPrimitive( Max, double, std::numeric_limits<double>::max(), 1.0 );

    TestSingleFPrimitive( Min, float, std::numeric_limits<float>::min(), 1.0f );
    TestSingleDPrimitive( Min, double, std::numeric_limits<double>::min(), 1.0 );

    TestSingleFPrimitive( Zebra, float, ( float )GenerateZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitive( Zebra, double, ( double )GenerateZebraValue<int64_t>(), 1.0 );

    TestSingleFPrimitive( ZebraInv, float, ( float )GenerateInvZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitive( ZebraInv, double, ( double )GenerateInvZebraValue<int64_t>(), 1.0 );


    TestSinglePrimitivePacked( Max, uint8_t, std::numeric_limits<uint8_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, uint16_t, std::numeric_limits<uint16_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, uint32_t, std::numeric_limits<uint32_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, uint64_t, std::numeric_limits<uint64_t>::max(), 1 );

    TestSinglePrimitivePacked( Min, uint8_t, std::numeric_limits<uint8_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, uint16_t, std::numeric_limits<uint16_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, uint32_t, std::numeric_limits<uint32_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, uint64_t, std::numeric_limits<uint64_t>::min(), 1 );

    TestSinglePrimitivePacked( Zebra, uint8_t, GenerateZebraValue<uint8_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, uint16_t, GenerateZebraValue<uint16_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, uint32_t, GenerateZebraValue<uint32_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, uint64_t, GenerateZebraValue<uint64_t>(), 1 );

    TestSinglePrimitivePacked( ZebraInv, uint8_t, GenerateInvZebraValue<uint8_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, uint16_t, GenerateInvZebraValue<uint16_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, uint32_t, GenerateInvZebraValue<uint32_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, uint64_t, GenerateInvZebraValue<uint64_t>(), 1 );

    TestSinglePrimitivePacked( Max, int8_t, std::numeric_limits<int8_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, int16_t, std::numeric_limits<int16_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, int32_t, std::numeric_limits<int32_t>::max(), 1 );
    TestSinglePrimitivePacked( Max, int64_t, std::numeric_limits<int64_t>::max(), 1 );

    TestSinglePrimitivePacked( Zebra, int8_t, GenerateZebraValue<int8_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, int16_t, GenerateZebraValue<int16_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, int32_t, GenerateZebraValue<int32_t>(), 1 );
    TestSinglePrimitivePacked( Zebra, int64_t, GenerateZebraValue<int64_t>(), 1 );

    TestSinglePrimitivePacked( ZebraInv, int8_t, GenerateInvZebraValue<int8_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, int16_t, GenerateInvZebraValue<int16_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, int32_t, GenerateInvZebraValue<int32_t>(), 1 );
    TestSinglePrimitivePacked( ZebraInv, int64_t, GenerateInvZebraValue<int64_t>(), 1 );

    TestSinglePrimitivePacked( Min, int8_t, std::numeric_limits<int8_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, int16_t, std::numeric_limits<int16_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, int32_t, std::numeric_limits<int32_t>::min(), 1 );
    TestSinglePrimitivePacked( Min, int64_t, std::numeric_limits<int64_t>::min(), 1 );

    TestSingleFPrimitivePacked( Max, float, std::numeric_limits<float>::max(), 1.0f );
    TestSingleDPrimitivePacked( Max, double, std::numeric_limits<double>::max(), 1.0 );

    TestSingleFPrimitivePacked( Min, float, std::numeric_limits<float>::min(), 1.0f );
    TestSingleDPrimitivePacked( Min, double, std::numeric_limits<double>::min(), 1.0 );

    TestSingleFPrimitivePacked( Zebra, float, ( float )GenerateZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitivePacked( Zebra, double, ( double )GenerateZebraValue<int64_t>(), 1.0 );

    TestSingleFPrimitivePacked( ZebraInv, float, ( float )GenerateInvZebraValue<int32_t>(), 1.0f );
    TestSingleDPrimitivePacked( ZebraInv, double, ( double )GenerateInvZebraValue<int64_t>(), 1.0 );
}