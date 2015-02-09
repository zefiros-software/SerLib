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
#ifndef __SERIALISATION_TEMPARRAY_H__
#define __SERIALISATION_TEMPARRAY_H__

#include "interface/abstractTempArray.h"

template< typename T >
class TempArray
    : public AbstractTempArray
{
public:

    ~TempArray()
    {
    }

    Internal::Type::Type GetSubType() const
    {
        return Internal::Type::GetEnum< T >();
    }

    T *GetData()
    {
        return &mValues.front();
    }

    void Resize( const size_t size )
    {
        mValues.resize( size );
    }

    void PushBack( const T &value )
    {
        mValues.push_back( value );
    }

    void PopFront( T &dest )
    {
        dest = mValues.front();
        mValues.erase( mValues.begin() );
    }

    size_t GetRemainingCount() const
    {
        return mValues.size();
    }

private:

    std::vector< T > mValues;
};

TempArray< TempObject * >::~TempArray()
{
    for ( std::vector<TempObject *>::iterator it = mValues.begin(), end = mValues.end(); it != end; ++it )
    {
        delete *it;
    }
}

#endif