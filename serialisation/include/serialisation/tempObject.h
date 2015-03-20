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
#ifndef __SERIALISATION_TEMPOBJECT_H__
#define __SERIALISATION_TEMPOBJECT_H__

#include "interface/ITempData.h"

#include <vector>

class TempObject
    : public ITempData
{
public:

    TempObject()
        : mTerminatorRead( false )
    {
    }

    Internal::Type::Type GetType() const
    {
        return Internal::Type::Object;
    }

    void AddData( uint8_t index, ITempData *data )
    {
        mTempData.push_back( std::pair< uint8_t, ITempData * >( index, data ) );
    }

    ITempData *TryRemoveData( uint8_t index )
    {
        for ( std::vector< std::pair< uint8_t, ITempData * > >::iterator it = mTempData.begin(), end = mTempData.end();
                it != end; ++it )
        {
            if ( it->first == index )
            {
                ITempData *data = it->second;
                mTempData.erase( it );
                return data;
            }
        }

        return NULL;
    }

    bool GetTerminatorRead() const
    {
        return mTerminatorRead;
    }

    void SetTerminatorRead()
    {
        mTerminatorRead = true;
    }

    bool IsNonEmpty() const
    {
        return !mTempData.empty();
    }

private:

    std::vector< std::pair< uint8_t, ITempData * > > mTempData;

    bool mTerminatorRead;
};

#endif