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
#ifndef __SERIALISATION_MESSAGEADAPTER_H__
#define __SERIALISATION_MESSAGEADAPTER_H__

#include "internalMessage.h"
#include "defines.h"
#include "types.h"

template< typename TMessage >
class MessageAdapter
    : public InternalMessage
{
public:

	MessageAdapter( const std::string &fileName )
		: mMessage( fileName )
	{
	}

	MessageAdapter( std::stringstream &stream )
		: mMessage( stream )
	{

	}

	MessageAdapter( std::iostream &stream )
		: mMessage( stream )
	{

	}

	MessageAdapter( std::ifstream &stream )
		: mMessage( stream )
	{
	}

	MessageAdapter( std::fstream &stream )
		: mMessage( stream )
	{
	}

	MessageAdapter( std::istream &stream )
		: mMessage( stream )
	{
	}

	MessageAdapter( std::ofstream &stream )
		: mMessage( stream )
	{
	}

	MessageAdapter( std::ostream &stream )
		: mMessage( stream )
	{
	}

	void ClearBuffer()
	{
		mMessage.ClearBuffer();
	}

    void InitObject()
    {
        mMessage.InitObject();
    }

    inline bool InitObject( uint8_t index )
    {
        return mMessage.InitObject( index );
    }

    void FinishObject()
    {
        mMessage.FinishObject();
    }

    inline void FinishObject( uint8_t index )
    {
        mMessage.FinishObject( index );
    }

    inline bool InitParent( uint8_t index )
    {
        return mMessage.InitParent( index );
    }

    inline void FinishParent( uint8_t index )
    {
        mMessage.FinishParent( index );
    }

    inline void InitArrayObject()
    {
        mMessage.InitArrayObject();
    }

    inline void FinishArrayObject()
    {
        mMessage.FinishArrayObject();
    }

    inline void Store( std::string &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( uint8_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( uint16_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( uint32_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( uint64_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( int8_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( int16_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }
    void Store( int32_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( int64_t &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }


    void Store( float &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    void Store( double &value, uint8_t index )
    {
        mMessage.Store( value, index );
    }

    size_t CreateArray( Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00 )
    {
        return mMessage.CreateArray( type, size, index, flags );
    }

    void StoreArrayItem( std::string &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( uint8_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( uint16_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( uint32_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( uint64_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( int8_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( int16_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( int32_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( int64_t &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( float &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreArrayItem( double &value )
    {
        mMessage.StoreArrayItem( value );
    }

    void StoreContiguous( uint8_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( uint16_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( uint32_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( uint64_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( int8_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( int16_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( int32_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( int64_t *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( float *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

    void StoreContiguous( double *begin, size_t size )
    {
        mMessage.StoreContiguous( begin, size );
    }

protected:

    TMessage mMessage;

private:

    MessageAdapter &operator=( const MessageAdapter & );
};
#endif