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
#include <string>

class StringClass
{
public:

    StringClass( const std::string &ss ) : str( ss )
    {
    }

    void OnStore( Message &message )
    {
        message.Store( str, 0 );
    }

    std::string str;
};

TEST( P( BasicTest ), EmptyString )
{
    StringClass ss1( "" );
    StringClass ss2( GenerateRandomString() );

    SimpleSerialiseDeserialiseStream( ss1, ss2 );

    EXPECT_EQ( ss1.str, ss2.str );
}

TEST( P( BasicTest ), FullString )
{
    StringClass ss1( GenerateRandomString() );
    StringClass ss2( "" );

    SimpleSerialiseDeserialiseStream( ss1, ss2 );

    EXPECT_EQ( ss1.str, ss2.str );
}

class VectorClass
{
public:

    VectorClass( std::vector< uint8_t > &v ) : vec( v )
    {
    }

    void OnStore( Message &message )
    {
        message.StoreContainer( vec, 0 );
    }

    std::vector< uint8_t > &vec;

private:

    VectorClass &operator=( const VectorClass & );
};

TEST( P( BasicTest ), EmptyContainer )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v2.resize( GetRandom< uint8_t >() );

    VectorClass vc1( v1 );
    VectorClass vc2( v2 );

    SimpleSerialiseDeserialiseStream( vc1, vc2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), FullContrainer )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v1.resize( GetRandom< uint8_t >() );

    VectorClass vc1( v1 );
    VectorClass vc2( v2 );

    SimpleSerialiseDeserialiseStream( vc1, vc2 );

    EXPECT_EQ( v1, v2 );
}

class ArrayClass
{
public:

    ArrayClass( std::vector< uint8_t > &v ) : vec( v )
    {
    }

    void OnStore( Message &message )
    {
        message.StoreContainer( vec, 0 );
    }

    std::vector< uint8_t > &vec;

private:

    ArrayClass &operator=( const ArrayClass & );
};

class ArrayFubarClass
{
public:

    ArrayFubarClass( std::vector< uint8_t > &v ) : vec( v )
    {
    }

    void OnStore( Message &message )
    {
        message.StoreContainer( vec, 0 );
    }

    std::vector< uint8_t > &vec;

private:

    ArrayFubarClass &operator=( const ArrayFubarClass & );
};

class ArrayFubar2Class
{
public:

    ArrayFubar2Class( std::vector< uint8_t > &v ) : vec( v )
    {
    }

    void OnStore( Message &message )
    {
        message.StoreContainer( vec, 0 );
    }

    std::vector< uint8_t > &vec;

private:

    ArrayFubar2Class &operator=( const ArrayFubar2Class & );
};

TEST( P( BasicTest ), EmptyArray )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v2.resize( GetRandom< uint8_t >() );

    ArrayClass ac1( v1 );
    ArrayClass ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), FullArray )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v1.resize( GetRandom< uint8_t >() );

    ArrayClass ac1( v1 );
    ArrayClass ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), EmptyArrayFubar1 )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v2.resize( GetRandom< uint8_t >() );

    ArrayFubarClass ac1( v1 );
    ArrayFubarClass ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), FullArrayFubar1 )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v1.resize( GetRandom< uint8_t >() );

    ArrayFubarClass ac1( v1 );
    ArrayFubarClass ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), EmptyArrayFubar2 )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v2.resize( GetRandom< uint8_t >() );

    ArrayFubar2Class ac1( v1 );
    ArrayFubar2Class ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}

TEST( P( BasicTest ), FullArrayFubar2 )
{
    std::vector< uint8_t > v1;
    std::vector< uint8_t > v2;
    v1.resize( GetRandom< uint8_t >() );

    ArrayFubar2Class ac1( v1 );
    ArrayFubar2Class ac2( v2 );

    SimpleSerialiseDeserialiseStream( ac1, ac2 );

    EXPECT_EQ( v1, v2 );
}