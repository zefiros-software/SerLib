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
#ifndef __SERIALISATION_ABSTRACTSERIALISEDATA_H__
#define __SERIALISATION_ABSTRACTSERIALISEDATA_H__

#include "interface/abstractSerialiser.h"
#include "interface/ISerialiseData.h"
#include "interface/ISerialisable.h"

#include "types.h"

#include <stdint.h>

class AbstractRepeatedData
    : public ISerialiseData
{
public:

    virtual ISerialiseData *GetSerialisable( const uint32_t index ) = 0;

    virtual Internal::Type::Type GetSubType() const = 0;

    virtual uint32_t Count() const = 0;

    virtual void Resize( const size_t size ) = 0;

    virtual Internal::Type::Type GetType() const
    {
        return Internal::Type::Repeated;
	}

	inline virtual void Store( std::string &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	};

	inline virtual void Store( uint8_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( uint16_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( uint32_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( uint64_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( int8_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( int16_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( int32_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( int64_t &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( float &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( double &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline virtual void Store( ISerialisable &, const uint32_t, const Mode::Mode )
	{
		StoreError();
	}

	inline void StoreError() const
	{
		throw std::invalid_argument( "Invalid conversion" );
	}
};

#endif