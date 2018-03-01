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
#ifndef __SERIALISATION_BUFFEREDWRITE_CPP__
#define __SERIALISATION_BUFFEREDWRITE_CPP__

#include "serialisation/bufferedStreamWriter.h"

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter(const std::string &fileName)
    : mStreamWriter(fileName),
      mWriteIndex(0)
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter(std::ofstream &stream)
    : mStreamWriter(stream),
      mWriteIndex(0)
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter(std::fstream &stream)
    : mStreamWriter(stream),
      mWriteIndex(0)
{

}

SERIALISATION_INLINE BufferedStreamWriter::BufferedStreamWriter(std::ostream &stream)
    : mStreamWriter(stream),
      mWriteIndex(0)
{

}

SERIALISATION_INLINE BufferedStreamWriter::~BufferedStreamWriter()
{
    Close();
}

SERIALISATION_INLINE void BufferedStreamWriter::ClearBuffer()
{
    mStreamWriter.WriteBytes(mWriteBuffer, mWriteIndex);
    mWriteIndex = 0;

    mStreamWriter.ClearBuffer();
}

SERIALISATION_INLINE void BufferedStreamWriter::Close()
{
    ClearBuffer();

    mStreamWriter.Close();
}

#endif