/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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
 *
 * @endcond
 */

#pragma once
#ifndef __SERIALISATION_TYPES_CPP__
#define __SERIALISATION_TYPES_CPP__

#include "serialisation/defines.h"
#include "serialisation/types.h"

SERIALISATION_INLINE bool Internal::Type::IsInteger(Type type)
{
    return (type >= UInt8) && (type <= SInt64);
}

SERIALISATION_INLINE bool Internal::Type::IsSignedInt(Type type)
{
    return (type >= SInt8) && (type <= SInt64);
}

SERIALISATION_INLINE bool Internal::Type::AreCompatible(const Type type, const Type type2)
{
    if (type == type2)
    {
        return true;
    }

    if ((IsSignedInt(type) && type - SInt8 == type2 - UInt8) || (IsSignedInt(type2) && type2 - SInt8 == type - UInt8))
    {
        return true;
    }


    if ((type == Float && type2 == UInt32) || (type2 == Float && type == UInt32))
    {
        return true;
    }

    if ((type == Double && type2 == UInt64) || (type2 == Double && type == UInt64))
    {
        return true;
    }

    if (IsInteger(type) && IsInteger(type2))
    {
        return true;
    }

    return false;
}

#endif