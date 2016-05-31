/**
 * Copyright (c) 2016 Mick van Duijn, Koen Visscher and Paul Visscher
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

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define TestReorderedPrimitive( test, name, seed1, seed2, type1, type2 )  \
    TEST( P(test), type1 ## type2 ## name ## _stream )                    \
    {                                                                     \
        NormalPrimitive< type1, type2 > c1( seed1 );                      \
        ReorderedPrimitive< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseStream( c1, c2 );                       \
        c1.TestEqual( c2 );                                               \
    }                                                                     \
    TEST( P(test), type1 ## type2 ## name ## _file )                      \
    {                                                                     \
        NormalPrimitive< type1, type2 > c1( seed1 );                      \
        ReorderedPrimitive< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseFile( c1, c2 );                         \
        c1.TestEqual( c2 );                                               \
    }                                                                     \
    TEST( P(test), type1 ## type2 ## name ## _backwards )                 \
    {                                                                     \
        NormalPrimitive< type1, type2 > c1( seed1 );                      \
        ReorderedPrimitive< type1, type2 > c2( seed2 );                   \
        std::string file = TEST_FILE( test, type1 ## type2 ## name);      \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );              \
        c1.TestEqual( c2 );                                               \
    }

#define TestReorderedObjectClass( test, name, seed1, seed2, type1, type2 )  \
    TEST( P(test), type1 ## type2 ## name ## _stream )                      \
    {                                                                       \
        NormalObjectClass< type1, type2 > c1( seed1 );                      \
        ReorderedObjectClass< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseStream( c1, c2 );                         \
        c1.TestEqual( c2 );                                                 \
    }                                                                       \
    TEST( P(test), type1 ## type2 ## name ## _file )                        \
    {                                                                       \
        NormalObjectClass< type1, type2 > c1( seed1 );                      \
        ReorderedObjectClass< type1, type2 > c2( seed2 );                   \
        SimpleSerialiseDeserialiseFile( c1, c2 );                           \
        c1.TestEqual( c2 );                                                 \
    }                                                                       \
    TEST( P(test), type1 ## type2 ## name ## _backwards )                   \
    {                                                                       \
        NormalObjectClass< type1, type2 > c1( seed1 );                      \
        ReorderedObjectClass< type1, type2 > c2( seed2 );                   \
        std::string file = TEST_FILE( test, type1 ## type2 ## name);        \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );                \
        c1.TestEqual( c2 );                                                 \
    }

#define TestReorderedRepeatedObjectClass( test, name, seed1, seed2, type1, type2, its ) \
    TEST( P( test ), type1 ## type2 ## name ## _stream )                                \
    {                                                                                   \
        NormalRepeatedObjectClass< type1, type2, its > c1( seed1 );                     \
        ReorderedRepeatedObjectClass< type1, type2, its > c2( seed2 );                  \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                     \
        c1.TestEqual( c2 );                                                             \
    }                                                                                   \
    TEST( P( test ), type1 ## type2 ## name ## _file )                                  \
    {                                                                                   \
        NormalRepeatedObjectClass< type1, type2, its > c1( seed1 );                     \
        ReorderedRepeatedObjectClass< type1, type2, its > c2( seed2 );                  \
        SimpleSerialiseDeserialiseFile( c1, c2 );                                       \
        c1.TestEqual( c2 );                                                             \
    }                                                                                   \
    TEST( P( test ), type1 ## type2 ## name ## _backwards )                             \
    {                                                                                   \
        NormalRepeatedObjectClass< type1, type2, its > c1( seed1 );                     \
        ReorderedRepeatedObjectClass< type1, type2, its > c2( seed2 );                  \
        std::string file = TEST_FILE( test, type ## name);                              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );                            \
        c1.TestEqual( c2 );                                                             \
    }

namespace
{
    template< typename T1, typename T2 >
    class NormalPrimitive
        : public ISerialisable
    {
    public:

        NormalPrimitive( uint32_t seed = 233232 )
        {
            g_seed = seed;

            mMemberT1 = GetRandom< T1 >();
            mMemberT2 = GetRandom< T2 >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMemberT1, 0 );
            message.Store( mMemberT2, 1 );
        }

        void TestEqual( NormalPrimitive< T1, T2 > &c2 )
        {
            ASSERT_EQ( mMemberT1, c2.mMemberT1 );
            ASSERT_EQ( mMemberT2, c2.mMemberT2 );
        }

    protected:

        T1 mMemberT1;
        T2 mMemberT2;
    };

    template< typename T1, typename T2 >
    class ReorderedPrimitive
        : public NormalPrimitive< T1, T2 >
    {
    public:

        ReorderedPrimitive( uint32_t seed = 233232 )
            : NormalPrimitive< T1, T2 >( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( NormalPrimitive< T1, T2 >::mMemberT2, 1 );
            message.Store( NormalPrimitive< T1, T2 >::mMemberT1, 0 );
        }
    };

    template< typename T1, typename T2 >
    class NormalObjectClass
        : public ISerialisable
    {
    public:

        NormalObjectClass( uint32_t seed = 233232 )
            : mMember1( seed ), mMember2( seed )
        {

        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.Store( mMember2, 1 );
        }

        void TestEqual( NormalObjectClass< T1, T2 > &c2 )
        {
            mMember1.TestEqual( c2.mMember1 );
            mMember2.TestEqual( c2.mMember2 );
        }

    protected:

        NormalPrimitive< T1, T2 > mMember1;
        ReorderedPrimitive< T2, T1 > mMember2;
    };

    template< typename T1, typename T2 >
    class ReorderedObjectClass
        : public NormalObjectClass< T1, T2 >
    {
    public:

        ReorderedObjectClass( uint32_t seed = 233232 )
            : NormalObjectClass< T1, T2 >( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( NormalObjectClass< T1, T2 >::mMember2, 1 );
            message.Store( NormalObjectClass< T1, T2 >::mMember1, 0 );
        }
    };

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint8_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint16_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint32_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, uint64_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int8_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int16_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int32_t, int64_t );

    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, uint64_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int8_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int16_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int32_t );
    TestReorderedPrimitive( ReorderedPrimitive, randomVals, 343422, 21331, int64_t, int64_t );



    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint8_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint16_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint32_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, uint64_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int8_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int16_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int32_t, int64_t );

    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, uint64_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int8_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int16_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int32_t );
    TestReorderedObjectClass( ReorderedObjectClass, randomVals, 343422, 21331, int64_t, int64_t );
}
