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

#define TestParent( test, name, seed1, seed2 )              \
    TEST( P( test ), name ## _stream )                      \
    {                                                       \
        Child c1( seed1 );                                  \
        ReorderedChild c2( seed2 );                         \
        SimpleSerialiseDeserialiseStream( c1, c2 );         \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), name ## _file )                        \
    {                                                       \
        Child c1( seed1 );                                  \
        ReorderedChild c2( seed2 );                         \
        SimpleSerialiseDeserialiseFile( c1, c2 );           \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), name ## _backwards )                   \
    {                                                       \
        Child c1( seed1 );                                  \
        ReorderedChild c2( seed2 );                         \
        std::string file = TEST_FILE( test, name);          \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );\
        c1.TestEqual( c2 );                                 \
    }

#define TestInterfacedParent( test, name, seed1, seed2 )    \
    TEST( P( test ), name ## _stream )                      \
    {                                                       \
        InterfacedChild c1( seed1 );                        \
        InterfacedReorderedChild c2( seed2 );               \
        SimpleSerialiseDeserialiseStream( c1, c2 );         \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), name ## _file )                        \
    {                                                       \
        InterfacedChild c1( seed1 );                        \
        InterfacedReorderedChild c2( seed2 );               \
        SimpleSerialiseDeserialiseFile( c1, c2 );           \
        c1.TestEqual( c2 );                                 \
    }                                                       \
    TEST( P( test ), name ## _backwards )                   \
    {                                                       \
        InterfacedChild c1( seed1 );                        \
        InterfacedReorderedChild c2( seed2 );               \
        std::string file = TEST_FILE( test, name);          \
        SimpleSerialiseDeserialiseBackwards( file, c1, c2 );\
        c1.TestEqual( c2 );                                 \
    }

namespace TestClasses
{
    class AbstractParent
    {
    public:

        AbstractParent( uint32_t seed )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint8_t >();
            mMember2 = GetRandom< uint64_t >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( AbstractParent &c2 )
        {
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint8_t mMember1;
        uint64_t mMember2;
    };

    class Parent
        : public AbstractParent
    {
    public:

        Parent( uint32_t seed )
            : AbstractParent( seed + 1 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint32_t >();
            std::stringstream ss;
            ss << GetRandom< uint64_t >();
            mMember2 = ss.str();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent< AbstractParent >( *this, 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( Parent &c2 )
        {
            AbstractParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint32_t mMember1;
        std::string mMember2;

    };

    class SecondParent
        : public AbstractParent
    {
    public:

        SecondParent( uint32_t seed )
            : AbstractParent( seed + 2 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint8_t >();
            mMember2 = GetRandom< float >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent< AbstractParent >( *this, 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( SecondParent &c2 )
        {
            AbstractParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_FLOAT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint8_t mMember1;
        float mMember2;
    };

    class Child
        : public Parent, public SecondParent
    {
    public:

        Child( uint32_t seed )
            : Parent( seed + 3 ), SecondParent( seed + 4 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint32_t >();
            mMember2 = GetRandom< double >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent< Parent >( *this, 0 );
            message.Store( mMember2, 1 );
            message.StoreParent< SecondParent >( *this, 1 );
        }

        virtual void TestEqual( Child &c2 )
        {
            Parent::TestEqual( c2 );
            SecondParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_DOUBLE_EQ( mMember2, c2.mMember2 );
        }

    protected:

        uint32_t mMember1;
        double mMember2;
    };

    class ReorderedChild
        : public Child
    {
    public:

        ReorderedChild( uint32_t seed )
            : Child( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.StoreParent< SecondParent >( *this, 1 );
            message.Store( mMember2, 1 );
            message.StoreParent< Parent >( *this, 0 );
            message.Store( mMember1, 0 );
        }
    };

    class InterfacedAbstractParent
        : public ISerialisable
    {
    public:

        InterfacedAbstractParent( uint32_t seed )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint8_t >();
            mMember2 = GetRandom< uint64_t >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( InterfacedAbstractParent &c2 )
        {
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint8_t mMember1;
        uint64_t mMember2;
    };

    class InterfacedParent
        : public InterfacedAbstractParent
    {
    public:

        InterfacedParent( uint32_t seed )
            : InterfacedAbstractParent( seed + 1 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint32_t >();
            std::stringstream ss;
            ss << GetRandom< uint64_t >();
            mMember2 = ss.str();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent( *( ( InterfacedAbstractParent * )this ), 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( InterfacedParent &c2 )
        {
            InterfacedAbstractParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint32_t mMember1;
        std::string mMember2;

    };

    class InterfacedSecondParent
        : public InterfacedAbstractParent
    {
    public:

        InterfacedSecondParent( uint32_t seed )
            : InterfacedAbstractParent( seed + 2 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint8_t >();
            mMember2 = GetRandom< float >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent( *( ( InterfacedAbstractParent * )this ), 0 );
            message.Store( mMember2, 1 );
        }

        virtual void TestEqual( InterfacedSecondParent &c2 )
        {
            InterfacedAbstractParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_FLOAT_EQ( mMember2, c2.mMember2 );
        }

    private:

        uint8_t mMember1;
        float mMember2;
    };

    class InterfacedChild
        : public InterfacedParent, public InterfacedSecondParent
    {
    public:

        InterfacedChild( uint32_t seed )
            : InterfacedParent( seed + 3 ), InterfacedSecondParent( seed + 4 )
        {
            g_seed = seed;
            mMember1 = GetRandom< uint32_t >();
            mMember2 = GetRandom< double >();
        }

        virtual void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.Store( mMember1, 0 );
            message.StoreParent( *( ( InterfacedParent * )this ), 0 );
            message.Store( mMember2, 1 );
            message.StoreParent( *( ( InterfacedSecondParent * )this ), 1 );
        }

        virtual void TestEqual( InterfacedChild &c2 )
        {
            InterfacedParent::TestEqual( c2 );
            InterfacedSecondParent::TestEqual( c2 );
            ASSERT_EQ( mMember1, c2.mMember1 );
            ASSERT_DOUBLE_EQ( mMember2, c2.mMember2 );
        }

    protected:

        uint32_t mMember1;
        double mMember2;
    };

    class InterfacedReorderedChild
        : public InterfacedChild
    {
    public:

        InterfacedReorderedChild( uint32_t seed )
            : InterfacedChild( seed )
        {

        }

        void SERIALISATION_CUSTOM_INTERFACE( Message &message )
        {
            message.StoreParent( *( ( InterfacedSecondParent * )this ), 1 );
            message.Store( mMember2, 1 );
            message.StoreParent( *( ( InterfacedParent * )this ), 0 );
            message.Store( mMember1, 0 );
        }
    };



    TestParent( TestParent, DoubleInheritance, 343422, 21331 );
    TestInterfacedParent( TestInterfacedParent, DoubleInheritance, 343422, 21331 );
}