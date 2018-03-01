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
#ifndef __SERIALISATION_MESSAGEADAPTER_H__
#define __SERIALISATION_MESSAGEADAPTER_H__

#include "serialisation/internalMessage.h"

template< typename TMessage >
class MessageAdapter
    : public InternalMessage
{
public:

    explicit MessageAdapter(const std::string &fileName)
        : mMessage(fileName)
    {
    }

    explicit MessageAdapter(std::stringstream &stream)
        : mMessage(stream)
    {

    }

    explicit MessageAdapter(std::iostream &stream)
        : mMessage(stream)
    {

    }

    explicit MessageAdapter(std::ifstream &stream)
        : mMessage(stream)
    {
    }

    explicit MessageAdapter(std::fstream &stream)
        : mMessage(stream)
    {
    }

    explicit MessageAdapter(std::istream &stream)
        : mMessage(stream)
    {
    }

    explicit MessageAdapter(std::ofstream &stream)
        : mMessage(stream)
    {
    }

    explicit MessageAdapter(std::ostream &stream)
        : mMessage(stream)
    {
    }

    virtual void ClearBuffer()
    {
        mMessage.ClearBuffer();
    }

    virtual void InitObject()
    {
        mMessage.InitObject();
    }

    virtual inline bool InitObject(uint8_t index)
    {
        return mMessage.InitObject(index);
    }

    virtual void FinishObject()
    {
        mMessage.FinishObject();
    }

    virtual inline void FinishObject(uint8_t index)
    {
        mMessage.FinishObject(index);
    }

    virtual inline bool InitParent(uint8_t index)
    {
        return mMessage.InitParent(index);
    }

    virtual inline void FinishParent(uint8_t index)
    {
        mMessage.FinishParent(index);
    }

    virtual inline void InitArrayObject()
    {
        mMessage.InitArrayObject();
    }

    virtual inline void FinishArrayObject()
    {
        mMessage.FinishArrayObject();
    }

    virtual inline void Store(std::string &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(uint8_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(uint16_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(uint32_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(uint64_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(int8_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(int16_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(int32_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(int64_t &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }


    virtual inline void Store(float &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline void Store(double &value, uint8_t index)
    {
        mMessage.Store(value, index);
    }

    virtual inline size_t CreateArray(Type::Type type, size_t size, uint8_t index, uint8_t flags = 0x00)
    {
        return mMessage.CreateArray(type, size, index, flags);
    }

    virtual inline void StoreArrayItem(std::string &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(uint8_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(uint16_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(uint32_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(uint64_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(int8_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(int16_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(int32_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(int64_t &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(float &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreArrayItem(double &value)
    {
        mMessage.StoreArrayItem(value);
    }

    virtual inline void StoreContiguous(uint8_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(uint16_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(uint32_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(uint64_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(int8_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(int16_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(int32_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(int64_t *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(float *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

    virtual inline void StoreContiguous(double *begin, size_t size)
    {
        mMessage.StoreContiguous(begin, size);
    }

protected:

    TMessage mMessage;

private:

    MessageAdapter &operator=(const MessageAdapter &);
};
#endif