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
    TEST( P( test ), type ## name )                                       \
    {                                                                     \
        TestClass1< type, its > c1( seed1 ), c2( seed2 );           \
        SimpleSerialiseDeserialiseStream( c1, c2 );                       \
        c1.TestEqual( c2 );                                               \
    }

#define TestFSerialClass( test, name, seed1, seed2, its )     \
    TEST( P( test ), float ## name )                                \
    {                                                               \
        TestClass1F< its > c1( seed1 ), c2( seed2 );          \
        SimpleSerialiseDeserialiseStream( c1, c2 );                 \
        c1.TestEqual( c2 );                                         \
    }

#define TestDSerialClass( test, name, seed1, seed2, its ) \
    TEST( P( test ), double ## name )                           \
    {                                                           \
        TestClass1D< its > c1( seed1 ), c2( seed2 );      \
        SimpleSerialiseDeserialiseStream( c1, c2 );             \
        c1.TestEqual( c2 );                                     \
    }

#define TestIFDMixedSerialClass( test, name, typeI, seed1, seed2, its )   \
    TEST( P( test ), typeI ## name )                                            \
    {                                                                           \
        TestClass2F1< typeI, its > c1( seed1 ), c2( seed2 );              \
        SimpleSerialiseDeserialiseStream( c1, c2 );                             \
        c1.TestEqual( c2 );                                                     \
    }

#define TestFIDMixedSerialClass( test, name, typeI, seed1, seed2, its )  \
    TEST( P( test ), typeI ## name )                                           \
    {                                                                          \
        TestClass2F2< typeI, its > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseStream( c1, c2 );                            \
        c1.TestEqual( c2 );                                                    \
    }

#define TestIDIMixedSerialClass( test, name, typeI1, typeI2, seed1, seed2, its )  \
    TEST( P( test ), typeI1 ## typeI2 ## name )                                         \
    {                                                                                   \
        TestClass2F3< typeI1, typeI2, its > c1( seed1 ), c2( seed2 );             \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                     \
        c1.TestEqual( c2 )                                                              \
    }

#define TestEasyRepeatedClass( test, name, seed1, seed2, its ) \
    TEST( P( test ), name )                                          \
    {                                                                \
        TestClass3< its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );                  \
        c1.TestEqual( c2 );                                          \
    }

#define TestRepeatedPrimitiveMessageClass( test, name, seed1, seed2, its, type )      \
    TEST( P( test ), type ## name )                                                         \
    {                                                                                       \
        TestClass4< Primitive< type >, its, type > c1( seed1 ), c2( seed2 );    \
        SimpleSerialiseDeserialiseStream( c1, c2 );                                         \
        c1.TestEqual( c2 );                                                                 \
    }

namespace TestClasses
{
    template< typename T, uint32_t its = 32 >
    class TestClass1
        : public ISerialisable
    {
    public:

        TestClass1( uint32_t seed = 233232 )
        {
            srand( seed );

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
            srand( seed );

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

    template< uint32_t its = 100 >
    class TestClass3
        : public ISerialisable
    {
    public:

        TestClass3( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberT.push_back( GetRandom<uint8_t>() );
                mMemberS.push_back( GetRandom<uint16_t >() );
                mMemberR.push_back( GetRandom<uint32_t>() );
                mMemberG.push_back( GetRandom<uint64_t >() );

                mMemberTs.push_back( GetRandom<int8_t>() );
                mMemberSs.push_back( GetRandom<int16_t >() );
                mMemberRs.push_back( GetRandom<int32_t>() );
                mMemberGs.push_back( GetRandom<int64_t >() );

                mMemberF.push_back( GetRandom< float > () );
                mMemberD.push_back( GetRandom< double > () );

            }

            name1 = "TestClassTres";
            name2 = "CouldBeUsedToStoreNames";
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.CreateArray( Type::UInt8, its, 1 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberT[i] );
            }

            message.CreateArray( Type::SInt8, its, 0 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberTs[i] );
            }

            message.CreateArray( Type::UInt16, its, 2 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberS[i] );
            }

            message.CreateArray( Type::SInt16, its, 3 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberSs[i] );
            }

            message.CreateArray( Type::UInt32, its, 4 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberR[i] );
            }

            message.StoreContainer( mMemberR, 4 );

            message.CreateArray( Type::SInt32, its, 5 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberRs[i] );
            }

            message.CreateArray( Type::UInt64, its, 6 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberG[i] );
            }

            message.CreateArray( Type::SInt64, its, 7 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberGs[i] );
            }

            message.Store( name1, 8 );
            message.Store( name2, 9 );

            message.CreateArray( Type::Float, its, 10 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberF[i] );
            }

            message.CreateArray( Type::Double, its, 11 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberD[i] );
            }

        }

        void TestEqual( TestClass3 &c2 )
        {
            for ( uint32_t i = 0; i < its; i++ )
            {
                EXPECT_EQ( mMemberT[i], c2.mMemberT[i] );
                EXPECT_EQ( mMemberTs[i], c2.mMemberTs[i] );
                EXPECT_EQ( mMemberR[i], c2.mMemberR[i] );
                EXPECT_EQ( mMemberRs[i], c2.mMemberRs[i] );
                EXPECT_EQ( mMemberG[i], c2.mMemberG[i] );
                EXPECT_EQ( mMemberGs[i], c2.mMemberGs[i] );
                EXPECT_EQ( mMemberS[i], c2.mMemberS[i] );
                EXPECT_EQ( mMemberSs[i], c2.mMemberSs[i] );
                EXPECT_FLOAT_EQ( mMemberF[i], c2.mMemberF[i] );
                EXPECT_DOUBLE_EQ( mMemberD[i], c2.mMemberD[i] );
            }

            EXPECT_EQ( name1.compare( c2.name1 ), 0 );
            EXPECT_EQ( name2.compare( c2.name2 ), 0 );
        }

        std::vector< uint8_t > mMemberT;
        std::vector< uint16_t > mMemberS;
        std::vector< uint32_t > mMemberR;
        std::vector< uint64_t > mMemberG;

        std::vector< int8_t > mMemberTs;
        std::vector< int16_t > mMemberSs;
        std::vector< int32_t > mMemberRs;
        std::vector< int64_t > mMemberGs;

        std::string name1;
        std::string name2;

        std::vector< float > mMemberF;
        std::vector< double > mMemberD;
    };

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

        void TestEqual( Primitive &c2 )
        {
            EXPECT_EQ( mMember, c2.mMember );
        }

        T mMember;
    };

    template< typename TestClass, uint32_t its = 100, typename T = uint32_t >
    class TestClass4
        : public ISerialisable
    {
    public:

        TestClass4( uint32_t seed = 233232 )
        {
            srand( seed );

            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberTestClasses.push_back( TestClass( GetRandom< T >() ) );
            }
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.CreateArray( Type::Object, its, 0 );

            for ( uint32_t i = 0; i < its; ++i )
            {
                message.StoreArrayItem( mMemberTestClasses[ i ] );
            }
        }

        void TestEqual( TestClass4 &c2 )
        {
            for ( uint32_t i = 0; i < its; ++i )
            {
                mMemberTestClasses[ i ].TestEqual( c2.mMemberTestClasses[ i ] );
            }
        }

        std::vector< TestClass > mMemberTestClasses;
    };



    TestSerialClass( MultiSerialisation, randomVals, uint8_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, uint16_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, uint32_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, uint64_t, 343422, 21331, 32 );

    TestSerialClass( MultiSerialisation, randomVals, int8_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, int16_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, int32_t, 343422, 21331, 32 );
    TestSerialClass( MultiSerialisation, randomVals, int64_t, 343422, 21331, 32 );

    TestFSerialClass( MultiSerialisation, randomVals, 343422, 21331, 32 );
    TestDSerialClass( MultiSerialisation, randomVals, 343422, 21331, 32 );


    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint8_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint16_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint32_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, uint64_t, 343422, 21331, 10 );

    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int8_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int16_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int32_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( MixedIFDSerialisation, randomVals, int64_t, 343422, 21331, 10 );

    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint8_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint16_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint32_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, uint64_t, 343422, 21331, 10 );

    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int8_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int16_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int32_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( MixedFIDSerialisation, randomVals, int64_t, 343422, 21331, 10 );

    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, uint8_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, uint16_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, uint32_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, uint64_t, 343422, 21331, 10 );

    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, int8_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, int16_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, int32_t, 343422, 21331, 10 );
    TestIFDMixedSerialClass( PackedMixedIFDSerialisation, randomVals, int64_t, 343422, 21331, 10 );

    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, uint8_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, uint16_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, uint32_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, uint64_t, 343422, 21331, 10 );

    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, int8_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, int16_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, int32_t, 343422, 21331, 10 );
    TestFIDMixedSerialClass( PackedMixedFIDSerialisation, randomVals, int64_t, 343422, 21331, 10 );

    TestEasyRepeatedClass( EasyRepeated, randomVals, 343422, 21331, 5 );
    TestEasyRepeatedClass( EasyRepeatedPacked, randomVals, 343422, 21331, 100 );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint64_t );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int64_t );

}