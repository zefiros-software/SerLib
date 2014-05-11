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
#include "varIntData.h"

#include "serialisation.h"
#include "helper.h"

#include "gtest/gtest.h"

#include <iostream>
#include <cstdio>

template< typename T, uint32_t Flag = 0x00 >
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
        message.Store( mMember, 1, Flag );
    }

    T mMember;
};

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc =  ;
#endif

    testing::InitGoogleTest( &argc, argv );

    Primitive< int8_t, 1 > c1( std::numeric_limits<float>::max() ), c2( 1.0f );
    SimpleSerialiseDeserialiseStream( c1, c2 );

    int result = RUN_ALL_TESTS();

    //system( "pause" );

    return result;
}
