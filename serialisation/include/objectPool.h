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
#ifndef __SERIALISATION_OBJECTPOOL_H__
#define __SERIALISATION_OBJECTPOOL_H__

#include <assert.h>
#include <cstddef>
#include <vector>

template< typename T >
class ObjectPool
{
public:

    ObjectPool( const std::size_t capacity = 5000 )
        : mCapacity( capacity ),
          mBorrowedCount( 0 ),
          mReturnedCount( 0 )
    {
    }

    ~ObjectPool()
    {
        for ( typename std::vector< T * >::iterator it = mPool.begin(), end = mPool.end(); it != end; ++it )
        {
            delete *it;
        }

        assert( mBorrowedCount == mReturnedCount );
    }

    T *Get()
    {
        ++mBorrowedCount;

        return CreateInstance();
    }

    void GetMultiple( typename std::vector< T * > &vector, const size_t count )
    {
        mBorrowedCount += count;

        const size_t size = mPool.size();
        const size_t actualCount = count > size ? size : count;

		typename std::vector< T * >::iterator begin = mPool.begin() + size - actualCount;
		typename std::vector< T * >::iterator end = mPool.end();

        vector.insert( vector.end(), begin, end );
		mPool.erase( begin, end );

        for ( size_t i = size; i < count; ++i )
        {
            vector.push_back( new T() );
        }
    }

    void Dispose( T *const object )
    {
        ++mReturnedCount;

        if ( mPool.size() < mCapacity )
        {
            mPool.push_back( object );
        }
        else
        {
            delete object;
        }
    }

    void DisposeMultiple( typename std::vector< T * > &vector, typename std::vector< T * >::iterator begin,
                          typename std::vector< T * >::iterator end )
    {
        const size_t remaining = mCapacity - mPool.size();
        const size_t count = end - begin;
        const size_t actualCount = count < remaining ? count : remaining;

        mPool.insert( mPool.end(), begin, begin + actualCount );

        for ( typename std::vector< T * >::iterator it = begin + actualCount; it != end; ++it )
        {
            delete *it;
        }

        vector.erase( begin, end );
    }

    std::size_t GetBorrowedCount() const
    {
        return mBorrowedCount;
    }

    std::size_t GetReturnedCount() const
    {
        return mReturnedCount;
    }

private:

    std::vector< T * > mPool;

    std::size_t mCapacity;
    std::size_t mBorrowedCount;
    std::size_t mReturnedCount;

    T *CreateInstance()
    {
        T *object = NULL;

        if ( !mPool.empty() )
        {
            object = mPool.back();
            mPool.pop_back();
        }
        else
        {
            object = new T();
        }

        return object;
    }
};

#endif