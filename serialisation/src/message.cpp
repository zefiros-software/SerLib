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
#ifndef __SERIALISATION_MESSAGE_CPP__
#define __SERIALISATION_MESSAGE_CPP__

#include "serialisation/message.h"

SERIALISATION_INLINE Message::Message(const std::string &fileName, Format::Format format,
                                      Mode::Mode mode /*= Mode::Serialise*/,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateInternalMessage(format, mode, fileName, useBuffer)),
      mMode(mode),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::stringstream &stream, Format::Format format,
                                      Mode::Mode mode /*= Mode::Serialise*/,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateInternalMessage(format, mode, stream, useBuffer)),
      mMode(mode),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::iostream &stream, Format::Format format,
                                      Mode::Mode mode /*= Mode::Serialise*/,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateInternalMessage(format, mode, stream, useBuffer)),
      mMode(mode),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::fstream &stream, Format::Format format,
                                      Mode::Mode mode /*= Mode::Serialise*/,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateInternalMessage(format, mode, stream, useBuffer)),
      mMode(mode),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::ifstream &stream, Format::Format format,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateDeserMessage(format, stream, useBuffer)),
      mMode(Mode::Deserialise),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::istream &stream, Format::Format format,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateDeserMessage(format, stream, useBuffer)),
      mMode(Mode::Deserialise),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::ofstream &stream, Format::Format format,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateSerMessage(format, stream, useBuffer)),
      mMode(Mode::Serialise),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::Message(std::ostream &stream, Format::Format format,
                                      bool useBuffer /*= true */)
    : mInternalMessage(CreateSerMessage(format, stream, useBuffer)),
      mMode(Mode::Serialise),
      mFormat(format)
{

}

SERIALISATION_INLINE Message::~Message()
{
    ClearBuffers();
    DeleteInternalMessage(mInternalMessage);
}

SERIALISATION_INLINE void Message::ClearBuffers()
{
    mInternalMessage->ClearBuffer();
}

SERIALISATION_INLINE Mode::Mode Message::GetMode()
{
    return mMode;
}

SERIALISATION_INLINE Format::Format Message::GetFormat()
{
    return mFormat;
}

SERIALISATION_INLINE void Message::Store(ISerialisable &serialisable, uint8_t index)
{
    Store< ISerialisable >(serialisable, index);
}

SERIALISATION_INLINE void Message::Store(std::string &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(uint8_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(uint16_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(uint32_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(uint64_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(int8_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(int16_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(int32_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(int64_t &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(float &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(double &value, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    mInternalMessage->Store(value, index);
}

SERIALISATION_INLINE void Message::Store(std::function< void() > func, uint8_t index)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    if (mInternalMessage->InitObject(index))
    {
        func();

        mInternalMessage->FinishObject(index);
    }
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< uint8_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< uint16_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< uint32_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< uint64_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< int8_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< int16_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< int32_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< int64_t > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< float > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreContainer(std::vector< double > &container, uint8_t index,
                                                  uint8_t flags /*= 0x00 */)
{
    StorePrimitiveVector(container, index, flags);
}

SERIALISATION_INLINE void Message::StoreArrayItem(ISerialisable &value)
{
    StoreArrayItem< ISerialisable >(value);
}


SERIALISATION_INLINE void Message::StoreArrayItem(std::string &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(uint8_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(uint16_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(uint32_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(uint64_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(int8_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(int16_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(int32_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(int64_t &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(float &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE void Message::StoreArrayItem(double &value)
{
    mInternalMessage->StoreArrayItem(value);
}

SERIALISATION_INLINE size_t Message::CreateArray(Type::Type type, size_t size, uint8_t index,
                                                 uint8_t flags /*= 0x00 */)
{
    SERIALISATION_ASSERT_INDEX_IN_RANGE(index);

    return mInternalMessage->CreateArray(type, size, index, flags);
}

#endif