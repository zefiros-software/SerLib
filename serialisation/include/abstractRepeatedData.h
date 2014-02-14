#pragma once
#ifndef __ABSTRACTSERIALISEDATA_H__
#define __ABSTRACTSERIALISEDATA_H__

#include "serialiseData.h"
#include "types.h"

class AbstractRepeatedData
    : public ISerialiseData
{
public:

    virtual ISerialiseData *const GetSerialisable( const size_t index ) = 0;

    virtual Type::Type GetType() const
    {
        return Type::Repeated;
    }

    virtual Type::Type GetSubType() const = 0;

    virtual size_t Size() const = 0;

    virtual void Resize( const size_t size ) = 0;

    virtual uint32_t GetFieldCount() const = 0;
};

#endif