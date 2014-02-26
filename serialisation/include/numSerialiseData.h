#pragma region copyright
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
#pragma endregion

#pragma once
#ifndef __NUMSERIALISEDATA_H__
#define __NUMSERIALISEDATA_H__

#include "ISerialiseData.h"

template< typename U, Type::Type T >
class NumSerialiseData
    : public ISerialiseData
{
public:

    template< typename D >
    void Store( D &val, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            mValue = ( U )val;
            break;

        case Mode::Deserialise:
            val = ( D )mValue;
            break;
        }
    }

    virtual Type::Type GetType() const
    {
        return T;
    }

    virtual size_t Size() const
    {
        return sizeof( U );
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        stream.read( ( char * )&mValue, sizeof( U ) );
    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        stream.write( ( const char * )&mValue, sizeof( U ) );
    }

protected:

    U mValue;
};

typedef NumSerialiseData< uint8_t, Type::Char > CharSerialiseData;
typedef NumSerialiseData< uint16_t, Type::WORD > WORDSerialiseData;
typedef NumSerialiseData< uint32_t, Type::DWORD > DWORDSerialiseData;
typedef NumSerialiseData< uint64_t, Type::QWORD > QWORDSerialiseData;

#endif