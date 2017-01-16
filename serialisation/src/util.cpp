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
#include "serialisation/defines.h"
#include "serialisation/util.h"

SERIALISATION_INLINE uint32_t Util::FloatToUInt32( const float f )
{
    int32_t exp;
    float fi = frexp( f, &exp );
    --exp;

    uint32_t result = ZigZag< int32_t, uint32_t >( exp );
    result |= ZigZag< int32_t, uint32_t >( static_cast<int32_t>( ldexp( fi, 23 ) ) ) << 8;

    return result;
}

SERIALISATION_INLINE float Util::UInt32ToFloat( const uint32_t i )
{
    int32_t exp = ZagZig< uint32_t, int32_t >( i & 0xff );
    ++exp;
    return ldexp( ldexp( static_cast<float>( ZagZig< uint32_t, int32_t >( i >> 8 ) ), -23 ), exp );
}

SERIALISATION_INLINE uint64_t Util::DoubleToUInt64( const double f )
{
    int32_t exp;
    double fi = frexp( f, &exp );
    --exp;

    uint64_t result = ZigZag< int64_t, uint64_t >( exp );
    result |= ZigZag< int64_t, uint64_t >( static_cast<int64_t>( ldexp( fi, 52 ) ) ) << 11;

    return result;
}

SERIALISATION_INLINE double Util::UInt64ToDouble( const uint64_t i )
{
    int32_t exp = ZagZig< uint32_t, int32_t >( i & 0x7ff );
    ++exp;
    return ldexp( ldexp( static_cast<double>( ZagZig< uint64_t, int64_t >( i >> 11 ) ), -52 ), exp );
}