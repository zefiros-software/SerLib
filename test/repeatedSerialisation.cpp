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

#define TestEasyRepeatedClass( test, name, seed1, seed2, its ) \
    TEST( P( test ), name )                                          \
    {                                                                \
        TestClass3< its > c1( seed1 ), c2( seed2 );            \
        SimpleSerialiseDeserialiseStream( c1, c2 );                  \
        c1.TestEqual( c2 );                                          \
    }

#define TestEasyRepeatedClassNoHelper( test, name, seed1, seed2, its ) \
    TEST( P( test ), name )                                          \
    {                                                                \
        TestClass3NoHelper< its > c1( seed1 ), c2( seed2 );            \
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
            message.StoreContainer( mMemberT, 1 );
            message.StoreContainer( mMemberTs, 0 );
            message.StoreContainer( mMemberS, 2 );
            message.StoreContainer( mMemberSs, 3 );
            message.StoreContainer( mMemberR, 4 );
            message.StoreContainer( mMemberRs, 5 );
            message.StoreContainer( mMemberG, 6 );
            message.StoreContainer( mMemberGs, 7 );


            message.Store( name1, 8 );
            message.Store( name2, 9 );

            message.StoreContainer( mMemberF, 10 );
            message.StoreContainer( mMemberD, 11 );
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

    template< uint32_t its = 100 >
    class TestClass3NoHelper
        : public ISerialisable
    {
    public:

        TestClass3NoHelper( uint32_t seed = 233232 )
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

        void TestEqual( TestClass3NoHelper &c2 )
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

    template<>
    class Primitive< float >
        : public ISerialisable
    {
    public:

        Primitive( const float &value )
            : mMember( value )
        {
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember, 1 );
        }

        void TestEqual( Primitive &c2 )
        {
            EXPECT_FLOAT_EQ( mMember, c2.mMember );
        }

        float mMember;

    };

    template<>
    class Primitive< double >
        : public ISerialisable
    {
    public:

        Primitive( const double &value )
            : mMember( value )
        {
        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember, 1 );
        }

        void TestEqual( Primitive &c2 )
        {
            EXPECT_DOUBLE_EQ( mMember, c2.mMember );
        }

        double mMember;

    };

    TestEasyRepeatedClass( EasyRepeated, randomVals, 343422, 21331, 5 );
    TestEasyRepeatedClassNoHelper( EasyRepeatedNoHelper, randomVals, 343422, 21331, 5 );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, uint64_t );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int8_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int16_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int32_t );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, int64_t );

    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, float );
    TestRepeatedPrimitiveMessageClass( RepeatedPrimitiveMessage, randomVals, 343422, 21331, 100, double );
}