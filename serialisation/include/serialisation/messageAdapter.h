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
#ifndef __MESSAGEADAPTER_H__
#define __MESSAGEADAPTER_H__

#include "types.h"
#include "defines.h"
#include "streamBuffer.h"

template< typename TMessage >
class MessageAdapter
    : public InternalMessage
{
public:

    MessageAdapter( StreamBuffer< SERIALISERS_BUFFERSIZE > &buffer )
        : mMessage( buffer )
    {

    }

    void InitObject()
    {
        mMessage.InitObject();
    }

    void FinishObject()
    {
        mMessage.FinishObject();
    }

    inline void InitObject( uint8_t index )
    {
        mMessage.InitObject( index );
    }

    inline void FinishObject( uint8_t index )
    {
        mMessage.FinishObject( index );
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


private:

    TMessage mMessage;
};

#endif