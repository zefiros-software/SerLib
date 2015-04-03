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

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define TestSerialClass( test, name, type, seed1, seed2, its )      \
    TEST( P( test ), type ## name ## _stream )                      \
    {                                                               \
        TestClass1< type, its > c1( seed1 ), c2( seed2 );           \
        SimpleSerialiseDeserialiseStream( c1, c2 );                 \
        c1.TestEqual( c2 );                                         \
    }                                                               \
    TEST( P( test ), type ## name ## _file )                        \
    {                                                               \
        TestClass1< type, its > c1( seed1 ), c2( seed2 );           \
        SimpleSerialiseDeserialiseFile( c1, c2 );                   \
        c1.TestEqual( c2 );                                         \
    }                                                               \
    TEST( P( test ), type ## name ## _backwards )                   \
    {                                                               \
        TestClass1< type, its > c1( seed1 ), c2( seed2 );           \
        std::string file = TEST_FILE( test, type ## name);          \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );        \
        c1.TestEqual( c2 );                                         \
    }

#define TestFSerialClass( test, name, seed1, seed2, its )       \
    TEST( P( test ), float ## name ## _stream )                 \
    {                                                           \
        TestClass1F< its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );             \
        c1.TestEqual( c2 );                                     \
    }                                                           \
    TEST( P( test ), float ## name ## _file )                   \
    {                                                           \
        TestClass1F< its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseFile( c1, c2 );               \
        c1.TestEqual( c2 );                                     \
    }                                                           \
    TEST( P( test ), float ## name ## _backwards )              \
    {                                                           \
        TestClass1F< its > c1( seed1 ), c2( seed2 );            \
        std::string file = TEST_FILE( test, float ## name);     \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );    \
        c1.TestEqual( c2 );                                     \
    }

#define TestDSerialClass( test, name, seed1, seed2, its )   \
    TEST( P( test ), double ## name ## _stream )            \
    {                                                       \
        TestClass1D< its > c1( seed1 ), c2( seed2 );        \
        SimpleSerialiseDeserialiseStream( c1, c2 );         \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), double ## name ## _file )              \
    {                                                       \
        TestClass1D< its > c1( seed1 ), c2( seed2 );        \
        SimpleSerialiseDeserialiseFile( c1, c2 );           \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), double ## name ## _backwards )         \
    {                                                       \
        TestClass1D< its > c1( seed1 ), c2( seed2 );        \
        std::string file = TEST_FILE( test, double ## name);\
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );\
        c1.TestEqual( c2 );                                 \
    }

#define TestIFDMixedSerialClass( test, name, typeI, seed1, seed2, its ) \
    TEST( P( test ), typeI ## name ## _stream )                         \
    {                                                                   \
        TestClass2F1< typeI, its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        c1.TestEqual( c2 );                                             \
    }                                                                   \
    TEST( P( test ), typeI ## name ## _file )                           \
    {                                                                   \
        TestClass2F1< typeI, its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );                     \
        c1.TestEqual( c2 );                                             \
    }                                                                   \
    TEST( P( test ), typeI ## name ## _backwards )                      \
    {                                                                   \
        TestClass2F1< typeI, its > c1( seed1 ), c2( seed2 );            \
        std::string file = TEST_FILE( test, typeI ## name);             \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );            \
        c1.TestEqual( c2 );                                             \
    }

#define TestFIDMixedSerialClass( test, name, typeI, seed1, seed2, its )  \
    TEST( P( test ), typeI ## name ## _stream )                          \
    {                                                                    \
        TestClass2F2< typeI, its > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseStream( c1, c2 );                      \
        c1.TestEqual( c2 );                                              \
    }                                                                    \
    TEST( P( test ), typeI ## name ## _file )                            \
    {                                                                    \
        TestClass2F2< typeI, its > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseFile( c1, c2 );                        \
        c1.TestEqual( c2 );                                              \
    }                                                                    \
    TEST( P( test ), typeI ## name ## _backwards )                       \
    {                                                                    \
        TestClass2F2< typeI, its > c1( seed1 ), c2( seed2 );             \
        std::string file = TEST_FILE( test, typeI ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );             \
        c1.TestEqual( c2 );                                              \
    }

#define TestIDIMixedSerialClass( test, name, typeI1, typeI2, seed1, seed2, its )    \
    TEST( P( test ), typeI1 ## typeI2 ## name ## _stream )                          \
    {                                                                               \
        TestClass2F3< typeI1, typeI2, its > c1( seed1 ), c2( seed2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                 \
        c1.TestEqual( c2 )                                                          \
    }                                                                               \
    TEST( P( test ), typeI1 ## typeI2 ## name ## _file )                            \
    {                                                                               \
        TestClass2F3< typeI1, typeI2, its > c1( seed1 ), c2( seed2 );               \
        SimpleSerialiseDeserialiseFile( c1, c2 );                                   \
        c1.TestEqual( c2 )                                                          \
    }                                                                               \
    TEST( P( test ), typeI1 ## typeI2 ## name ## _backwards )                       \
    {                                                                               \
        TestClass2F3< typeI1, typeI2, its > c1( seed1 ), c2( seed2 );               \
        std::string file = TEST_FILE( test, typeI1 ## typeI2 ## name);              \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );                        \
        c1.TestEqual( c2 )                                                          \
    }

namespace
{
    template< typename T, uint32_t its = 32 >
    class TestClass1
        : public ISerialisable
    {
    public:

        TestClass1( uint32_t seed = 233232 )
        {
            g_seed = seed;

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMember[i] = GetRandom< T >();
            }
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            for ( uint8_t i = 0; i < its; ++i )
            {
                message.Store( mMember[i], i );
            }
        }

        virtual void TestEqual( TestClass1 &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_EQ( mMember[i], c2.mMember[i] );
            }
        }

        T mMember[its];
    };

    template< uint32_t its = 32 >
    class TestClass1F
        : public TestClass1< float, its >
    {
    public:

        TestClass1F( uint32_t seed = 233232 )
            : TestClass1< float, its >( seed )
        {
        }

        virtual void TestEqual( TestClass1< float, its > &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_FLOAT_EQ( this->mMember[i], c2.mMember[i] );
            }
        }
    };

    template< uint32_t its = 32 >
    class TestClass1D
        : public TestClass1< double, its >
    {
    public:

        TestClass1D( uint32_t seed = 233232 )
            : TestClass1< double, its >( seed )
        {
        }

        virtual void TestEqual( TestClass1< double, its > &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_DOUBLE_EQ( this->mMember[i], c2.mMember[i] );
            }
        }
    };


    template< typename T, typename S, typename R, uint8_t its = 10 >
    class TestClass2
        : public ISerialisable
    {
    public:

        TestClass2( uint32_t seed = 233232 )
        {
            g_seed = seed;

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberT[i] = GetRandom< T >();
                mMemberS[i] = GetRandom< S >();
                mMemberR[i] = GetRandom< R >();
            }
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            for ( uint8_t i = 0; i < its; ++i )
            {
                uint8_t j = 3 * i;
                message.Store( mMemberT[i], j );
                message.Store( mMemberS[i], j + 1 );
                message.Store( mMemberR[i], j + 2 );
            }
        }

        virtual void TestEqual( TestClass2 &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_EQ( mMemberR[i], c2.mMemberR[i] );
            }
        }

        T mMemberT[its];
        S mMemberS[its];
        R mMemberR[its];
    };

    template< typename T, uint32_t its = 100 >
    class TestClass2F1
        : public TestClass2< T, float, double, its >
    {
    public:

        TestClass2F1( uint32_t seed = 233232 )
            : TestClass2< T, float, double, its >( seed )
        {
        }

        virtual void TestEqual( TestClass2< T, float, double, its > &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_EQ( this->mMemberT[i], c2.mMemberT[i] );
                EXPECT_FLOAT_EQ( this->mMemberS[i], c2.mMemberS[i] );
                EXPECT_DOUBLE_EQ( this->mMemberR[i], c2.mMemberR[i] );
            }
        }
    };

    template< typename T, uint32_t its = 100 >
    class TestClass2F2
        : public TestClass2< float, T, double, its >
    {
    public:

        TestClass2F2( uint32_t seed = 233232 )
            : TestClass2< float, T, double, its >( seed )
        {
        }

        virtual void TestEqual( TestClass2< float, T, double, its > &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_FLOAT_EQ( this->mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( this->mMemberS[i], c2.mMemberS[i] );
                EXPECT_DOUBLE_EQ( this->mMemberR[i], c2.mMemberR[i] );
            }
        }
    };

    template< typename T, typename R, uint32_t its = 100 >
    class TestClass2F3
        : public TestClass2< T, double, R, its >
    {
    public:

        TestClass2F3( uint32_t seed = 233232 )
            : TestClass2< T, double, R, its >( seed )
        {
        }

        virtual void TestEqual( TestClass2< T, double, R, its > &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                EXPECT_DOUBLE_EQ( this->mMemberS[i], c2.mMemberS[i] );
            }
        }
    };



    TestSerialClass( MultiSerialisation, randomVals, uint8_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, uint16_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, uint32_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, uint64_t, 343422, 21331, 28 );

    TestSerialClass( MultiSerialisation, randomVals, int8_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, int16_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, int32_t, 343422, 21331, 28 );
    TestSerialClass( MultiSerialisation, randomVals, int64_t, 343422, 21331, 28 );

    TestFSerialClass( MultiSerialisation, randomVals, 343422, 21331, 28 );
    TestDSerialClass( MultiSerialisation, randomVals, 343422, 21331, 28 );


    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint8_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint16_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint32_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint64_t, 343422, 21331, 9 );

    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int8_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int16_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int32_t, 343422, 21331, 9 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int64_t, 343422, 21331, 9 );

    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint8_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint16_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint32_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint64_t, 343422, 21331, 9 );

    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int8_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int16_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int32_t, 343422, 21331, 9 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int64_t, 343422, 21331, 9 );

}