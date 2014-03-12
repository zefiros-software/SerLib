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
#include "interface/ISerialiseData.h"
#include "types.h"
#include "util.h"

#include <assert.h>
#include <stdint.h>
#include <vector>

template< typename DataType, Type::Type SubType >
class RepeatedData
    : public AbstractRepeatedData
{
public:

    virtual ISerialiseData *GetSerialisable( const size_t index )
    {
        return &mFields.at( index );
    }

    virtual void WriteToStream( std::ostream &stream ) const
    {
        for ( typename std::vector< DataType >::const_iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            it->WriteToStream( stream );
        }
    }

    virtual void ReadFromStream( std::istream &stream )
    {
        for ( typename std::vector< DataType >::iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            it->ReadFromStream( stream );
        }
    }

    virtual Type::Type GetSubType() const
    {
        return SubType;
    }

    virtual size_t Size() const
    {
        size_t size = Util::CalculateVarIntSize( Util::CreateHeader( ( uint32_t )mFields.size(), SubType ) );

        for ( typename std::vector< DataType >::const_iterator it = mFields.begin(), end = mFields.end(); it != end; ++it )
        {
            size += it->Size();
        }

        return size;
    }

    virtual void Resize( const size_t size )
    {
        assert( size >= mFields.size() );
        mFields.resize( size );
    }

    virtual uint32_t GetFieldCount() const
    {
        return ( uint32_t )mFields.size();
    }

protected:

    std::vector< DataType > mFields;
};

#endif