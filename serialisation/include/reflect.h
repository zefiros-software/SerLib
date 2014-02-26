#pragma region copyright
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
#pragma endregion

#pragma once
#ifndef __REFLECT_H__
#define __REFLECT_H__

#include "types.h"

#include <assert.h>
#include <string>

#ifdef CPP11
#include <unordered_map>
#else
#include <map>
#endif

class Reflect
{
public:

    void Set( const uint32_t index, const std::string &name );

    const std::string &Get( const uint32_t index ) const;

    bool Exists( const uint32_t index ) const;

private:

#ifdef CPP11
    typedef std::unordered_map< uint32_t, std::string > Map;
#else
    typedef std::map< uint32_t, std::string > Map;
#endif


    Map mNames;
};

#endif