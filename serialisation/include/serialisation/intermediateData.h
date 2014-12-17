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
#ifndef __SERIALISATION_BUFFEREDDATA_H__
#define __SERIALISATION_BUFFEREDDATA_H__

#include "serialisation/interface/IIntermediateData.h"

#include "serialisation/defines.h"
#include "serialisation/types.h"

#include <vector>

template< typename T >
class IntermediateData
    : public IIntermediateData
{
public:

    Internal::Type::Type GetType() const
    {
        return Internal::Type::GetEnum< T >();
    }

    const T &GetValue() const
    {
        return mValue;
    }

    void ReadFrom( StreamBuffer< SERIALISERS_BUFFERSIZE > &streamBuffer )
    {
        streamBuffer.ReadBytes( &mValue, sizeof( T ) );
    }

private:

    T mValue;
};

void IntermediateData< std::string >::ReadFrom( StreamBuffer< SERIALISERS_BUFFERSIZE > &streamBuffer )
{
    uint32_t size;
    streamBuffer.ReadBytes( &size, sizeof( uint32_t ) );

    std::vector< char > buffer( size );

    streamBuffer.ReadBytes( &buffer[0], size );
    mValue = std::string( &buffer[0], size );
}

#endif