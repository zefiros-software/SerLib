/**
 * Copyright (c) 2017 Zefiros Software.
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
#include "serialisation/tempObject.h"
#include "serialisation/defines.h"

SERIALISATION_INLINE TempObject::TempObject()
    : mTerminatorRead( false )
{
}

SERIALISATION_INLINE Internal::Type::Type TempObject::GetType() const
{
    return Internal::Type::Object;
}

SERIALISATION_INLINE void TempObject::AddData( uint8_t index, ITempData *data )
{
    mTempData.push_back( std::pair< uint8_t, ITempData * >( index, data ) );
}

SERIALISATION_INLINE ITempData *TempObject::TryRemoveData( uint8_t index )
{
    typedef std::vector< std::pair< uint8_t, ITempData * > >::iterator iterator;

    for ( iterator it = mTempData.begin(), end = mTempData.end(); it != end; ++it )
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

SERIALISATION_INLINE size_t TempObject::GetSize()
{
    return mTempData.size();
}

SERIALISATION_INLINE ITempData *TempObject::PopBack()
{
    ITempData *temp = mTempData.back().second;
    mTempData.pop_back();
    return temp;
}

SERIALISATION_INLINE bool TempObject::GetTerminatorRead() const
{
    return mTerminatorRead;
}

SERIALISATION_INLINE void TempObject::SetTerminatorRead()
{
    mTerminatorRead = true;
}

SERIALISATION_INLINE bool TempObject::IsNonEmpty() const
{
    return !mTempData.empty();
}