#pragma once
#ifndef __VARINTSERIALISEDATA_H__
#define __VARINTSERIALISEDATA_H__

#include "serialiseData.h"
#include "varint.h"

class VarIntSerialiseData
    : public ISerialiseData
{
public:

    template< typename U >
    void SetValue( U val )
    {
        mValue = val;
    }

    template< typename U >
    U GetValue() const
    {
        return static_cast< U >( mValue );
    }

    virtual Type::Type GetType() const;

    virtual size_t Size() const;

    virtual void ReadFromStream( std::istream &stream );

    virtual void WriteToStream( std::ostream &stream ) const;

private:

    uint64_t mValue;
};

#endif