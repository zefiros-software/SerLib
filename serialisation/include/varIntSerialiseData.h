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
    void Store( U &val, Mode::Mode mode )
    {
        switch ( mode )
        {
        case Mode::Serialise:
            mValue = val;
            break;

        case Mode::Deserialise:
            val = ( U )mValue;
            break;
        }
    }

    virtual Type::Type GetType() const;

    virtual size_t Size() const;

    virtual void ReadFromStream( std::istream &stream );

    virtual void WriteToStream( std::ostream &stream ) const;

private:

    uint64_t mValue;
};

#endif