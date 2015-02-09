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
#include "serialisation/message.h"

#include "gtest/gtest.h"

#include <iostream>
#include <cstdio>

#include <stdlib.h>

class Testclass
    : public ISerialisable
{
public:

    Testclass()
    {
        std::stringstream ss;
        ss << rand();

        mValue1 = rand();

        while ( ( mValue2 = Util::UInt32ToFloat( rand() ) ) == std::numeric_limits< float >::infinity() );

        mValue3 = ss.str();
    }

    void SERIALISATION_CUSTOM_INTERFACE( Message &message )
    {
        message.Store( mValue1, 0 );
        message.Store( mValue2, 1 );
        message.Store( mValue3, 2 );
    }

private:

    uint32_t mValue1;
    float mValue2;
    std::string mValue3;
};

class Testclass2
	: public ISerialisable
{
public:

	Testclass2()
	{
		std::stringstream ss;
		ss << rand();

		mValue1 = rand();

		while ( ( mValue2 = Util::UInt32ToFloat( rand() ) ) == std::numeric_limits< float >::infinity() );

		mValue3 = ss.str();
	}

	void SERIALISATION_CUSTOM_INTERFACE( Message &message )
	{
		message.Store( mValue4, 3 );
		message.Store( mValue3, 2 );
		message.Store( mValue2, 1 );
		message.Store( mValue1, 0 );
	}

private:

	uint32_t mValue1;
	float mValue2;
	std::string mValue3;
	Testclass mValue4;
};

class Testclass3
	: public ISerialisable
{
public:

	Testclass3()
	{
		std::stringstream ss;
		ss << rand();

		mValue1 = rand();

		while ( ( mValue2 = Util::UInt32ToFloat( rand() ) ) == std::numeric_limits< float >::infinity() );

		mValue3 = ss.str();
	}

	void SERIALISATION_CUSTOM_INTERFACE( Message &message )
	{
		message.Store( mValue3, 2 );
		message.Store( mValue2, 1 );
		message.Store( mValue1, 0 );
		message.Store( mValue4, 3 );
	}

private:

	uint32_t mValue1;
	float mValue2;
	std::string mValue3;
	Testclass mValue4;
};

int main( int argc, char **argv )
{

#ifdef _WIN32
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc = 4568;
#endif

    testing::InitGoogleTest( &argc, argv );

    int result = RUN_ALL_TESTS();
	
    system( "pause" );

    return result;
}
