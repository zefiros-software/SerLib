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
#pragma once
#ifndef __SERIALISATION_DENSEBITSET_H__
#define __SERIALISATION_DENSEBITSET_H__

#include <cstdint>
#include <vector>

class DenseBitset
{
public:

    class BitReference
    {
        friend class DenseBitset;

    public:

        inline const BitReference &operator=( bool val ) const
        {
            if ( val )
            {
                mInt |= mMask;
            }
            else
            {
                mInt &= ~mMask;
            }

            return *this;
        }

        inline const BitReference &operator=( const BitReference &br ) const
        {
            return this->operator =( bool( br ) );
        }

        inline operator bool() const
        {
            return ( ( mInt & mMask ) != 0 ) ? true : false;
        }


    private:

        uint8_t &mInt;
        uint8_t mMask;

        BitReference( uint8_t &integer, uint8_t mask ) noexcept;
    };

    DenseBitset( size_t size = 0 )
    {
        Resize( size );
    }

    template< typename tT >
    DenseBitset( tT begin, tT end )
        : mBits( begin, end )
    {
    }

    inline void Resize( size_t size )
    {
        mBits.resize( ( size  + 7 ) >> 3 );
    }

    bool operator[]( size_t bit ) const
    {
        return ( ( mBits[( bit >> 3 )] ) & ( static_cast<uint8_t>( 1 ) << ( bit & 7 ) ) ) != 0;
    }

    BitReference operator[]( size_t bit )
    {
        return BitReference( mBits[bit >> 3], static_cast<uint8_t>( 1 ) << ( bit & 7 ) );
    }

    const std::vector< uint8_t > &GetBits() const
    {
        return mBits;
    }

    template< typename tT >
    void ToVec( std::vector< tT > &vec, size_t size ) const
    {
        vec.resize( size );

        for ( size_t i = 0; i < size; ++i )
        {
            vec[i] = this->operator[]( i );
        }
    }
private:

    std::vector< uint8_t > mBits;
};

#endif