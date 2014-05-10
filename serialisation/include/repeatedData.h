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
#ifndef __SERIALISATION_REPEATEDDATA_H__
#define __SERIALISATION_REPEATEDDATA_H__

#include "interface/abstractRepeatedData.h"
#include "types.h"
#include "util.h"
#include "poolHolder.h"

#include <assert.h>
#include <stdint.h>
#include <vector>

class AbstractSerialiser;

template< typename DataType >
class RepeatedData
    : public AbstractRepeatedData
{
public:

    RepeatedData( const uint32_t flags = 0 )
        : mFlags( flags )
    {

    }

    virtual uint32_t GetFlags() const
    {
        return mFlags;
    }

    virtual void SetFlags( const uint32_t flags )
    {
        mFlags = flags;
    }

    virtual ISerialiseData *GetSerialisable( const uint32_t index )
    {
        return GetConcreteSerialisable( index );
    }

    virtual DataType *GetConcreteSerialisable( const uint32_t index )
    {
        return mFields.at( index );
    }

    virtual Internal::Type::Type GetSubType() const
    {
        return Internal::Type::GetEnum< DataType >();
    }

    virtual void Resize( const size_t size )
    {
        size_t oldSize = mFields.size();

        ObjectPool< DataType > &pool = PoolHolder::Get().GetPool< DataType >();

        if ( oldSize < size )
        {
            for ( size_t i = oldSize; i < size; ++ i )
            {
                DataType *const field = pool.Get();
                field->SetFlags( mFlags );
                mFields.push_back( field );
            }
        }
        else
        {
            for ( size_t i = 0, end = size - oldSize; i < end; ++i )
            {
                DataType *const field = mFields.back();
				mFields.pop_back();

				field->SetFlags(0);
				pool.Dispose(field);
            }
        }
    }

    virtual uint32_t Count() const
    {
        return ( uint32_t )mFields.size();
    }

    virtual void SerialiseTo( AbstractSerialiser *const serialiser )
    {
        serialiser->Serialise( this );
    }

protected:

    std::vector< DataType * > mFields;
    uint32_t mFlags;
};

#endif