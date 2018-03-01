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
#ifndef __SERIALISATION_BINARYSERMESSAGE_H__
#define __SERIALISATION_BINARYSERMESSAGE_H__

#include "serialisation/util.h"

#include <fstream>

template< typename TStreamWriter >
class BinarySerMessage
{
public:

    BinarySerMessage(const std::string &fileName)
        : mStreamWriter(fileName)
    {
    }

    BinarySerMessage(std::ofstream &stream)
        : mStreamWriter(stream)
    {
    }

    BinarySerMessage(std::fstream &stream)
        : mStreamWriter(stream)
    {
    }

    BinarySerMessage(std::ostream &stream)
        : mStreamWriter(stream)
    {
    }

    SERIALISATION_FORCEINLINE void InitObject()
    {
    }

    SERIALISATION_FORCEINLINE void FinishObject()
    {
        const uint8_t terminator = (uint8_t)Internal::Type::Terminator & 0x07;
        WritePrimitive(terminator);
    }

    SERIALISATION_FORCEINLINE bool InitObject(uint8_t index)
    {
        WriteHeader(index, Internal::Type::Object);
        return true;
    }

    SERIALISATION_FORCEINLINE void FinishObject(uint8_t /*index*/)
    {
        FinishObject();
    }

    SERIALISATION_FORCEINLINE bool InitParent(uint8_t index)
    {
        return InitObject(index + 28);
    }

    SERIALISATION_FORCEINLINE void FinishParent(uint8_t index)
    {
        FinishObject(index + 28);
    }

    SERIALISATION_FORCEINLINE void InitArrayObject()
    {
    }

    SERIALISATION_FORCEINLINE void FinishArrayObject()
    {
        FinishObject();
    }

    SERIALISATION_FORCEINLINE void ClearBuffer()
    {
        mStreamWriter.ClearBuffer();
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void Store(TPrimitive &value, uint8_t index)
    {
        const Internal::Type::Type type = Internal::Type::GetEnum< TPrimitive >();

        WriteHeader(index, type);
        WritePrimitive(value);
    }

    SERIALISATION_FORCEINLINE void Store(std::string &value, uint8_t index)
    {
        WriteHeader(index, Internal::Type::String);
        WritePrimitive(value);
    }

    SERIALISATION_FORCEINLINE void Store(float &value, uint8_t index)
    {
        uint32_t flexman = Util::FloatToUInt32(value);

        Store(flexman, index);
    }

    SERIALISATION_FORCEINLINE void Store(double &value, uint8_t index)
    {
        uint64_t flexman = Util::DoubleToUInt64(value);

        Store(flexman, index);
    }

    SERIALISATION_FORCEINLINE size_t CreateArray(Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00)
    {
        const Internal::Type::Type iType = static_cast< Internal::Type::Type >(type);

        WriteHeader(index, Internal::Type::Array);
        WriteHeader(flags, iType);

        mStreamWriter.WriteSize(size);

        return size;
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void StoreArrayItem(TPrimitive &value)
    {
        WritePrimitive(value);
    }

    SERIALISATION_FORCEINLINE void StoreArrayItem(float &value)
    {
        uint32_t flexman = Util::FloatToUInt32(value);

        StoreArrayItem(flexman);
    }

    SERIALISATION_FORCEINLINE void StoreArrayItem(double &value)
    {
        uint64_t flexman = Util::DoubleToUInt64(value);

        StoreArrayItem(flexman);
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void StoreContiguous(TPrimitive *begin, size_t size)
    {
        mStreamWriter.WritePrimitiveBlock(begin, size);
    }

    inline void StoreContiguous(float *begin, size_t size)
    {
        for (size_t i = 0; i < size; i += 128, begin += 128)
        {
            size_t blockSize = static_cast< size_t >(size - i);
            blockSize = blockSize > 128 ? 128 : blockSize;

            for (size_t k = 0; k < blockSize; ++k)
            {
                mU32Buffer[ k ] = Util::FloatToUInt32(begin[ k ]);
            }

            mStreamWriter.WritePrimitiveBlock(mU32Buffer, blockSize);
        }
    }

    inline void StoreContiguous(double *begin, size_t size)
    {
        for (size_t i = 0; i < size; i += 128, begin += 128)
        {
            size_t blockSize = static_cast<size_t>(size - i);
            blockSize = blockSize > 128 ? 128 : blockSize;

            for (size_t k = 0; k < blockSize; ++k)
            {
                mU64Buffer[ k ] = Util::DoubleToUInt64(begin[ k ]);
            }

            mStreamWriter.WritePrimitiveBlock(mU64Buffer, blockSize);
        }
    }

private:

    TStreamWriter mStreamWriter;

    uint64_t mU64Buffer[ 128 ];
    uint32_t mU32Buffer[ 128 ];

    BinarySerMessage(BinarySerMessage &);

    BinarySerMessage &operator=(const BinarySerMessage &bsm);

    template< typename T >
    SERIALISATION_FORCEINLINE void WriteHeader(const T index, Internal::Type::Type type)
    {
        const T header = Util::CreateHeader(index, ToBinaryType(type));

        WritePrimitive(header);
    }

    inline Internal::Type::Type ToBinaryType(const Internal::Type::Type type)
    {
        Internal::Type::Type binaryType = type;

        if (Internal::Type::IsSignedInt(type))
        {
            binaryType = static_cast< Internal::Type::Type >(type - Internal::Type::SInt8 + Internal::Type::UInt8);
        }
        else if (type == Internal::Type::Float)
        {
            binaryType = Internal::Type::UInt32;
        }
        else if (type == Internal::Type::Double)
        {
            binaryType = Internal::Type::UInt64;
        }

        return binaryType;
    }

    template< typename TPrimitive >
    SERIALISATION_FORCEINLINE void WritePrimitive(TPrimitive &value)
    {
        mStreamWriter.WritePrimitive(value);
    }

    SERIALISATION_FORCEINLINE void WritePrimitive(std::string &value)
    {
        mStreamWriter.WriteSize(value.length());
        mStreamWriter.WriteBytes(value.c_str(), value.length());
    }
};

#endif
