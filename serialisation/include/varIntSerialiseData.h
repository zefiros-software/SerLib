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
        return ( U )mValue;
    }

    virtual Type GetType() const;

    virtual size_t Size() const;

    virtual void ReadFromStream( std::istream &stream );

    virtual void WriteToStream( std::ostream &stream ) const;

private:

    U64 mValue;
};

#endif