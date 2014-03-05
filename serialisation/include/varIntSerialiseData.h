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
#ifndef __SERIALIZATION_VARINTSERIALISEDATA_H__
#define __SERIALIZATION_VARINTSERIALISEDATA_H__

#include "interface/ISerialiseData.h"

#include <stdint.h>

class VarIntSerialiseData
    : public ISerialiseData
{
public:

    VarIntSerialiseData() : mValue(0)
    {

    }

    template< typename U >
    void Store( U &val, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            mValue = val;
            break;

        case Mode::Deserialise:
            val = ( U )mValue;
            break;
        }
    }

    virtual Type::Type GetType() const;

    virtual size_t Size() const;

    virtual void ReadFromStream( std::istream &stream );

    virtual void WriteToStream( std::ostream &stream ) const;

private:

    uint64_t mValue;
};

#endif