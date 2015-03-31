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

#pragma once
#ifndef __HELPER_H__
#define __HELPER_H__

#define CONCATEXT( a, b ) a##b
#define CONCAT( a, b ) CONCATEXT( a, b )
#define P( prefix ) CONCAT( PREFIX, prefix )

#define TEST_FILE_DETAIL( file, suite, test ) file #suite #test
#define TEST_FILE( suite, test ) TEST_FILE_DETAIL("../../test/test-files/", suite, _ ## test ## .bin )

#include "serialisation/message.h"

#include <sstream>
#include <cstdlib>
#include <limits>

template< typename T1, typename T2 >
void SimpleSerialiseDeserialiseStream( T1 &c1, T2 &c2 )
{
    std::stringstream ss;
    {
        Message message( ss, Format::Binary, Mode::Serialise );
        MessageHelper::Store( message, c1 );
    }
    {
        Message message( ss, Format::Binary, Mode::Deserialise );
        MessageHelper::Store( message, c2 );
    }
}

template< typename T1, typename T2 >
void SimpleSerialiseDeserialiseBackwards( const std::string &file, T1 &c1/**/, T2 &c2 )
{
    /* Enable this when you need to regenerate the backwards compatibility files
    {
        // clear the file if needed
        std::ofstream ofs;
        ofs.open( file.c_str(), std::ofstream::out | std::ofstream::trunc );
        ofs.close();

        Message message( file, Format::Binary, Mode::Serialise );
        MessageHelper::Store( message, c1 );
    }
    /// */

    {
        Message message( file, Format::Binary, Mode::Deserialise );
        MessageHelper::Store( message, c2 );
    }
}


template< typename T1, typename T2 >
void SimpleSerialiseDeserialiseFile( T1 &c1, T2 &c2 )
{
    // clear file
    {
        std::ofstream ofs;
        ofs.open( "file.bin", std::ofstream::out | std::ofstream::trunc );
        ofs.close();
    }
    {
        Message message( "file.bin", Format::Binary, Mode::Serialise );
        MessageHelper::Store( message, c1 );
    }
    {
        Message message( "file.bin", Format::Binary, Mode::Deserialise );
        MessageHelper::Store( message, c2 );
    }
}

template< typename T >
T GenerateZebraValue()
{
    const uint16_t bits = sizeof( T ) << 3;
    T result = 0;

    for ( uint16_t i = 0; i < bits; ++++i )
    {
        result |= ( T )( 1ull << i );
    }

    return result;
}

template< typename T >
T GenerateInvZebraValue()
{
    return GenerateZebraValue< T >() ^ std::numeric_limits<T>::max();
}

static uint32_t g_seed = 0;

inline int GetFastRand()
{
    g_seed = ( 214013 * g_seed + 2531011 );
    return ( g_seed >> 16 ) & 0x7FFF;
}

template< typename T >
T GetRandom()
{
    return static_cast< T >( ( ( double )GetFastRand() / RAND_MAX ) * std::numeric_limits< T >::max() );
}

template<>
float GetRandom< float >();


template<>
double GetRandom< double >();


std::string GenerateRandomString();

#endif
